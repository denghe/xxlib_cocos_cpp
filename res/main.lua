local f = require "xxx"
f()

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
		if sprite:containsTouch(t) then
			print(t:getLocation())
		end
	end
end)
sprite:addEventListener(listener)


coroutine_create = coroutine.create
resume = coroutine.resume
yield = coroutine.yield

local function mainLoop()
	local yield = _G.yield
	local scene = cc.scene()
	sprite:setScale(1, 2)
	sprite:setAnchorPoint(0,0)
	while true do
		for i = 1, 420 do
			yield()
			sprite:setRotation(i)
			sprite:setPosition(i+200, i+200)
		end
		cc.restart()
	end
end
gMainLoopCoro = coroutine_create(mainLoop)
