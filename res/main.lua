require "libs.lua"

-- avoid memory leak
collectgarbage("setpause", 100)
collectgarbage("setstepmul", 5000)

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

-- 创建 scene, 将 safe area 中心设为 0,0 点
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
gX1 = -gW2
gX4 = -gW2
gX7 = -gW2
gX2 = 0
gX5 = 0
gX8 = 0
gX3 = gW2
gX6 = gW2
gX9 = gW2
gY1 = -gH2
gY2 = -gH2
gY3 = -gH2
gY4 = 0
gY5 = 0
gY6 = 0
gY7 = gH2
gY8 = gH2
gY9 = gH2



local frameNames =
{
	'interface_l.png',
	'interface_r.png',
	'reelpay.png',
	'reelshadow.png',
	'wheelpay.png',
	'window.png',
	'windowdark.png',
	'windowlamp.png',
}


local ThemeHuca = {}

ThemeHuca.BonusGameState =
{
    Idle = 0 ,
    Rolling = 1,
    Show = 2,
    Leaving = 3
}

ThemeHuca.BonusGameMode =
{
    Manual = 0,
    Auto = 1
}

ThemeHuca.BonusCellNum = 4

ThemeHuca.BonusWheelCellNum = 14

ThemeHuca.BonusGameImgPath = ''

ThemeHuca.Ratio = 640/768

ThemeHuca.BonusSymbolNormal = {
    ThemeHuca.BonusGameImgPath ..'reelsymbol1.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol2.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol3.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol4.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol5.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol6.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol7.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol8.png'
}

ThemeHuca.BonusSymbolBlur ={
    ThemeHuca.BonusGameImgPath ..'reelsymbol1_r.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol2_r.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol3_r.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol4_r.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol5_r.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol6_r.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol7_r.png',
    ThemeHuca.BonusGameImgPath ..'reelsymbol8_r.png'
}

cc.addSpriteFramesWithFile("bg_ui.plist")
cc.addSpriteFramesWithFile("bg_effect_0.plist")
cc.addSpriteFramesWithFile("bg_effect_1.plist")
cc.addSpriteFramesWithFile("bg_effect_2.plist")
cc.addSpriteFramesWithFile("vip_wheel_pack.plist")

--[[
go(function()
	local fn = frameNames[1]
	local title = cc.Label.createWithSystemFont(fn, "", 32)
	gScene:addChild(title)
	title:setPosition(gX8, gY8)
	title:setAnchorPoint(0.5, 1)
	local sprite = cc.Sprite.Create_SpriteFrameName_Owner_Positon_Anchor_Scale(fn, gScene, gX5, gY5)
	SleepSecs(3)
	for i = 2, #frameNames do
		fn = frameNames[i]
		title:setString(fn)
		sprite:setSpriteFrame(fn)
		SleepSecs(3)
	end
end)
]]



local this = {}
this._TAG = 'ThemeHuca.BonusGameLayer'
this._wheel = nil
this._wheelDark = nil
this._wheelLampList = nil
this._windowWheel = nil
this._spinCellList = nil

this._wheelAngleInterval = -1.2
this._wheelSpeedInit = 720.0
this._wheelAccelerateTime = 2
this._wheelSpeedAccelerate = 180
this._wheelAccelerateRound = 1
this._wheelAccelerate = 0
this._anticipationAngle = 0
this._anticipationSpeed = 75
this._wheelSlow = false
this._wheelSlower = false
this._wheelSpeed = 0
this._wheelIndex = 0

this._wheelLampRunInterval = 0.25
this._wheelLampRunIndex = 0
this._wheelLampRunTime = 0.0

this._wheelLampBlinkInterval = 0.4
this._wheelLampBlinkTime = 0
this._wheelLampEvenShow = true

this._wheelPayPosX = nil
this._wheelPayPosYList = nil
this._wheelPayWin = nil
this._reelPayWin = nil
this._reelWin = nil
this._symbolLineWin = nil
this._symbolLineDark = nil
this._symbolWindowWin = nil
this._windowWin = nil
this._windowWinOpacity = 200
this._reelPayWinPosXList = nil
this._symbolLineDarkPosXList = nil

this._bonusGameResult = nil
this._bonusTotalWin = nil
this._bonusGameIndex = 0
this._bonusSpinIndex = 0

this._reelResult = nil
this._scatterWin = 0
this._lineWin = 0

this._lastWheelIndex = -1
this._reelSpinEffectId = nil
this._distanceX = 55
this._distanceY = -2

this._bonusTextLabel = nil

this.speedUp = false
this.speedUpInReel = false

this.gameIndexLabel = nil
this.gameIndex = nil

this._wheelStopEffectFinished = false
this._readyToExit = false
this.state = ThemeHuca.BonusGameState.Idle
this.mode = ThemeHuca.BonusGameMode.Auto
    

local centerX = 0
local centerY = 0

this._wheelPayPosX = centerX +463
this._wheelPayPosYList = {centerY +219,centerY +166,centerY +113,centerY +60,centerY  +7,centerY -46,centerY -99,centerY -152}

this._reelPayWinPosXList = {centerX +51, centerX +146, centerX +241}
this._symbolLineDarkPosXList = {centerX -240,centerX -148.5,centerX -56,centerX +36}

local background = cc.Sprite.create(ThemeHuca.BonusGameImgPath .. 'bg_background.png')
background:setPosition(centerX, centerY)
background:setScale(gW/1136, gH/640)
background:setName('bg_background')
gScene:addChild(background, 0)

local root = cc.Node.create()
gScene:addChild(root)
root:setPosition(centerX - 100, centerY - 20)

local bgHole = cc.Sprite.create(ThemeHuca.BonusGameImgPath .. 'background_b.png')
bgHole:setPosition((centerX-101)*ThemeHuca.Ratio + this._distanceX, (centerY+65)*ThemeHuca.Ratio + this._distanceY)
root:addChild(bgHole, 0)

local interfaceL = cc.Sprite.createWithSpriteFrameName('interface_l.png')
interfaceL:setPosition((centerX-101)*ThemeHuca.Ratio + this._distanceX, (centerY+65) * ThemeHuca.Ratio + this._distanceY)
root:addChild(interfaceL, 1)

local interfaceR = cc.Sprite.createWithSpriteFrameName('interface_r.png')
interfaceR:setPosition((centerX+390) * ThemeHuca.Ratio + this._distanceX, (centerY+58) * ThemeHuca.Ratio + this._distanceY)
root:addChild(interfaceR, 1)

this.gameIndexLabel = cc.Sprite.create(ThemeHuca.BonusGameImgPath .. 'BONUS_TIMES.png')
this.gameIndexLabel:setPosition((centerX-101 + 450 + 5)*ThemeHuca.Ratio + this._distanceX, (centerY+65 + 290 + 20 + 15)*ThemeHuca.Ratio + this._distanceY)
this.gameIndex = cc.Label.createWithBMFont(ThemeHuca.BonusGameImgPath..'BONUS_TIMES_Font.fnt', '', 0)
this.gameIndex:setPosition((centerX-101 + 20 + 550 + 100 + 5 )*ThemeHuca.Ratio + this._distanceX, (centerY+65 + 298 + 20 + 15)*ThemeHuca.Ratio + this._distanceY)
root:addChild(this.gameIndexLabel, 1)
root:addChild(this.gameIndex, 1)

this._wheelLampList = {}
local lampPos = {{centerX - 459,centerY + 49},{ centerX - 422, centerY + 213},{ centerX - 330,  centerY + 338},{ centerX - 187, centerY + 410},{ centerX - 22, centerY + 413},{ centerX + 122, centerY + 346},{ centerX + 222 , centerY + 223},{ centerX + 256, centerY + 71},{ centerX + 225, centerY - 88},{ centerX + 133, centerY - 210},{ centerX - 11, centerY - 289},{ centerX - 172, centerY - 296},{ centerX - 319, centerY - 224},{ centerX - 421, centerY - 95}}
for i = 1, 14 do
    this._wheelLampList[i] = cc.Sprite.createWithSpriteFrameName('weelamp.png')
    this._wheelLampList[i]:setPosition(lampPos[i][1]*ThemeHuca.Ratio + this._distanceX, lampPos[i][2] * ThemeHuca.Ratio + this._distanceY)
    this._wheelLampList[i]:setVisible(false)
    root:addChild(this._wheelLampList[i], 1)
end
print('@debug bonuGameLayer complete1')
this._reelPayWin = cc.Sprite.createWithSpriteFrameName('reelpaywin.png')
this._reelPayWin:setBlendFunc(1, 1)
this._reelPayWin:setPosition(this._reelPayWinPosXList[1] * ThemeHuca.Ratio + this._distanceX , 494 *ThemeHuca.Ratio + this._distanceY)
this._reelPayWin:setVisible(false)
root:addChild(this._reelPayWin, 1)

local reelPay = cc.Sprite.createWithSpriteFrameName('reelpay.png')
reelPay:setPosition((centerX -102) * ThemeHuca.Ratio + this._distanceX , (centerY + 173) * ThemeHuca.Ratio + this._distanceY)
root:addChild(reelPay, 1)

local reelShadow = cc.Sprite.createWithSpriteFrameName('reelshadow.png')
reelShadow:setPosition( (centerX - 102)* ThemeHuca.Ratio + this._distanceX ,  (centerY + 64) * ThemeHuca.Ratio + this._distanceY)
root:addChild(reelShadow, 2)

local window = cc.Sprite.createWithSpriteFrameName('window.png')
window:setPosition((centerX + 213 )* ThemeHuca.Ratio + this._distanceX , (centerY + 65)* ThemeHuca.Ratio + this._distanceY)
root:addChild(window, 2)

local windowLamp = cc.Sprite.createWithSpriteFrameName('windowlamp.png')
windowLamp:setPosition((centerX + 218) * ThemeHuca.Ratio + this._distanceX , (centerY + 65)* ThemeHuca.Ratio + this._distanceY)
root:addChild(windowLamp, 2)

print('@debug bonuGameLayer complete2')
local wheelPayStencil = cc.Sprite.createWithSpriteFrameName('showarea-2.png')
wheelPayStencil:setPosition((centerX + 394)* ThemeHuca.Ratio + this._distanceX , (centerY + 49)* ThemeHuca.Ratio + this._distanceY)
local wheelPayClipper = cc.ClippingNode.create(wheelPayStencil)
wheelPayClipper:setAlphaThreshold(0.1)
this._wheelPayWin = cc.Sprite.createWithSpriteFrameName('wheelpaywin.png')
this._wheelPayWin:setBlendFunc(1, 1)
this._wheelPayWin:setPosition(this._wheelPayPosX * ThemeHuca.Ratio + this._distanceX , this._wheelPayPosYList[1] * ThemeHuca.Ratio + this._distanceY)
this._wheelPayWin:setVisible(false)
wheelPayClipper:addChild(this._wheelPayWin)
root:addChild(wheelPayClipper, 1)

local wheelPay = cc.Sprite.createWithSpriteFrameName('wheelpay.png')
wheelPay:setPosition((centerX + 421)* ThemeHuca.Ratio + this._distanceX, (centerY + 29)* ThemeHuca.Ratio + this._distanceY)
root:addChild(wheelPay, 1)
print('@debug bonuGameLayer complete3')

local wheelSymbolIndex = math.floor(math.random() * ThemeHuca.BonusWheelCellNum)
local initRotation = (-360/ThemeHuca.BonusWheelCellNum * wheelSymbolIndex + this._wheelAngleInterval + 360)%360
print('@debug rotation : ' .. initRotation)
this._wheel = cc.Sprite.createWithSpriteFrameName('wheel.png')
this._wheel:setPosition((centerX -101)* ThemeHuca.Ratio + this._distanceX, (centerY + 65)* ThemeHuca.Ratio + this._distanceY)
this._wheel:setRotation(initRotation)
root:addChild(this._wheel, 0)

this._wheelDark  = cc.Sprite.createWithSpriteFrameName('wheeldark.png')
this._wheelDark:setPosition((centerX -101)* ThemeHuca.Ratio + this._distanceX, (centerY + 65)* ThemeHuca.Ratio + this._distanceY)
this._wheelDark:setVisible(false)
root:addChild(this._wheelDark, 0)

local windowWheelStencil = cc.Sprite.createWithSpriteFrameName('showarea-1.png')
windowWheelStencil:setPosition((centerX + 212 )* ThemeHuca.Ratio + this._distanceX, (centerY + 65)* ThemeHuca.Ratio+ this._distanceY)
local windowWheelClipper = cc.ClippingNode.create(windowWheelStencil)
windowWheelClipper:setAlphaThreshold(0.1)
this._windowWheel = cc.Sprite.createWithSpriteFrameName('windowheel.png')
this._windowWheel:setPosition((centerX -263.5)* ThemeHuca.Ratio + this._distanceX , (centerY +  61)* ThemeHuca.Ratio +this._distanceY)
this._windowWheel:setRotation(initRotation)
windowWheelClipper:addChild(this._windowWheel)
this._windowDark = cc.Sprite.createWithSpriteFrameName('windowdark.png')
this._windowDark:setPosition((centerX + 212)* ThemeHuca.Ratio + this._distanceX, (centerY + 65)* ThemeHuca.Ratio + this._distanceY)
this._windowDark:setVisible(false)
windowWheelClipper:addChild(this._windowDark)
this._windowWin = cc.Sprite.createWithSpriteFrameName('windowin.png')
this._windowWin:setBlendFunc(1, 1)
this._windowWin:setPosition((centerX + 212)* ThemeHuca.Ratio + this._distanceX, (centerY + 65)* ThemeHuca.Ratio + this._distanceY)
this._windowWin:setOpacity(this._windowWinOpacity)
this._windowWin:setVisible(false)
windowWheelClipper:addChild(this._windowWin)
root:addChild(windowWheelClipper, 1)
print('@debug bonuGameLayer complete4')

this._spinCellList = {}
local cellXList = { (centerX -240)* ThemeHuca.Ratio, (centerX -148)* ThemeHuca.Ratio , (centerX -56)* ThemeHuca.Ratio, (centerX + 36)* ThemeHuca.Ratio}
local cellY = (centerY + 64) * ThemeHuca.Ratio
local reelSize = { 72, 140 * ThemeHuca.Ratio }
local cellSize = { 72, 76 }
for i = 1, ThemeHuca.BonusCellNum do
	local spr = cc.Sprite.create(ThemeHuca.BonusSymbolNormal[i])
    spr:setPosition(cellXList[i] + this._distanceX, cellY + this._distanceY)
    this._spinCellList[i] = spr
    root:addChild(this._spinCellList[i] , 1)
end
print('@debug bonuGameLayer complete4-1')

this._symbolLineDark = {}
for i = 1 , ThemeHuca.BonusCellNum do
    this._symbolLineDark[i] = cc.Sprite.createWithSpriteFrameName('reeldark.png')
    this._symbolLineDark[i]:setPosition(this._symbolLineDarkPosXList[i] * ThemeHuca.Ratio + this._distanceX , (centerY +  64) * ThemeHuca.Ratio + this._distanceY)
    this._symbolLineDark[i]:setVisible(false)
    root:addChild(this._symbolLineDark[i], 1)
end
print('@debug bonuGameLayer complete5')

this._reelWin = cc.Sprite.createWithSpriteFrameName('reelwin-a.png')
this._reelWin:setBlendFunc(1, 1)
this._reelWin:setPosition((centerX - 101)* ThemeHuca.Ratio + this._distanceX, (centerY + 64)* ThemeHuca.Ratio + this._distanceY)
this._reelWin:setVisible(false)
root:addChild(this._reelWin, 2)

local symbolLineWinPosX = {centerX -241, centerX -149, centerX -57, centerX+ 35}
this._symbolLineWin = {}
for i = 1, ThemeHuca.BonusCellNum do
    this._symbolLineWin[i] = cc.Sprite.createWithSpriteFrameName('reelwin-b.png')
    this._symbolLineWin[i]:setBlendFunc(1, 1)
    this._symbolLineWin[i]:setPosition(symbolLineWinPosX[i] * ThemeHuca.Ratio + this._distanceX, (centerY + 65) * ThemeHuca.Ratio + this._distanceY)
    this._symbolLineWin[i]:setVisible(false)
    root:addChild(this._symbolLineWin[i], 2)
end

this._symbolWindowWin = {}
for i =1 , ThemeHuca.BonusCellNum do
    this._symbolWindowWin[i] = cc.Sprite.createWithSpriteFrameName('reelwin-c.png')
    this._symbolWindowWin[i]:setBlendFunc(1, 1)
    this._symbolWindowWin[i]:setPosition(symbolLineWinPosX[i] * ThemeHuca.Ratio + this._distanceX, (centerY + 65)* ThemeHuca.Ratio + this._distanceY)
    this._symbolWindowWin[i]:setOpacity(this._windowWinOpacity)
    this._symbolWindowWin[i]:setVisible(false)
    root:addChild(this._symbolWindowWin[i], 2)
end
print("end")

go(function()
	while true do
		initRotation = initRotation - this._wheelAngleInterval
		this._wheel:setRotation(initRotation)
		this._windowWheel:setRotation(initRotation)
		yield()
	end
end)
