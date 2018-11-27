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
-- 队列具有下列操作函数:
-- SetResult(int symbolId): 在队列当前位置插入一张用于展示先前结果的图, 当移到看不到时, 从队列中移除
-- SetPosition(int symbolId): 重建队列, 且改变队列当前指向. SetResult 的内容做相应的移动.
-- Spin(int steps): 开始滚动. 传入滚多少格. 视角表现为通过 SetPos 设的那张图将以模糊态进入视野并持续模糊 steps - 2 后, 最后两张清晰, 最后一张弹一下. 
-- Stop(int symbolId): 逻辑上令滚动立即停止, 模糊的继续滚, 2张的继续 在其位置的下一张, 插入 symbolId 图, 带出清晰的 symbolId 图
-- SpinOnce(): 只转1格, 展示 SetPos 的结果出来.

-- 画一下各张图看看顺序. 看上去有 9 个符号. 每个 133 像素高度.
for i = 1, numSymbols do
	cc.Sprite.Create_SpriteFrameName_Owner_Positon_Anchor_Scale(spriteFrameNames.symbolNormals[i], gScene, gX4 + i * 133, gY4, 1, 0.5)
end

-- 考虑使用 float 下标, 与 symbol 对应. * 133 后与显示 offset 相对应, 滚动就是 + 0.225. 大于 9 时 -= 9. 小于 0 时 += 9
-- 需要显示 Result 时，sprite 显示映射受其影响. 

-- 构造一个 item 结构体并返回
local ItemCreate = function(cell, symbolId)
	local item = {}
	item.positon = 9	-- 当前位置
	item.result = 1		-- 结果展示
	item.state = 0		-- 0: stoped   1: move   2: bounce

	item.SetResult = function(symbolId)
		item.result = symbolId
	end

	item.SetPosition = function(symbolId)
		item.positon = symbolId
	end

	item.SpinCore = function(step)
		item.sum = item.sum + step
		item.position = item.position + step
		if item.position < 0 then
			item.position = item.position + numSymbols
		end
		if item.position > numSymbols then
			item.position = item.position - numSymbols
		end
		item.Update()
	end

	item.Spin = function(times, step)
		assert(item.state == 0)
		step = step or -2/9	-- 不传默认向下滚
		assert(step <= numSymbols or step >= -numSymbols)
		go(function()
			item.state = 1
			item.sum = 0
			while true do
				-- todo: check stop click
				item.SpinCore(step)
				yield()
				if math.abs(item.sum) > times then
					break
				end
			end
			item.state = 2

			-- todo: 模拟弹性效果
			item.moving = false
		end)
	end

	item.Stop = function(symbolId)
	end

	item.SpinOnce = function()
	end

	item.sprite1 = cc.Sprite.Create_SpriteFrameName_Owner_Positon_Anchor_Scale(spriteFrameNames.symbolNormals[symbolId], cell)
	item.sprite2 = cc.Sprite.Create_SpriteFrameName_Owner_Positon_Anchor_Scale(spriteFrameNames.symbolNormals[symbolId], cell)
	item.Update = function()
		-- todo: 根据当前状态和 offset, 使用 sprite1 & 2 在相应坐标显示相应图片
		--item.sprite1:setPosition()
		--item.sprite2:setPosition()  setSpriteFrame
	end
	item.Update()
	return item
end

--[[
-- 画一个旋转按钮和一个 cell. 点击后开始转动, 2 秒后停止
go(function()
	local cell = CellCreate(gScene, 133, 133, gX5, gY5)
	local items = {}
	items[1] = ItemCreate(cell, 1, 0, itemHeight)
	items[2] = ItemCreate(cell, 2, 0, 0)
	items[3] = ItemCreate(cell, 3, 0, -itemHeight)
	items.Move = function()
		items[1].Move()
		items[2].Move()
		items[3].Move()
	end
	local btnSpin = BtnCreate("spin_CN.png", gScene, gX3, gY3, 1, 0, 1, 1, function(touch)
		items.Move()
	end)
end)
]]