----------------------------------------------------------------------
-- 协程相关
----------------------------------------------------------------------

-- 常用函数减少查表次数
coroutine_create = coroutine.create
coroutine_status = coroutine.status
resume = coroutine.resume
yield = coroutine.yield
table_unpack = table.unpack

-- 简单的 List 结构, 主用于倒序访问并交换删除 if #t > 0 then for i = #t, 1, -1 do ...... end end
gListCreate = function()
	local t = {}
	t.Add = function(o)
		t[#t + 1] = o
	end
	-- 有可能调用前需要 t[#t].idxAtT = idx
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

-- 全局协程池
gCoros = gListCreate()

-- 压入一个协程函数. 有参数就跟在后面. 有延迟执行的效果
go = function(func, ...)
	--print("push")
	local args = {...}
	local co = nil
	if #args == 0 then
		co = coroutine_create(func)
	else
		co = coroutine_create(function() func(table_unpack(args)) end)
	end
	gCoros.Add(co)
	return co
end

-- 压入一个协程函数时立即执行一次
gorun = function(func, ...)
	local co = go(func, ...)
	resume(co)
	return co
end

-- 阻塞 n 帧
-- 适合在协程环境使用
gSleepFrames = function(n)
	local yield = yield
	for _=1, n do
		yield()
	end
end

-- 阻塞 n 秒
-- 适合在协程环境使用
gSleepSecs = function(n)
	local elapsedSecs = os.time() + n
	local yield = yield
	while elapsedSecs > os.time() do
		yield()
	end
end

-- 注册每帧执行函数
cc.mainLoopCallback(function()
	-- 隐藏执行 uv.Run(Once)

	-- 执行 gCoros 里的所有协程
	local t = gCoros
	if #t > 0 then
		for i = #t, 1, -1 do
			local co = t[i]
			local ok, msg = resume(co)
			--if not ok then
				--print(msg)
			--end
			if coroutine_status(co) == "dead" then
				t.SwapRemoveAt(i)
			end
		end
	end
end)


----------------------------------------------------------------------
-- 状态机相关
----------------------------------------------------------------------

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


----------------------------------------------------------------------
-- 坐标角度计算相关函数( 弧度 )
----------------------------------------------------------------------

gPI_180 = 3.14159265358979323846 / 180

gGetAngle = function(x1, y1, x2, y2)
	return math.atan(y2 - y1, x2 - x1)
end

gGetDistance = function(x1, y1, x2, y2)
	local dx = x1 - x2
	local dy = y1 - y2
	return math.sqrt(dx * dx + dy * dy)
end

gRotate = function(x, y, angle)
	local sina = math.sin(angle)
	local cosa = math.cos(angle)
	return x * cosa - y * sina, x * sina + y * cosa
end

gNormalize = function(x, y)
    local n = x * x + y * y
    if n == 1 then return x, y end
    n = math.sqrt(n)
    if n < 2e-37 then return 0, 0 end
    n = 1 / n
    return x * n, y * n
end


----------------------------------------------------------------------
-- 设计尺寸换算相关
----------------------------------------------------------------------

-- 按指定设计尺寸来计算缩放
gGetDesignScale = function(sW, sH, dW, dH)
	if dW / dH > sW / sH then
		return sW / dW
	else
		return sH / dH
	end
end


----------------------------------------------------------------------
-- 序列化相关
----------------------------------------------------------------------

-- 将 bb 转为 pkg 并返回 pkg. 失败返回 nil
gReadRoot = function(bb)
	if bb ~= nil then
		local success, pkg = pcall(function() return bb:ReadRoot() end)
		if success then
			return pkg
		--else
			--print(pkg)
		end
	end
end

-- 将 pkg 填充到 bb 并返回 bb
gWriteRoot = function(bb, pkg)
	bb:Clear()
	bb:WriteRoot(pkg)
	return bb
end

-- 公用序列化容器
gBB = BBuffer.Create()

----------------------------------------------------------------------
-- 网络相关
----------------------------------------------------------------------

-- 域名解析. 返回 { ip list }. 长度为 0 意味着解析失败或超时
-- 适合在协程环境使用
gResolveIPs = function(domain, timeoutMS)
	local rt = { 1 }
	local resolver = xx.UvResolver.Create()
	resolver:OnFinish(function()
		rt[1] = resolver:GetIPList()
	end)
	resolver:Resolve(domain, timeoutMS)
	local yield = yield
	while rt[1] == 1 do
		yield()
	end
	return rt[1];
end

-- 多 ip 拨号. 返回最先连接成功的 peer. 为空则连接超时
-- 适合在协程环境使用
gDial = function(ips, port, timeoutMS)
	local rt = { 1 }
	local dialer = xx.UvDialer.Create()
	dialer:OnAccept(function(peer)
		rt[1] = peer
	end)
	dialer:Dial(ips, port, timeoutMS)
	local yield = yield
	while rt[1] == 1 do
		yield()
	end
	return rt[1];
end

-- 发推送
gSendPush = function(peer, pkg)
	peer:SendPush(gWriteRoot(gBB, pkg))
end

-- 发请求
gSendRequest = function(peer, pkg, cb)
	return peer:SendRequest(gWriteRoot(gBB, pkg), function(bb)
		cb(gReadRoot(bb))
	end)
end

-- 发应答
gSendResponse = function(peer, pkg, serial)
	return peer:SendResponse(gWriteRoot(gBB, pkg), serial)
end





--[[

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
]]



--[[

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


]]
