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


-- 全局帧数
gFrameNumber = 0

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
SleepSec = function(n)
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


-- 指向当前场景
gScene = cc.scene()


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

-- 上面函数的 lua 实现
function CreateUvTcpClient2(domain, port, timeoutSec)
	local ips = gGetIPList(domain, timeoutSec)
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
		return rtv.c
	end
end



-- 网络解包并返回. 失败返回 nil
BBToObject = function(bb)
	if bb ~= nil then
		local success, pkg = pcall(function() return bb:ReadRoot() end)
		if success then
			return pkg
		end
	end
end


local bb = BBuffer.Create()
-- 发送请求. 直接返回结果. 返回空为超时或断开
-- 适合在协程环境使用
SendRequest = function(c, p)
	local null = null
	bb:Clear()
	bb:WriteRoot(p)
	local t = { null }
	c:SendRequest(bb, function(bb)
		t[1] = BBToObject(bb)
	end, 5)
	while t[1] == null do
		yield()
	end
	return t[1]
end


local co = coroutine_create(function() while true do yield() end end)
-- 注册每帧执行函数
cc.mainLoopCallback(function()
	-- 隐藏执行 uv.Run(Once)

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


-- 模拟加载一个状态. 3 秒后状态关闭. 再次打开它. 3 次后重启动程序
local panel = require "panel.lua"
panel.autoCloseDelayFrames = 180
go(function()
	for i = 1, 3 do
		yield()
		gStates_Open(panel)
		gStates_WaitDisappear(panel)
	end
	cc.restart()
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
		SleepSec(1)
		goto LabCreateConn
	end
	print("connected. SendRequest")
	local pkg = PKG_Client_Login_Auth.Create()
	pkg.pkgMD5 = "md5"
	pkg.username = u
	pkg.password = p
	local r = SendRequest(c, pkg)
	if r == nil then
		print("waiting SendRequest timeout")
	else
		print("SendRequest recved data = ")
		print(r)
	end

	while c:GetState() == 2 do
		yield()
	end
	print("Disconnected.")
	goto LabCreateConn
end)
