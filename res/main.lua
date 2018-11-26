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

-- 画格子里面的元素
local ItemCreate = function(cell, itemId, x, y)
	local item = {}
	item.sprite = cc.Sprite.Create_SpriteFrameName_Owner_Positon_Anchor_Scale(spriteFrameNames.symbolNormals[itemId], cell)
	item.x = x
	item.y = y
	item.UpdatePos = function()
		item.sprite:setPosition(item.x, item.y)
	end
	item.moving = false
	item.Move = function()
		go(function()
			item.moving = true
			local destY = item.y + itemHeight
			while item.y <= destY do
				item.y = item.y + 5	-- todo: 变速
				item.UpdatePos()
				yield()
			end
			item.moving = false
		end)
	end
	item.UpdatePos()
	return item
end

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


local panel = {}
panel.opened = false
panel.Open = function()
	....
	panel.opened = true
	go(function()
		-....
		while panel.opened do
			yield()
			-- ...
		end
	end)
end
panel.Close = function()
	....
	panel.opened = false
end
return panel

xx.lua


local p = require "xx.lua"
SetState(p)