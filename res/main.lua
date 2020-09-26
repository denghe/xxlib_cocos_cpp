require "libs.lua"

-- avoid memory leak
collectgarbage("setpause", 100)
collectgarbage("setstepmul", 5000)

-- 创建显示窗口( for desktop os )
cc.createSetOpenGLView("cocos_cpp_lua", 800, 450)

-- 初始化设计尺寸( 如果不设置，iOS 上左下角统计会显示不出来 )
local sW, sH = cc.getFrameSize()
cc.setDesignResolutionSize( sW, sH, cc.ResolutionPolicy.NO_BORDER )

-- 打开 ogl 帧统计信息显示
cc.setDisplayStats(true)

-- 设置帧数
cc.setAnimationInterval(1 / 60)

-- 取可用区域坐标宽高
gX, gY, gW, gH = cc.getSafeAreaRect()

gW2 = gW/2
gH2 = gH/2

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


-- 创建 scene, 将 safe area 中心设为 0,0 点( 有可能导致部分 cocos 基础组件坐标或裁剪出问题 )
gScene = cc.Scene.create()
gScene:setContentSize(0, 0)
gScene:setPosition(gX + gW2, gY + gH2)
gScene:setIgnoreAnchorPointForPosition(true)

-- 加载并运行场景
cc.runWithScene(gScene)

-- 启动主携程
go(function()
-- 暂缓一帧 ( 否则 Director getRunningScene 取不到 )
yield()

print("asdf")

end)
