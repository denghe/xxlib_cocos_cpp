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
	-- 通常调用前还有一句: container[#container].idxAtContainer = idx
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


-- 公用网络层
-- todo: 用 CreateUvTcpClient 来创建
gNet = xx.UvTcpClient.Create()


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



PI180 = 3.14159265358979323846 / 180

GetAngle = function(x, y)
	return math.atan(y, x) / PI180
end

GetXyIncAngle = function(x, y)
	local a = math.atan(y, x)
	return math.cos(a), math.sin(a), a / PI180
end

Normalize = function(x, y)
    local n = x * x + y * y
    if n == 1 then return x, y end
    n = math.sqrt(n)
    if n < 2e-37 then return 0, 0 end
    n = 1 / n
    return x * n, y * n
end

GetRotatePos = function(x, y, angle)
	angle = -angle * PI180
	local sina = math.sin(angle)
	local cosa = math.cos(angle)
	return x * cosa - y * sina, x * sina + y * cosa
end




-- 全局帧数
gFrameNumber = 0

-- 注册每帧执行函数
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
