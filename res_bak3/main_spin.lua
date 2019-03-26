require "libs.lua"

-- 初始化显示相关

-- 定显示尺寸
gW = 1280
gH = 720

-- 创建显示窗口( for desktop os )
cc.createSetOpenGLView("cocos_cpp_lua", gW, gH)

-- 设置设计尺寸和适配模式( 智能留黑边 )
cc.setDesignResolutionSize(gW, gH, cc.ResolutionPolicy.SHOW_ALL)

-- 打开 ogl 帧统计信息显示
cc.setDisplayStats(true)

-- 设置帧数
cc.setAnimationInterval(1 / 60)

-- 取可用区域坐标宽高
gX, gY, gW, gH = cc.getSafeAreaRect()
gW2 = gW/2
gH2 = gH/2

-- 创建 scene, 将 safe area 中心设为 0,0 点
gScene = cc.Scene.create()
gScene:setContentSize(0, 0)
gScene:setPosition(gX + gW2, gY + gH2)
gScene:setIgnoreAnchorPointForPosition(true)

-- 加载并运行场景
cc.runWithScene(gScene)

--[[
键盘小键盘数字区九点定位相关变量初始化. 5 为 0, 0 点
7 8 9
4 5 6
1 2 3
]]
gX1 = -gW2
gX4 = -gW2
gX7 = -gW2
gX2 = 0
gX5 = 0
gX8 = 0
gX3 = gW2
gX6 = gW2
gX9 = gW2
gY1 = -gH2
gY2 = -gH2
gY3 = -gH2
gY4 = 0
gY5 = 0
gY6 = 0
gY7 = gH2
gY8 = gH2
gY9 = gH2


-- avoid memory leak
collectgarbage("setpause", 100)
collectgarbage("setstepmul", 5000)



-- 根据当前网上随便找的一些素材规划一些上下文
local spriteFrameNames = 
{
	symbolNormals = {
		'SH1-00.png',
		'SH2-00.png',
		'SM1-00.png',
		'SM2-00.png',
		'SM3-00.png',
		'SL1-00.png',
		'SL2-00.png',
		'SL3-00.png',
		'SWD-00.png'
	},
	symbolBlurs = {
		'SH1-r.png',
		'SH2-r.png',
		'SM1-r.png',
		'SM2-r.png',
		'SM3-r.png',
		'SL1-r.png',
		'SL2-r.png',
		'SL3-r.png',
		'SWD-r.png'
	},
	symbolHighlights = {
		'SH1-00.png',
		'SH2-00.png',
		'SM1-00.png',
		'SM2-00.png',
		'SM3-00.png',
		'SL1-00.png',
		'SL2-00.png',
		'SL3-00.png',
		'SWD-00.png'
	},
	symbolGrays = {
		'SH1-b.png',
		'SH2-b.png',
		'SM1-b.png',
		'SM2-b.png',
		'SM3-b.png',
		'SL1-b.png',
		'SL2-b.png',
		'SL3-b.png',
		'SWD-b.png'
	},
	symbolAlphas = {
		'H1.png',
		'H2.png',
		'M1.png',
		'M2.png',
		'M3.png',
		'L1.png',
		'L2.png',
		'L3.png',
		'WILD.png'
	}
}

-- 预加载
cc.addSpriteFramesWithFile("symbol.plist")

-- item 部分参数
local itemWidth = 133
local itemHeight = 133
local itemHeight2 = itemHeight / 2
local numSymbols = #spriteFrameNames.symbolNormals

-- 画按钮并设置一个 touch up 的事件处理回调
local BtnCreate = function(sfName, parent, pX, pY, aX, aY, sX, sY, touchUpFunc)
	local spr = cc.Sprite.Create_FileName_Owner_Positon_Anchor_Scale(sfName, parent, pX, pY, aX, aY, sX, sY)
	local tl = cc.EventListenerTouchOneByOne.create()
	tl:onTouchBegan(function(touch, event) return spr:containsTouch(touch) end)
	tl:onTouchEnded(function(touch, event) touchUpFunc(touch) end)
	cc.addEventListenerWithSceneGraphPriority(tl, spr)
	return spr
end

-- 画格子
local CellCreate = function(container, w, h, x, y)
	local cn = cc.ClippingNode.create()
	cn:setStencilRect(w, h)
	cn:setPosition(x, y)
	container:addChild(cn)
	return cn
end

-- 需求分析:
-- 队列看上去是一个固定序列( 看上去就是 sfName 数组顺序 ), 从上往下依次出现, 最终要停在哪张是通过滚动起始索引和距离来决定的, 无视当前图
-- 格子里应该至少有 2 个 sprite 复用. 映射到队列中. 根据位置和速度来决定使用清晰还是模糊的图. 以及显示坐标.

-- 画一下各张图看看顺序. 看上去有 9 个符号. 每个 133 像素高度.
for i = 1, numSymbols do
	cc.Sprite.Create_SpriteFrameName_Owner_Positon_Anchor_Scale(spriteFrameNames.symbolNormals[i], gScene, gX4 + 40 + i * 133, gY4 + 266, 1, 0.5)
end

-- 构造一个 item 结构体并返回
local ItemCreate = function(cell)
	local item =
	{
		pos = 0					-- 当前位置( Spin 时指定 ). fmod, 取整 + 1 后即为 symbolId. 显示时根据小数部分来计算前后两张的 offset
		, startPos = 0			-- 开始滚动的位置( Spin 时指定 ). 显示时如果判断 pos 值取整后 == startPos, 则用 lastSymbolId 来作为显示用图
		, endPos = 1			-- 结束滚动的位置( Spin 时指定 )
		, lastSymbolId = 1		-- 上次的结果, 用于展示
		, state = 0				-- 0: stoped   1: move   2: bounce ( Spin 时修改, 停止后为 0 )
	}

	-- 设置最后停的位置( 初始显示后第1次会用到, 修改静态展示图 )
	item.SetLastSymbol = function(symbolId)
		item.lastSymbolId = symbolId
		item.Update()
	end

	-- 开始滚动. 设置从多少滚动到多少.
	item.Spin = function(startPos, endPos, step)
		-- 参数检查与准备
		assert(item.state == 0)
		assert(startPos < endPos)
		step = step or 2 / 9
		item.pos = startPos
		item.startPos = startPos
		item.endPos = endPos

		-- 启动协程并立即执行一次
		gorun(function()
			-- 开始滚动
			item.state = 1
			while true do
				-- 滚一小段距离
				item.pos = item.pos + step
				item.Update()
				yield()

				-- 如果滚到超出了范围就跳出
				if item.pos > item.endPos then
					break
				end
			end

			-- 开始播放弹性效果. 先减速继续移点点, 弹回高点, 再用2帧跌落静止
			item.state = 2

			item.pos = item.pos + step / 2
			item.Update()
			yield()

			item.pos =  item.pos - step
			item.Update()
			yield()

			item.pos =  item.pos - step
			item.Update()
			yield()

			item.pos =  item.pos + (item.endPos - item.pos) / 2
			item.Update()
			yield()

			item.pos = item.endPos
			item.Update()
			yield()

			-- 停止
			item.lastSymbolId = math.fmod(item.endPos, numSymbols) + 1
			item.state = 0
		end)
	end

	-- 试着停止滚动( 并非马上停下来 )
	item.Stop = function()
		if item.state ~= 1 then return end
		-- 缩减 item.endPos 的值, 令其刚好大于 pos 且最终 symbolId 不变, 以实现速度停止
		local m = math.floor(item.pos / numSymbols)
		local offset = math.fmod(item.endPos, numSymbols)
		item.endPos = numSymbols * m + offset
	end

	item.Update = function()
		if item.state == 0 then
			item.sprite1:setSpriteFrame(spriteFrameNames.symbolNormals[item.lastSymbolId])
			item.sprite1:setPosition(0, 0)
			item.sprite2:setVisible(false)
		else
			item.sprite2:setVisible(true)

			local offset = math.fmod(item.pos, numSymbols)
			local floor_offset = math.floor(offset)

			local symbolId_1 = floor_offset + 1
			local symbolId_2 = symbolId_1 + 1
			if symbolId_2 > numSymbols then
				symbolId_2 = 1
			end

			local blur1 = nil
			local blur2 = nil

			if item.pos - item.startPos < 1 then
				symbolId_1 = item.lastSymbolId
				blur1 = false
				blur2 = true
			else
				blur1 = true
				blur2 = true
			end

			if item.endPos - item.pos < 2 then
				blur2 = false
			end

			if item.endPos - item.pos < 1 then
				blur1 = false
			end

			if blur1 then
				item.sprite1:setSpriteFrame(spriteFrameNames.symbolBlurs[symbolId_1])
			else
				item.sprite1:setSpriteFrame(spriteFrameNames.symbolNormals[symbolId_1])
			end

			if blur2 then
				item.sprite2:setSpriteFrame(spriteFrameNames.symbolBlurs[symbolId_2])
			else
				item.sprite2:setSpriteFrame(spriteFrameNames.symbolNormals[symbolId_2])
			end

			local digit = offset - floor_offset
			item.sprite1:setPosition(0, -133 * digit)
			item.sprite2:setPosition(0, 134 - 133 * digit)
		end
	end

	item.sprite1 = cc.Sprite.Create_SpriteFrameName_Owner_Positon_Anchor_Scale(spriteFrameNames.symbolNormals[item.lastSymbolId], cell)
	item.sprite2 = cc.Sprite.Create_SpriteFrameName_Owner_Positon_Anchor_Scale(spriteFrameNames.symbolNormals[item.lastSymbolId], cell)
	return item
end

-- 画一个旋转按钮和一个 cell. 点击后开始转动
local cell = CellCreate(gScene, 133, 133, gX5, gY5)
local item = ItemCreate(cell)
item.SetLastSymbol(3)
local btnSpin = BtnCreate("spin_CN.png", gScene, gX3, gY3, 1, 0, 1, 1, function(touch)
	--local startPos = math.random(1, numSymbols) - 1
	--local endPos = startPos + math.random(1, 30)
	--item.Spin(startPos, endPos)
	item.Spin(item.lastSymbolId, item.lastSymbolId + 4)
end)
