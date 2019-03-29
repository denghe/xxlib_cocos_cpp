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

local StartClient = function(domain, port, timeoutMS, useKcp)
go(function()
	local yield = yield
	local indent = "tcp: "
	if useKcp then
		indent = "                                                                 || kcp: "
	end
::LabRetry::
	local ips = GetIPList(domain, timeoutMS)
	if #ips == 0 then
		print(indent .. "get ip timeout")
		goto LabRetry 
	end
	print(indent.."get ip success. ips: ")
	for _, ip in ipairs(ips) do
		print(indent ..ip)
	end
	print(indent.. "dial.")
	local peer = NetDial(ips, port, timeoutMS, useKcp)
	if peer == nil then
		print(indent.. "dial timeout")
		goto LabRetry
	end
	print(indent..peer:GetIP(false) .." send ping")

	-- todo: set timeout
	peer:ResetTimeoutMS(3000)

	peer:OnReceivePush(function(bb)
		io.write(indent.."recv ")
		print(bb)
	end)

	local pkg = NET_Generic_Ping.Create()
	pkg.ticks = 123--xx.NowSteadyEpochMS()
	NetSendPush(peer, pkg)

	print(indent.."wait disconnect")
	while true do
		if peer:Disposed() then
			break
--		else
--			print(".")
		end
		yield()
	end
	print(indent.."disposed.")
	goto LabRetry
end)
end

StartClient("192.168.1.254", 12345, 2000, true)
StartClient("192.168.1.254", 12345, 2000, false)
