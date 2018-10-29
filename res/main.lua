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
	xx.UvLoop.GetIPList(domain, timeoutSec * 1000, function(...)
		print("111")
		rt[1] = {...}
	end)
	while rt[1] == 1 do
		yield()
	end
	return rt[1];
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


-- 模拟 2 秒一次域名解析成 ip 列表
go(function()
	while true do
		local ips = gGetIPList("www.baidu.com", 2)
		if #ips == 0 then
			print("GetIPList fail or timeout")
		else
			for i = 1, #ips do
				print(ips[i])
			end
		end
		gSleepSec(2)
	end
end)

-- 模拟加载一个状态. 如果状态关闭了, 再次打开它. 3 次后重启动程序
local panel = require "panel.lua"
go(function()
	for i = 1, 3 do
		gStates_Open(panel)
		gStates_WaitDisappear(panel)
	end
	cc.restart()
end)
