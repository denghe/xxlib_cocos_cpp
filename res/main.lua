require "libs.lua"

-- avoid memory leak
collectgarbage("setpause", 100)
collectgarbage("setstepmul", 5000)

-- 创建显示窗口( for desktop os )
cc.createSetOpenGLView("cocos_cpp_lua", 800, 450)

-- 初始化设计尺寸( 如果不设置，iOS 上左下角统计会显示不出来 )
local sW, sH = cc.getFrameSize()
cc.setDesignResolutionSize( sW, sH, cc.ResolutionPolicy.NO_BORDER )

-- 打开 ogl 帧统计信息显示
cc.setDisplayStats(true)

-- 设置帧数
cc.setAnimationInterval(1 / 60)

-- 取可用区域坐标宽高
gX, gY, gW, gH = cc.getSafeAreaRect()

gW2 = gW/2
gH2 = gH/2

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


-- 创建 scene, 将 safe area 中心设为 0,0 点( 有可能导致部分 cocos 基础组件坐标或裁剪出问题 )
gScene = cc.Scene.create()
gScene:setContentSize(0, 0)
gScene:setPosition(gX + gW2, gY + gH2)
gScene:setIgnoreAnchorPointForPosition(true)

-- 加载并运行场景
cc.runWithScene(gScene)

-- 启动主携程
go(function()
-- 暂缓一帧 ( 否则 Director getRunningScene 取不到 )
yield()




-- 开始逻辑代码
go(function()
	require "PKG_class.lua"

	local domain = "192.168.1.52"	--"www.baidu.com"
	local ips = {}
	local resolver = xx.UvResolver.Create()

	local dialer = xx.UvDialer.Create(1)  -- 2)
	local peer = nil
	dialer:OnAccept(function(p)
		peer = p
		if p ~= nil then
			p:ResetTimeoutMS(5000)
		end
	end)

	local recvs = {}

::LabResolveIPs::
	print("resolve domain = ".. domain)
	resolver:Cancel();
	local r = resolver:Resolve(domain, 3000)
	if r ~= 0 then
		print("r = " .. r)
		gSleepSecs(3)
		goto LabResolveIPs
	end
	while resolver:Busy() do
		yield()
	end
	ips = resolver:GetIPList()
	if #ips == 0 then
		goto LabResolveIPs
	end
	DumpPackage(ips)

::LabDial::
	print("dial...")
	dialer:Cancel()
	r = dialer:Dial(ips, 20001, 500)
	if r ~= 0 then
		print("r = " .. r)
		gSleepSecs(3)
		goto LabResolveIPs
	end
	while dialer:Busy() do
		yield()
	end
	if peer == nil then
		goto LabResolveIPs
	end
	print("connected. ip = ".. peer:GetIP())
	if peer:IsKcp() then
		print("protocol: kcp")
	end

	peer:OnReceivePush(function(bb)
		print("recv " .. tostring(bb))
		local o = gReadRoot(bb)
		if o == nil then
			return -1
		else
			recvs[#recvs + 1] = o
			return 0
		end
	end)

	local pkg = PKG_Generic_Xxx.Create()
	pkg.ticks = 12345
	gSendPush(peer, pkg)
	gSendPush(peer, pkg)
	gSendPush(peer, pkg)
	print("send pkgs")

	while not peer:Disposed() do
		yield()
		local len = #recvs
		if len ~= 0 then
			for i = 1, len do
				DumpPackage(recvs[i])
			end
			recvs = {}
		end
	end
	print("disconnected.")
	goto LabResolveIPs
end)




end)



--[[
go(function()
	local dialer = xx.UvDialerPeer.Create()
::LabDial::
	print("::LabDial::");
	dialer:Cleanup()
	local r = dialer:Dial({ "192.168.1.121" }, 20000, 500)
	if r ~= 0 then
		print("r = " .. r)
		gSleepSecs(3)
		goto LabDial
	end
	while dialer:Busy() do
		yield()
	end
	if not dialer:PeerAlive() then
		goto LabDial
	end
	print("connected...")
	dialer:OnAccept(function(peer)
		print("sim peer accepted: serviceId = " .. peer:GetId())
		peer:OnDisconnect(function()
			print("sim peer disconnected: serviceId = " .. peer:GetId())
		end)
		gBB:Clear()
		gBB:WriteRoot("asdf")
		peer:SendPush(gBB)
		print("send gBB")
	end)
	while dialer:PeerAlive() do
		yield()
	end
	print("disconnected...")
	gSleepSecs(3)
	goto LabDial
end)
]]


--[[
go(function()
	local yield = yield
	-- 暂缓一帧 ( 否则 Director getRunningScene 取不到 )
	yield()

	local cf = CatchFish.Create()
	if cf == nil then
		print("CatchFish create fail.")
	else
		--local r = cf:Init("192.168.1.149", 12345, "cfg.bin")
		--local r = cf:Init("192.168.1.163", 12345, "cfg.bin")
		--local r = cf:Init("192.168.1.154", 12345, "cfg.bin")
		--local r = cf:Init("192.168.1.254", 12345, "cfg.bin")
		--local r = cf:Init("10.0.0.216", 12345, "cfg.bin")
		local r = cf:Init("127.0.0.1", 12345, "cfg.bin")
		if r ~= 0 then
			print("CatchFish Init fail. r = ".. r)
		else
			while true do
				local r = cf:Update()
				if r ~= 0 then
					print("CatchFish Update fail. r = ".. r)
					break
				end
				yield()
			end
		end
		-- todo: get result from cf
		cf:Reset()
		cf = nil
	end
end)
]]
