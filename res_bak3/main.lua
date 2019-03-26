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


local this = {}

this.BonusGameState =
{
    Idle = 0 ,
    Rolling = 1,
    Show = 2,
    Leaving = 3
}

this.BonusGameMode =
{
    Manual = 0,
    Auto = 1
}

this.BonusCellNum = 4

this.BonusWheelCellNum = 14

this.BonusGameImgPath = ''

this.Ratio = 640/768

this.BonusSymbolNormal = {
    this.BonusGameImgPath ..'reelsymbol1.png',
    this.BonusGameImgPath ..'reelsymbol2.png',
    this.BonusGameImgPath ..'reelsymbol3.png',
    this.BonusGameImgPath ..'reelsymbol4.png',
    this.BonusGameImgPath ..'reelsymbol5.png',
    this.BonusGameImgPath ..'reelsymbol6.png',
    this.BonusGameImgPath ..'reelsymbol7.png',
    this.BonusGameImgPath ..'reelsymbol8.png'
}

this.BonusSymbolBlur ={
    this.BonusGameImgPath ..'reelsymbol1_r.png',
    this.BonusGameImgPath ..'reelsymbol2_r.png',
    this.BonusGameImgPath ..'reelsymbol3_r.png',
    this.BonusGameImgPath ..'reelsymbol4_r.png',
    this.BonusGameImgPath ..'reelsymbol5_r.png',
    this.BonusGameImgPath ..'reelsymbol6_r.png',
    this.BonusGameImgPath ..'reelsymbol7_r.png',
    this.BonusGameImgPath ..'reelsymbol8_r.png'
}


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
this.state = this.BonusGameState.Idle
this.mode = this.BonusGameMode.Auto
    

this._wheelPayPosX = 463
this._wheelPayPosYList = {219, 166, 113, 60, 7, -46, -99, -152}

this._reelPayWinPosXList = {51, 146, 241}
this._symbolLineDarkPosXList = {-240, -148.5, -56, 36}

local background = cc.Sprite.create(this.BonusGameImgPath .. 'bg_background.png')
background:setScale(gW/1136, gH/640)
background:setName('bg_background')
gScene:addChild(background, 0)

local root = cc.Node.create()
gScene:addChild(root)
root:setPosition(-100, -50)	-- 令图像大体上放置在屏的正中间

local bgHole = cc.Sprite.create(this.BonusGameImgPath .. 'background_b.png')
bgHole:setPosition(-101 * this.Ratio + this._distanceX, 65 * this.Ratio + this._distanceY)
root:addChild(bgHole, 0)

local interfaceL = cc.Sprite.createWithSpriteFrameName('interface_l.png')
interfaceL:setPosition(-101 * this.Ratio + this._distanceX, 65 * this.Ratio + this._distanceY)
root:addChild(interfaceL, 1)

local interfaceR = cc.Sprite.createWithSpriteFrameName('interface_r.png')
interfaceR:setPosition(390 * this.Ratio + this._distanceX, 58 * this.Ratio + this._distanceY)
root:addChild(interfaceR, 1)

this.gameIndexLabel = cc.Sprite.create(this.BonusGameImgPath .. 'BONUS_TIMES.png')
this.gameIndexLabel:setPosition((-101 + 450 + 5) * this.Ratio + this._distanceX, (65 + 290 + 20 + 15) * this.Ratio + this._distanceY)
this.gameIndex = cc.Label.createWithBMFont(this.BonusGameImgPath..'BONUS_TIMES_Font.fnt', '', 0)
this.gameIndex:setPosition((-101 + 20 + 550 + 100 + 5) * this.Ratio + this._distanceX, (65 + 298 + 20 + 15) * this.Ratio + this._distanceY)
root:addChild(this.gameIndexLabel, 1)
root:addChild(this.gameIndex, 1)

this._wheelLampList = {}
local lampPos = {{-459, 49},{-422, 213},{-330,  338},{-187, 410},{-22, 413},{122, 346},{222, 223},{256, 71},{225, 88},{133, 210},{-11, 289},{-172, 296},{-319, 224},{-421, 95}}
for i = 1, 14 do
    this._wheelLampList[i] = cc.Sprite.createWithSpriteFrameName('weelamp.png')
    this._wheelLampList[i]:setPosition(lampPos[i][1] * this.Ratio + this._distanceX, lampPos[i][2] * this.Ratio + this._distanceY)
    this._wheelLampList[i]:setVisible(false)
    root:addChild(this._wheelLampList[i], 1)
end

this._reelPayWin = cc.Sprite.createWithSpriteFrameName('reelpaywin.png')
this._reelPayWin:setBlendFunc(1, 1)
this._reelPayWin:setPosition(this._reelPayWinPosXList[1] * this.Ratio + this._distanceX , 494 * this.Ratio + this._distanceY)
this._reelPayWin:setVisible(false)
root:addChild(this._reelPayWin, 1)

local reelPay = cc.Sprite.createWithSpriteFrameName('reelpay.png')
reelPay:setPosition(-102 * this.Ratio + this._distanceX , 173 * this.Ratio + this._distanceY)
root:addChild(reelPay, 1)

local reelShadow = cc.Sprite.createWithSpriteFrameName('reelshadow.png')
reelShadow:setPosition(-102 * this.Ratio + this._distanceX ,  64 * this.Ratio + this._distanceY)
root:addChild(reelShadow, 2)

local window = cc.Sprite.createWithSpriteFrameName('window.png')
window:setPosition(213 * this.Ratio + this._distanceX , 65 * this.Ratio + this._distanceY)
root:addChild(window, 2)

local windowLamp = cc.Sprite.createWithSpriteFrameName('windowlamp.png')
windowLamp:setPosition(218 * this.Ratio + this._distanceX , 65 * this.Ratio + this._distanceY)
root:addChild(windowLamp, 2)

local wheelPayStencil = cc.Sprite.createWithSpriteFrameName('showarea-2.png')
wheelPayStencil:setPosition(394 * this.Ratio + this._distanceX , 49 * this.Ratio + this._distanceY)
local wheelPayClipper = cc.ClippingNode.create(wheelPayStencil)
wheelPayClipper:setAlphaThreshold(0.1)
this._wheelPayWin = cc.Sprite.createWithSpriteFrameName('wheelpaywin.png')
this._wheelPayWin:setBlendFunc(1, 1)
this._wheelPayWin:setPosition(this._wheelPayPosX * this.Ratio + this._distanceX , this._wheelPayPosYList[1] * this.Ratio + this._distanceY)
this._wheelPayWin:setVisible(false)
wheelPayClipper:addChild(this._wheelPayWin)
root:addChild(wheelPayClipper, 1)

local wheelPay = cc.Sprite.createWithSpriteFrameName('wheelpay.png')
wheelPay:setPosition(421 * this.Ratio + this._distanceX, 29 * this.Ratio + this._distanceY)
root:addChild(wheelPay, 1)

local wheelSymbolIndex = math.floor(math.random() * this.BonusWheelCellNum)
local initRotation = (-360 / this.BonusWheelCellNum * wheelSymbolIndex + this._wheelAngleInterval + 360) % 360
this._wheel = cc.Sprite.createWithSpriteFrameName('wheel.png')
this._wheel:setPosition(-101 * this.Ratio + this._distanceX, 65 * this.Ratio + this._distanceY)
this._wheel:setRotation(initRotation)
root:addChild(this._wheel, 0)

this._wheelDark  = cc.Sprite.createWithSpriteFrameName('wheeldark.png')
this._wheelDark:setPosition(-101 * this.Ratio + this._distanceX, 65 * this.Ratio + this._distanceY)
this._wheelDark:setVisible(false)
root:addChild(this._wheelDark, 0)

local windowWheelStencil = cc.Sprite.createWithSpriteFrameName('showarea-1.png')
windowWheelStencil:setPosition(212 * this.Ratio + this._distanceX, 65 * this.Ratio+ this._distanceY)
local windowWheelClipper = cc.ClippingNode.create(windowWheelStencil)
windowWheelClipper:setAlphaThreshold(0.1)
this._windowWheel = cc.Sprite.createWithSpriteFrameName('windowheel.png')
this._windowWheel:setPosition(-263.5 * this.Ratio + this._distanceX , 61 * this.Ratio +this._distanceY)
this._windowWheel:setRotation(initRotation)
windowWheelClipper:addChild(this._windowWheel)
this._windowDark = cc.Sprite.createWithSpriteFrameName('windowdark.png')
this._windowDark:setPosition(212 * this.Ratio + this._distanceX, 65 * this.Ratio + this._distanceY)
this._windowDark:setVisible(false)
windowWheelClipper:addChild(this._windowDark)
this._windowWin = cc.Sprite.createWithSpriteFrameName('windowin.png')
this._windowWin:setBlendFunc(1, 1)
this._windowWin:setPosition(212 * this.Ratio + this._distanceX, 65 * this.Ratio + this._distanceY)
this._windowWin:setOpacity(this._windowWinOpacity)
this._windowWin:setVisible(false)
windowWheelClipper:addChild(this._windowWin)
root:addChild(windowWheelClipper, 1)

this._spinCellList = {}
local cellXList = {-240 * this.Ratio, -148 * this.Ratio , -56 * this.Ratio, 36 * this.Ratio}
local cellY = 64 * this.Ratio
local reelSize = { 72, 140 * this.Ratio }
local cellSize = { 72, 76 }
for i = 1, this.BonusCellNum do
	local spr = cc.Sprite.create(this.BonusSymbolNormal[i])
    spr:setPosition(cellXList[i] + this._distanceX, cellY + this._distanceY)
    this._spinCellList[i] = spr
    root:addChild(this._spinCellList[i] , 1)
end

this._symbolLineDark = {}
for i = 1, this.BonusCellNum do
    this._symbolLineDark[i] = cc.Sprite.createWithSpriteFrameName('reeldark.png')
    this._symbolLineDark[i]:setPosition(this._symbolLineDarkPosXList[i] * this.Ratio + this._distanceX , 64 * this.Ratio + this._distanceY)
    this._symbolLineDark[i]:setVisible(false)
    root:addChild(this._symbolLineDark[i], 1)
end

this._reelWin = cc.Sprite.createWithSpriteFrameName('reelwin-a.png')
this._reelWin:setBlendFunc(1, 1)
this._reelWin:setPosition(-101 * this.Ratio + this._distanceX, 64 * this.Ratio + this._distanceY)
this._reelWin:setVisible(false)
root:addChild(this._reelWin, 2)

local symbolLineWinPosX = {-241, -149, -57, 35}
this._symbolLineWin = {}
for i = 1, this.BonusCellNum do
    this._symbolLineWin[i] = cc.Sprite.createWithSpriteFrameName('reelwin-b.png')
    this._symbolLineWin[i]:setBlendFunc(1, 1)
    this._symbolLineWin[i]:setPosition(symbolLineWinPosX[i] * this.Ratio + this._distanceX, 65 * this.Ratio + this._distanceY)
    this._symbolLineWin[i]:setVisible(false)
    root:addChild(this._symbolLineWin[i], 2)
end

this._symbolWindowWin = {}
for i = 1, this.BonusCellNum do
    this._symbolWindowWin[i] = cc.Sprite.createWithSpriteFrameName('reelwin-c.png')
    this._symbolWindowWin[i]:setBlendFunc(1, 1)
    this._symbolWindowWin[i]:setPosition(symbolLineWinPosX[i] * this.Ratio + this._distanceX, 65 * this.Ratio + this._distanceY)
    this._symbolWindowWin[i]:setOpacity(this._windowWinOpacity)
    this._symbolWindowWin[i]:setVisible(false)
    root:addChild(this._symbolWindowWin[i], 2)
end

--[[
观察到的：

symbols = { diamond, seven, bar, ring, watermelon, grape, lemon, cherry }
数组下标即 id. Exit 用 0 代表

有 5 列, 前 4 列排列顺序似乎随机，保险起见，需要参考视频来抄顺序
第五列为固定序列大环，游戏起始旋转位置为“钻石”. 而 0 度对应的显示是 Exit

一圈 14 个图案，0 度情况下, 从最右边 Exit 开始算, 顺时针 id 依次为 0, 4, 8, 7, 2, 6, 4, 0, 3, 6, 8, 1, 7, 5

刚开始转的时候, 每帧约转 0.45 个符号, 维持高速度 5.8 圈, 期间“停止”按钮可以点击( 猜测点击后可以立即进入到减速阶段 ), 中间 4 个轴在 1.4, 2.3, 2.75, 3.75 圈处依次停下
转速最慢的时候, 每帧约转 1/18 个符号
5.8 圈后, 速度似乎迅速跌落到每帧约转 1/3 个符号，维持一段时间后, 跌落到 1/4 符号, 并每经历一个符号, 分母 + 1, 直到 1/18 个符号, 直到结果符号
理论上讲, 从 1/3 的减速期开始到停, 这一段的总移动角度应该是恒定不变的. 
要控制最终落在哪个图像上, 在中间 4 轴停止后的 4 ~ 5.8 圈时, 控制其旋转时长, 不一定转满 5.8 圈. 判断当前位置 + 减速期总移动角度 ~= 目标输出即可切换到减速期


]]
go(function()
	local a = 360 / 14		-- 每个符号的角度步进值
	local r = a * -4		-- 钻石的起始角度

	function Rotate(ia, n)
		for i = 1, n do
			r = r + ia
			this._wheel:setRotation(r)
			this._windowWheel:setRotation(r)
			yield()
		end
	end

	Rotate(0, 1)
	Rotate(a * 0.45, 360 / (a * 0.45) * 5)
	Rotate(a / 3, 30)
	Rotate(a / 4, 15)
	for b = 5, 18 do
		Rotate(a / b, 19 - b)
	end
	Rotate(a / 19, 60)
end)
