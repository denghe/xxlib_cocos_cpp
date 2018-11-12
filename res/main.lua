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

-- 创建 scene, 将可用区域中心设为 0,0 点
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
gX1 = -gW
gY1 = -gH
gX2 = 0
gY2 = -gH
gX3 = gW
gY3 = -gH
gX4 = -gX
gY4 = 0
gX5 = 0
gY5 = 0
gX6 = gW
gY6 = 0
gX7 = -gW
gY7 = gH
gX8 = 0
gY8 = gH
gX9 = gW
gY9 = gH


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

-- 测试点击

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


local PI_div_180 = 3.14159265358979323846 / 180

local GetAngle = function(x, y)
	return math.atan(y, x) / PI_div_180
end

local GetXyIncAngle = function(x, y)
	local a = math.atan(y, x)
	return math.cos(a), math.sin(a), a / PI_div_180
end

local Normalize = function(x, y)
    local n = x * x + y * y
    if n == 1 then return x, y end
    n = math.sqrt(n)
    if n < 2e-37 then return 0, 0 end
    n = 1 / n
    return x * n, y * n
end

local GetRotatePos = function(x, y, angle)
	angle = -angle * PI_div_180
	local sina = math.sin(angle)
	local cosa = math.cos(angle)
	return x * cosa - y * sina, x * sina + y * cosa
end


-- 绘制一个 ASDW 控制的 sprite. touch 发生时向其坐标发射 bullet
go(function()
	local imgBody = cc.addImage("man.png")
	local imgBullet = cc.addImage("bullet.png")
	imgBody:retain()
	imgBullet:retain()

	local bullets = List_Create()

	bullets.Create = function(x, y, incX, incY, a)
		local bullet = 
		{
			0,
			cc.Sprite.Create_FileName_Owner_Positon_Anchor_Scale("bullet.png", gScene, x, y), 
			x, y, 
			incX, incY
		}
		bullets.Add(bullet)
		bullet[1] = #bullets
		bullet[2]:setRotation(-a)
		bullet.Release = function()
			bullet[2]:removeFromParent()
			local idx = bullet[1]
			bullets[#bullets][1] = idx
			bullets.SwapRemoveAt(idx)
		end
		bullet.Move = function()
			local x, y = bullet[3] + bullet[5], bullet[4] + bullet[6]
			if x < -740 or x > 740 or y < -460 or y > 460 then
				bullet.Release()
			else
				bullet[3], bullet[4] = x, y
				bullet[2]:setPosition(x, y)
			end
		end
		return bullet
	end

	local x, y = 0, 0
	local o = cc.Sprite.Create_FileName_Owner_Positon_Anchor_Scale("man.png", gScene, x, y, 0.5, 0.5)
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

		-- move man
		for i = 1, #keys do
			if gKeyCodes[keys[i]] then
				funcs[i]()
			end
		end
		o:setPosition(x, y)

		-- fire
		for i, t in pairs(gTouchs) do
			local tx, ty = o:convertTouchToNodeSpaceAR(t)
			local bx, by = 145, 0
			if tx < 0 then
				bx = -145
			end
			local a = 0
			tx, ty, a = GetXyIncAngle(tx - bx, ty - by)
			bullets.Create(x + bx, y + by, tx, ty, a)
		end

		-- move bullets
		for i = #bullets, 1, -1 do
			bullets[i].Move()
		end
	end
end)
