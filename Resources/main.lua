local function mainLoop()
	local yield = coroutine.yield
	while true do
		print(1)
		yield()
		print(2)
		yield()
		print(3)
		yield()
		print(3)
		yield()
		print(3)
		yield()
		print(3)
		yield()
		print(3)
		yield()
		print(3)
		yield()
		print(3)
		yield()
		print(3)
		yield()
		print(3)
		yield()
		print(3)
		print(cc)
		print(cc.restart)
		cc.restart()
	end
end

coroutine_create = coroutine.create
coroutine_resume = coroutine.resume

gMainLoopCoro = coroutine_create(mainLoop)
