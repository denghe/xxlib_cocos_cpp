----------------------------------------------------------------------
-- 常用全局函数 & 变量
----------------------------------------------------------------------

-- 常用函数减少查表次数
coroutine_create = coroutine.create
coroutine_status = coroutine.status
resume = coroutine.resume
yield = coroutine.yield
table_unpack = table.unpack


-- 简单的 List 结构
List_Create = function()
	local t = {}
	t.Add = function(o)
		t[#t + 1] = o
	end
	t.SwapRemoveAt = function(idx)
		local dl = #t
		assert(idx > 0 and idx <= dl)
		if idx < dl then
			t[idx] = t[dl]
		end
		t[dl] = nil
	end
	return t
end


-- 方便输出表结构
function DumpTable( t )  
    local cache = {}
    local function DumpSubTable(t,indent)
        if (cache[tostring(t)]) then
            print(indent.."*"..tostring(t))
        else
            cache[tostring(t)] = true
            if (type(t)=="table") then
                for pos,val in pairs(t) do
                    if (type(val)=="table") then
                        print(indent.."["..pos.."] => "..tostring(t).." {")
                        DumpSubTable(val,indent..string.rep(" ",string.len(pos)+8))
                        print(indent..string.rep(" ",string.len(pos)+6).."}")
                    elseif (type(val)=="string") then
                        print(indent.."["..pos..'] => "'..val..'"')
                    else
                        print(indent.."["..pos.."] => "..tostring(val))
                    end
                end
            else
                print(indent..tostring(t))
            end
        end
    end
    if (type(t)=="table") then
        print(tostring(t).." {")
        DumpSubTable(t,"  ")
        print("}")
    else
        DumpSubTable(t,"  ")
    end
    print()
end


-- 方便输出包结构
function DumpPackage(t)
	function DumpPackageCore(t, indent)
		local spaces = string.rep(" ",indent)
		print(spaces.."typeName : "..t.__proto.typeName)
		for k, v in pairs(t) do
			if k == "__proto"
				or k == "__index"
				or k == "__newindex"
				or k == "__isReleased"
				or k == "Release" then
				-- do nothing
			elseif type(v) == "table" then
				DumpPackageCore(v, indent + 4)
			else
				print(spaces.."["..k.."] = "..tostring(v))
			end
		end
	end
	local ts = { t }
	function GetMetatables(t)
		local mt = getmetatable(t)
		if mt ~= nil then
			ts[#ts + 1] = mt
			GetMetatables(mt)
		end
	end
	GetMetatables(t)
	for i = #ts, 1, -1 do
		DumpPackageCore(t, (#ts - i) * 4)
	end
end



-- 全局协程池( 乱序 )
gCoros = List_Create()

-- 压入一个可以是协程的函数. 有参数就跟在后面. 有延迟执行的效果.
gCoros_Push = function(func, ...)
	--print("push")
	local args = {...}
	if #args == 0 then
		gCoros.Add(coroutine_create(func))
	else
		gCoros.Add(coroutine_create(function() func(table_unpack(args)) end))
	end
end

-- for easy use
go = gCoros_Push

-- 阻塞 n 帧
-- 适合在协程环境使用
Sleep = function(n)
	for _=1, n do
		yield()
	end
end

-- 阻塞 n 秒( 按 60 帧粗算的 )
-- 适合在协程环境使用
SleepSecs = function(n)
	local n = math.floor(n * 60)
	for _=1, n do
		yield()
	end
end




-- 状态机集合
gStates = {}		

-- 根据状态名查找是否存在
function gStates_Exists(name)
	local t = gStates
	for _, s in pairs(t) do
		if s.stateName == name then 
			return true 
		end
	end
	return false
end

-- Open 一个状态( 会自动 Close 相同 state.stateGroupName 下的旧状态, 接着 Open 新状态 ), Open 完后返回
-- 适合在协程环境使用
function gStates_Open(state)
	local s = gStates[state.stateGroupName]
	if s ~= nil then
		s.Close()
	end
	gStates[state.stateGroupName] = state
	state.Open(s)	-- 传入 prev 状态
end

-- 关状态, 关完后返回( 根据组名定位 )
-- 适合在协程环境使用
function gStates_CloseByGroupName(stateGroupName)
	local s = gStates[stateGroupName]
	if s ~= nil then
		s.Close()
	end
	gStates[stateGroupName] = nil
end

-- 关状态, 关完后返回
-- 适合在协程环境使用
function gStates_Close(state)
	gStates_CloseByGroupName(state.stateGroupName)
end

-- 等 state 出现
-- 适合在协程环境使用
gStates_WaitAppear = function(state)
	local yield = yield
	while not gStates_Exists(state.stateName) do
		yield()
	end
end

-- 等 state 出现 并 消失
-- 适合在协程环境使用
gStates_WaitDisappear = function(state)
	gStates_WaitAppear(state)
	local yield = yield
	while gStates_Exists(state.stateName) do
		yield()
	end
end


-- 域名解析. 返回 { ip list }. 长度为 0 意味着解析失败或超时
-- 适合在协程环境使用
GetIPList = function(domain, timeoutSec)
	local rt = { 1 }
	xx.UvLoop.GetIPList(domain, timeoutSec, function(...)
		rt[1] = {...}
	end)
	while rt[1] == 1 do
		yield()
	end
	return rt[1];
end


-- 创建一个 tcp client 并解析域名 & 连接指定端口. 多 ip 域名将返回最快连上的. 超时时间可能因域名解析而比指定的要长. 不会超过两倍
-- 如果域名解析失败, 所有ip全都连不上, 超时, 回调将传入空.
-- domainName 也可以直接就是一个 ip. 这样会达到在 ipv6 协议栈下自动转换 ip 格式的目的
-- 同时, 对 apple 手机应用来讲, 调用本函数 或 GetIPList 可达到弹出网络权限请求面板的效果
-- 如果反复针对相同域名发起查询, 且上次的查询还没触发回调, 将返回 false.
-- 适合在协程环境使用
function CreateUvTcpClient(domain, port, timeoutSec)
	local rtv = { null }
	local b = xx.UvLoop.CreateTcpClientEx(domain, port, function(c)
		rtv[1] = c
	end, timeoutSec)
	while rtv[1] == null do
		yield()
	end
	return rtv[1]
end

-- 上面函数的 lua 实现( 备用 )
function CreateUvTcpClient2(domain, port, timeoutSec)
	local ips = GetIPList(domain, timeoutSec)
	if #ips == 0 then
		return
	else
		-- 为每个 ip 创建一个 UvTcpClient 同时连. 优先返回最快连接成功的
		local rtv = { numFinished = 0, c = null }
		for i = 1, #ips do
			local ip = ips[i]
			local c = xx.UvTcpClient.Create()
			local r = nil
			if string.find(ip, ":") ~= nil then
				r = c:SetAddress6(ip, port)
			else
				r = c:SetAddress(ip, port)
			end
			if r ~= 0 then
				rtv.numFinished = rtv.numFinished + 1
			else
				c:OnConnect(function(status)
					rtv.numFinished = rtv.numFinished + 1
					if status == 0 then
						rtv.c = c
					end
				end)
				r = c:Connect(timeoutSec)
				if r ~= 0 then
					rtv.numFinished = rtv.numFinished + 1
				end
			end
		end
		while rtv.c == null and rtv.numFinished < #ips do
			yield()
		end
		if rtv.c == null then
			return nil 
		end
		return rtv.c
	end
end




-- 用于注册更新函数. 直接往里面放函数或移除. 每帧将逻辑无序遍历执行.
gUpdates = {}

-- 执行所有 update 函数
gUpdates_Exec = function()
	local t = gUpdates
	for k, f in pairs(t) do
		f()
	end
end




-- 公用网络层
gNet = xx.UvTcpClient.Create()

-- 推送的多播处理函数集 key: proto, val: { func(serial, pkg)... }
gNetHandlers = {}

-- 注册网络包处理函数
gNetHandlers_Register = function(pkgProto, key, func)
	local t = gNetHandlers[pkgProto]
	if t == nil then
		t = {}
		gNetHandlers[pkgProto] = t
	end
	if t[key] ~= nil then
		print("warning: gNetEvents_Register: pkgProto = "..pkgProto.typeName..", key = "..key.." exists.")
	else
		t[key] = func
	end
end

-- 反注册网络包处理函数
gNetHandlers_Unregister = function(pkgProto, key)
	local t = gNetHandlers[pkgProto]
	if t == nil then
		print("warning: gNetHandlers_Unregister: pkgProto = "..pkgProto.typeName.." can't find any handler!!")
	else
		if t[key] == nil then
			print("warning: gNetHandlers_Unregister: pkgProto = "..pkgProto.typeName..", key = "..key.." does not exists!!")
		else
			t[key] = nil
		end
	end
end


-- 网络解包并返回. 失败返回 nil
BBToObject = function(bb)
	if bb ~= nil then
		local success, pkg = pcall(function() return bb:ReadRoot() end)
		if success then
			return pkg
		else
			print(pkg)
		end
	end
end


-- 发送推送包
local bb = BBuffer.Create()
gNet_Send = function(pkg)
	bb:Clear()
	bb:WriteRoot(pkg)
	return gNet:Send(bb)
end

-- 发送请求包. 如果不传入 cb, 则会阻塞等待网络返回数据并 return. 如果传入 cb, 则会在超时或收到返回数据时触发执行 cb
-- 适合在 coro 环境使用( 如果不传入 cb )
gNet_SendRequest = function(pkg, cb)
	bb:Clear()
	bb:WriteRoot(pkg)
	if cb ~= nil then
		return gNet:SendRequest(bb, cb)
	else
		local yield = coroutine.yield
		local t = { [1] = null }
		gNet:SendRequest(bb, function(pkg)
			t[1] = pkg
		end)
		while t[1] == null do
			yield()
		end
		return BBToObject(t[1])
	end
end

-- 发送应答包
gNet_SendResponse = function(pkg, serial)
	bb:Clear()
	bb:WriteRoot(pkg)
	return gNet:SendResponse(bb, serial)
end

-- 设置默认的推送处理回调
gNet:OnReceivePackage(function(bb)
	local pkg = BBToObject(bb)
	if pkg == nil then
		return
	end
	local t = gNetHandlers[pkg.__proto]
	if t == nil then
		print("warning: gNet:OnReceivePackage: typeName = "..pkg.__proto.typeName..", typeId = "..pkg.__proto.typeId.." can't find any handler!!")
	else
		for _, f in pairs(t) do
			f(pkg)
		end
	end
end)



-- 全局帧数
gFrameNumber = 0

-- 注册每帧执行函数
local co = coroutine_create(function() while true do yield() end end)
cc.mainLoopCallback(function()
	-- 隐藏执行 uv.Run(Once)

	-- 执行 gUpdates 里的所有函数
	gUpdates_Exec()

	-- 执行 gCoros 里的所有协程
	local t = gCoros
	if #t > 0 then
		for i = #t, 1, -1 do
			local co = t[i]
			local ok, msg = resume(co)
			if not ok then
				print(msg)
			end
			if coroutine_status(co) == "dead" then
				t.SwapRemoveAt(i)
			end
		end
	end

	-- 递增全局帧编号
	gFrameNumber = gFrameNumber + 1
end)


----------------------------------------------------------------------
-- 用户代码区
----------------------------------------------------------------------




-- 初始化显示相关
cc.createSetOpenGLView("cocos_cpp_lua", 1280, 720)
cc.setDesignResolutionSize(1280, 720, cc.ResolutionPolicy.SHOW_ALL)
cc.setDisplayStats(true)
cc.setAnimationInterval(1 / 60)
gScene = cc.Scene.create()
cc.runWithScene(gScene)

-- 9 点定位相关变量初始化
--[[
789
456
123
]]
gX, gY, gW, gH = cc.getSafeAreaRect()
gWh = gW/2
gHh = gH/2
gX1 = gX
gY1 = gY
gX2 = gX + gWh
gY2 = gY
gX3 = gX + gW
gY3 = gY
gX4 = gX
gY4 = gY + gHh
gX5 = gX + gWh
gY5 = gY + gHh
gX6 = gX + gW
gY6 = gY + gHh
gX7 = gX
gY7 = gY + gH
gX8 = gX + gWh
gY8 = gY + gH
gX9 = gX + gW
gY9 = gY + gH


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
--[[
]]

go(function()
	local imgBody = cc.addImage("hi.png")
	local imgBullet = cc.addImage("btn.png")
	imgBody:retain()
	imgBullet:retain()

	local bullets = List_Create()
	Bullet_Create = function(x, y)
		local bullet = { x, y, #bullets, cc.Sprite.Create_FileName_Owner_Positon_Anchor_Scale("btn.png", gScene, x, y) }
		bullets.Add(bullet)
		bullet.Release = function()
			bullet[4]:removeFromParent()
			bullets.SwapRemoveAt(bullet[3])
		end
		return bullet
	end

	local x, y = gX5, gY5
	local o = cc.Sprite.Create_FileName_Owner_Positon_Anchor_Scale("hi.png", gScene, x, y)
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
		for i = 1, #keys do
			if gKeyCodes[keys[i]] then
				funcs[i]()
			end
		end
		o:setPosition(x, y)
		for i, t in pairs(gTouchs) do
			print(i, t:getLocation())
		end
		--[[
		]]
	end
end)



















 --[[
-- 加载全局设置 gSettings
require "app/Common/Def.lua"
require "app/Common/PKG_class.lua"
require "app/Common/GameInfoManager.lua"

sGameManager.Init()

go(require "logic.lua")
]]






--[[
-- 模拟加载一个状态. 3 秒后状态关闭. 再次打开它. 3 次后重启动程序
local panel = require "panel.lua"
panel.autoCloseDelayFrames = 180
go(function()
	for i = 1, 3 do
		yield()			-- 循环语句后习惯性放一条 yield() 防御式编程

		gStates_Open(panel)				-- 打开面板
		gStates_WaitDisappear(panel)	-- 等待面板消失( 这个面板会自动关闭 )
	end
	cc.restart()		-- 重启
end)






-- 加载网络包生成物
require "PKG_class.lua"

-- 测试网络层
go(function()

::LabCreateConn::
	yield()

	print("try connect.")
	local c = CreateUvTcpClient("www.baidu.com", 80, 2)
	if c == nil then
		--SleepSec(1)
		goto LabCreateConn
	end
	print("connected. SendRequest")
	local pkg = PKG_Client_Login_Auth.Create()
	pkg.pkgMD5 = PKG_PkgGenMd5_Value
	pkg.username = "abc"
	pkg.password = "123123"
	local r = SendRequest(c, pkg)
	if r == nil then
		print("waiting SendRequest timeout")
	else
		print("SendRequest recved: ")
		DumpPackage(r)
	end

	print("wait disconnect...")
	while c:GetState() == 2 do
		yield()
	end
	print("Disconnected.")
	goto LabCreateConn

end)



-- 测试网络层2
go(function()

::LabCreateConn::
	yield()

	print("try connect.")
	local c = CreateUvTcpClient("127.0.0.1", 10000, 2)
	if c == nil then
		--SleepSec(1)
		goto LabCreateConn
	end
	print("connected. SendRequest")
	local pkg = PKG_Client_Login_Auth.Create()
	pkg.pkgMD5 = PKG_PkgGenMd5_Value
	pkg.username = "abc"
	pkg.password = "123123"
	local r = SendRequest(c, pkg)
	if r == nil then
		print("waiting SendRequest timeout")
	else
		print("SendRequest recved: ")
		DumpPackage(r)
	end

	print("wait disconnect...")
	while c:GetState() == 2 do
		yield()
	end
	print("Disconnected.")
	goto LabCreateConn

end)

]]
