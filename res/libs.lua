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
	local elapsedSecs = os.clock() + n
	local yield = yield
	while elapsedSecs > os.clock() do
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
-- 序列化相关
----------------------------------------------------------------------

-- 公用序列化容器
gBB = BBuffer.Create()

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

-- 方便输出包结构
function DumpPackage(t)  
    local cache = {}
    local function DumpSub(t,indent)
        if cache[tostring(t)] then
            print(indent.."*"..tostring(t))
        else
            cache[tostring(t)] = true
            if type(t)=="table" then
                for pos, val in pairs(t) do
					if pos == "__index"
					or pos == "__newindex"
					or pos == "__proto" then
						if pos == "__proto" then
							print(indent.."[__proto].typename = "..val.typeName)
						end
					else
						if type(val)=="table" then
							print(indent.."["..pos.."] => "..tostring(t).." {")
							DumpSub(val, indent..string.rep(" ", string.len(pos) + 8))
							print(indent..string.rep(" ",string.len(pos)+6).."}")
						elseif (type(val) == "string") then
							print(indent.."["..pos..'] => "'..val..'"')
						else
							print(indent.."["..pos.."] => "..tostring(val))
						end
					end
                end
            else
                print(indent..tostring(t))
            end
        end
    end
    if type(t) == "table" then
        print(tostring(t).." {")
        DumpSub(t, "  ")
        print("}")
    else
        DumpSub(t, "  ")
    end
    print()
end


----------------------------------------------------------------------
-- 网络相关
----------------------------------------------------------------------

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

