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

-- 创建 scene, 将可用区域中心设为 0,0 点
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
gX1 = -gW
gY1 = -gH
gX2 = 0
gY2 = -gH
gX3 = gW
gY3 = -gH
gX4 = -gX
gY4 = 0
gX5 = 0
gY5 = 0
gX6 = gW
gY6 = 0
gX7 = -gW
gY7 = gH
gX8 = 0
gY8 = gH
gX9 = gW
gY9 = gH


-- avoid memory leak
collectgarbage("setpause", 100)
collectgarbage("setstepmul", 5000)


-- 测试按键

gKeyCodes = {}
gTouchs = {}

local ELK = cc.EventListenerKeyboard.create()
ELK:onKeyPressed(function(kc, e)
	gKeyCodes[kc] = true
end)
ELK:onKeyReleased(function(kc, e)
	gKeyCodes[kc] = false
end)
cc.addEventListenerWithFixedPriority(ELK, -1)

-- 测试点击

local ELT = cc.EventListenerTouchOneByOne.create()
local addTouch = function(t, e)
	gTouchs[t:getID()] = t
	return true
end
local removeTouch = function(t, e)
	gTouchs[t:getID()] = nil
end
ELT:onTouchBegan(addTouch)
ELT:onTouchEnded(removeTouch)
ELT:onTouchCancelled(removeTouch)
cc.addEventListenerWithFixedPriority(ELT, -1)


-- 绘制一个 ASDW 控制的 sprite. touch 发生时向其坐标发射 bullet
go(function()
	local imgBody = cc.addImage("man.png")
	local imgBullet = cc.addImage("bullet.png")
	imgBody:retain()
	imgBullet:retain()

	local bullets = List_Create()

	bullets.Create = function(x, y, incX, incY, a)
		local bullet = 
		{
			0,
			cc.Sprite.Create_FileName_Owner_Positon_Anchor_Scale("bullet.png", gScene, x, y), 
			x, y, 
			incX, incY
		}
		bullets.Add(bullet)
		bullet[1] = #bullets
		bullet[2]:setRotation(-a)
		bullet.Release = function()
			bullet[2]:removeFromParent()
			local idx = bullet[1]
			bullets[#bullets][1] = idx
			bullets.SwapRemoveAt(idx)
		end
		bullet.Move = function()
			local x, y = bullet[3] + bullet[5], bullet[4] + bullet[6]
			if x < -740 or x > 740 or y < -460 or y > 460 then
				bullet.Release()
			else
				bullet[3], bullet[4] = x, y
				bullet[2]:setPosition(x, y)
			end
		end
		return bullet
	end

	local x, y = 0, 0
	local o = cc.Sprite.Create_FileName_Owner_Positon_Anchor_Scale("man.png", gScene, x, y, 0.5, 0.5)
	local keys = 
	{
		cc.KeyCode.KEY_A, 
		cc.KeyCode.KEY_S, 
		cc.KeyCode.KEY_D, 
		cc.KeyCode.KEY_W
	}
	local funcs = 
	{ 
		function() x = x - 1 end, 
		function() y = y - 1 end, 
		function() x = x + 1 end, 
		function() y = y + 1 end
	}
	while true do
		yield()

		-- move man
		for i = 1, #keys do
			if gKeyCodes[keys[i]] then
				funcs[i]()
			end
		end
		o:setPosition(x, y)

		-- fire
		for i, t in pairs(gTouchs) do
			local tx, ty = o:convertTouchToNodeSpaceAR(t)
			local bx, by = 145, 0
			if tx < 0 then
				bx = -145
			end
			local a = 0
			tx, ty, a = GetXyIncAngle(tx - bx, ty - by)
			bullets.Create(x + bx, y + by, tx, ty, a)
		end

		-- move bullets
		for i = #bullets, 1, -1 do
			bullets[i].Move()
		end
	end
end)
