-- 全局协程池( 乱序 )
gUpdates = {}

-- 压入一个可以是协程的函数. 参数跟在后面. 延迟执行到其消亡
gUpdates_Push = function(func, ...)
	gUpdates[func] = coroutine.create(function()
		local co = coroutine.create(func)
		coroutine.resume(...)
		if coroutine.status(co) == "dead" then
			gUpdates[func] = nil
		else
			gUpdates[func] = co
		end
	end)
end

-- 执行里面的所有协程
gCoros_Exec = function()
	local cs = coroutine.status
	local cr = coroutine.resume
	local t = gCoros
	for k, co in pairs(t) do
		local ok, msg = cr(co)
		if not ok then
			print(msg)
		end
		if cs(co) == "dead" then
			t[ k ] = nil
		end
	end
end
