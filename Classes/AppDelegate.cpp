/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
void uuid_generate(unsigned char* buf)
{
	auto s = cocos2d::JniHelper::callStaticStringMethod(
		"org/cocos2dx/cpp_empty_tests/AppActivity", "GetUUID");
	// todo: s -> buf
}
#endif



#define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
#endif



#include "lua_wrapper.hpp"


void AppDelegate::Restart()
{
	if (!restarted)
	{
		if (L)
		{
			lua_close(L);
			L = nullptr;
		}
		uv.MPCreate(&mp);

		cocos2d::Director::getInstance()->restart();

		restarted = true;
	}
	else
	{
		// 创建 Scene 单例并运行
		scene = cocos2d::Scene::create();
		cocos2d::Director::getInstance()->runWithScene(scene);

		// 初始化 lua 部分
		int r = Lua_Init();
		assert(!r);

		restarted = false;
	}
}

AppDelegate::AppDelegate()
{
	instance = this;
}

AppDelegate::~AppDelegate()
{
	lua_close(L);
	L = nullptr;

#if USE_AUDIO_ENGINE
	cocos2d::experimental::AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
	CocosDenshion::SimpleAudioEngine::end();
#endif
}


// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
	// set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

	cocos2d::GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
	// initialize director
	auto director = cocos2d::Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = cocos2d::GLViewImpl::createWithRect(projectName, cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
		glview = cocos2d::GLViewImpl::create(projectName);
#endif
		director->setOpenGLView(glview);
	}
	// 设置为显示所有内容, 设备过宽或过长则留黑边
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);


	// turn on display FPS
	director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	// 可视区域尺寸
	visibleSize = director->getVisibleSize();

	// 原点坐标( 有些软按键设备原点就不是 0,0 )
	origin = director->getVisibleOrigin();

	// 初始化 uv loop
	uv.MPCreate(&mp);

	// 创建 Scene 单例并运行
	scene = cocos2d::Scene::create();
	cocos2d::Director::getInstance()->runWithScene(scene);

	// 初始化 lua 部分
	int r = Lua_Init();
	assert(!r);

	return true;
}




// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
	cocos2d::Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
	cocos2d::experimental::AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	cocos2d::Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
	cocos2d::experimental::AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
