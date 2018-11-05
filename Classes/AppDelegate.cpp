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
#pragma execution_character_set("utf-8")

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


#include "cocos/ui/CocosGUI.h"

#include "lua_all.hpp"


void InitGlobals(bool first)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	std::wstring str;
	str.resize(16384);
	decltype(auto) n = GetCurrentDirectory(16384, str.data());
	str.resize(wcslen(str.data()));
	str.append(L"/..");
	auto len = 2 * str.size() + 1;
	std::string s2;
	s2.resize(len);
	s2.resize(wcstombs(s2.data(), str.c_str(), len));
	cocos2d::FileUtils::getInstance()->setDefaultResourceRootPath(s2);
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::FileUtils::getInstance()->setSearchPaths({ "assets/" });
#else
	cocos2d::FileUtils::getInstance()->setSearchPaths({ "res/" });
#endif

	if (first)
	{
		mp = new xx::MemPool();
	}
	else
	{
		lua_close(gLua);
		uv->Release();
	}
	uv = mp->MPCreate<xx::UvLoop>();
	uv->InitRpcTimeoutManager();

	gScene = cocos2d::Scene::create();
	cocos2d::Director::getInstance()->runWithScene(gScene);

	int r = Lua_Init();
	assert(!r);
}

void ReleaseGlobals()
{
	lua_close(gLua);
	gLua = nullptr;

	uv->Release();
	uv = nullptr;

	delete mp;
	mp = nullptr;
}

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
	ReleaseGlobals();

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
	director = cocos2d::Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = cocos2d::GLViewImpl::createWithRect(projectName, cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
		glview = cocos2d::GLViewImpl::create(projectName);
#endif
		director->setOpenGLView(glview);
	}
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);

	// turn on display FPS
	director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	// 可视区域尺寸
	visibleSize = director->getVisibleSize();

	// 原点坐标( 有些软按键设备原点就不是 0,0 )
	origin = director->getVisibleOrigin();

	// 重启完成后重置运行时环境
	cocos2d::Director::getInstance()->restartCallback = [this]
	{
		InitGlobals(false);
	};

	// 初始化运行时环境
	InitGlobals();

	return true;
}




// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
	if (enterBackground) enterBackground();

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
	if (enterForeground) enterForeground();

	cocos2d::Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
	cocos2d::experimental::AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
