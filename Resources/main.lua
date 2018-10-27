local scene = cc.scene()
local texture = cc.TextureCache.addImage("hi.png")
local sprite = cc.Sprite.new()
sprite:initWithTexture(texture)
scene:addChild(sprite)
local listener = cc.EventListenerTouchAllAtOnce.new()
listener:onTouchesBegan(function(...)
	local args = {...}
	local e = args[1]
	for i = 2, #args do
		local t = args[i]
		print(t:getLocationInView())
		print(t:getLocation())
		--if sprite:containsTouchPoint(t) then
		--end
	end
end)
sprite:addEventListenerWithSceneGraphPriority(listener)


coroutine_create = coroutine.create
resume = coroutine.resume
yield = coroutine.yield

local function mainLoop()
	local yield = _G.yield
	local scene = cc.scene()
	while true do
		for i = 1, 120 do
			yield()
			-- sprite:removeFromParent()
			sprite:setPosition(i+200, i+200)
		end
		cc.restart()
	end
end
gMainLoopCoro = coroutine_create(mainLoop)
