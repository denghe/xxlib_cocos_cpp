require "libs.lua"

-- avoid memory leak
collectgarbage("setpause", 100)
collectgarbage("setstepmul", 5000)

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

require "NET_class.lua"

go(function()
	local yield = yield
	local bb = BBuffer.Create()

::LabRetry::
	local ips = GetIPList("192.168.1.254", 2000)
	if #ips == 0 then
		print("get ip timeout")
		goto LabRetry 
	end
	print("get ip success. ips: ")
	for _, ip in ipairs(ips) do
		print(ip)
	end
	print("dial.")
	local peer = NetDial(ips, 12345, 2000, false)
	if peer == nil then
		print("dial timeout")
		goto LabRetry
	end
	print(peer:GetIP(false) .. " send ping")

	-- todo: set timeout

	local pkg = NET_Generic_Ping.Create()
	pkg.ticks = xx.NowSteadyEpochMS()
	bb:Clear()
	bb:WriteRoot(pkg)
	peer:SendPush(bb)

	while not peer:Disposed() do
		yield()
	end
	print("disposed.")
end)

--[[
go(function()
::LabRetry::
	local ips = GetIPList("www.baidu.com", 2000)
	if #ips == 0 then
		print("get ip timeout")
		goto LabRetry 
	end
	for _, ip in ipairs(ips) do
		print(ip)
	end
	local peer = NetDial(ips, 80, 2000)
	if peer == nil then
		print("dial timeout")
		goto LabRetry
	end
	print(peer:GetIP(false) .. " connected")
	while not peer:Disposed() do
		yield()
	end
	print("disconnected.")
end)
]]
