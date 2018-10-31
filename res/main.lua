----------------------------------------------------------------------
-- 常用全局函数 & 变量
----------------------------------------------------------------------

-- 常用函数减少查表次数
coroutine_create = coroutine.create
coroutine_status = coroutine.status
resume = coroutine.resume
yield = coroutine.yield
table_unpack = table.unpack

-- 全局帧数
gFrameNumber = 0

-- 全局协程池( 乱序 )
gCoros = {}

-- 压入一个可以是协程的函数. 有参数就跟在后面. 有延迟执行的效果.
gCoros_Push = function(func, ...)
	local args = {...}
	if #args == 0 then
		gCoros[coroutine_create(func)] = 1
	else
		gCoros[coroutine_create(function() func(table_unpack(args)) end)] = 1
	end
end

-- for easy use
go = gCoros_Push

-- 阻塞 n 帧
-- 适合在协程环境使用
gSleep = function(n)
	for _=1, n do
		yield()
	end
end

-- 阻塞 n 秒( 按 60 帧粗算的 )
-- 适合在协程环境使用
gSleepSec = function(n)
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

-- 关状态, 关完后返回
-- 适合在协程环境使用
function gStates_Close(state)
	local s = gStates[state.stateGroupName]
	if s ~= nil then
		s.Close()
	end
	gStates[state.stateGroupName] = nil
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
gGetIPList = function(domain, timeoutSec)
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
function gCreateUvTcpClient(domain, port, timeoutSec)
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
function gCreateUvTcpClient2(domain, port, timeoutSec)
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
gBBToObject = function(bb)
	if bb ~= nil then
		local success, pkg = pcall(function() return bb:ReadRoot() end)
		if success then
			return pkg
		end
	end
end


-- 注册每帧执行函数
cc.mainLoopCallback(function()
	-- 隐藏执行 uv.Run(Once)

	-- 执行 gCoros 里的所有协程
	local t = gCoros
	for co, _ in pairs(t) do
		local ok, msg = resume(co)
		if not ok then
			print(msg)
		end
		if coroutine_status(co) == "dead" then
			t[ co ] = nil
		end
	end

	-- 递增全局帧编号
	gFrameNumber = gFrameNumber + 1
end)

----------------------------------------------------------------------
-- 用户代码区
----------------------------------------------------------------------

-- todo: List, 用 List 来改进上面的代码



-- 测试网络层
go(function()
::LabCreateConn::

	local c = gCreateUvTcpClient("www.baidu.com", 80, 2)
	if c == nil then
		goto LabCreateConn
	end
	
	print(c:GetState())

	--print(b)

	-- todo: set c event handler
	-- todo:Send?

	--goto LabCreateConn
end)


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
