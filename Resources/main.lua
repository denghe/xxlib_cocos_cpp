local scene = cc.scene()
local texture = cc.TextureCache.addImage("hi.png")
local sprite = cc.Sprite.new()
sprite:initWithTexture(texture)
scene:addChild(sprite)


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
			sprite:setPosition({[1]=i,[2]=i})
		end
		cc.restart()
	end
end
gMainLoopCoro = coroutine_create(mainLoop)


