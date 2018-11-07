#pragma once

inline void Lua_Register_cc(lua_State* const& L)
{
	// 创建全局 cc 基表
	lua_createtable(L, 0, 100);										// cc
	lua_pushvalue(L, -1);											// cc, cc
	lua_setglobal(L, LuaKey_cc);									// cc


	/**************************************************************************************************/
	// Director
	/**************************************************************************************************/


	// 创建 帧循环事件设置 函数
	Lua_NewFunc(L, "mainLoopCallback", [](lua_State* L)
	{
		var t = Lua_ToTuple<Lua_Func>(L, "mainLoopCallback error! need 1 args: func/null");
		if (std::get<0>(t))
		{
			cocos2d::Director::getInstance()->mainLoopCallback = [f = std::move(std::get<0>(t))]
			{
				uv->Run(xx::UvRunMode::NoWait);		// 这个需要一直在的
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f);
				lua_settop(gLua, 0);
			};
		}
		else
		{
			cocos2d::Director::getInstance()->mainLoopCallback = []
			{
				uv->Run(xx::UvRunMode::NoWait);
			};
		}
		return 0;
	});

	// 创建 程序被切到后台事件设置 函数
	Lua_NewFunc(L, "enterBackground", [](lua_State* L)
	{
		var t = Lua_ToTuple<Lua_Func>(L, "enterBackground error! need 1 args: func/null");
		if (std::get<0>(t))
		{
			enterBackground = [f = std::move(std::get<0>(t))]
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f);
				lua_settop(gLua, 0);
			};
		}
		else
		{
			enterBackground = nullptr;
		}
		return 0;
	});

	// 创建 程序被切到前台事件设置 函数
	Lua_NewFunc(L, "enterForeground", [](lua_State* L)
	{
		var t = Lua_ToTuple<Lua_Func>(L, "enterForeground error! need 1 args: func/null");
		if (std::get<0>(t))
		{
			enterForeground = [f = std::move(std::get<0>(t))]
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f);
				lua_settop(gLua, 0);
			};
		}
		else
		{
			enterForeground = nullptr;
		}
		return 0;
	});


	// 创建 重启游戏 函数
	Lua_NewFunc(L, "restart", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->restart();
		cocos2d::Director::getInstance()->mainLoopCallback = nullptr;
		return 0;
	});

	Lua_NewFunc(L, "getRunningScene", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getRunningScene();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getAnimationInterval", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getAnimationInterval();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setAnimationInterval", [](lua_State* L)
	{
		var t = Lua_ToTuple<float>(L, "setAnimationInterval error! need 1 args: float interval. FPS = 1/interval");
		cocos2d::Director::getInstance()->setAnimationInterval(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "isDisplayStats", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->isDisplayStats();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setDisplayStats", [](lua_State* L)
	{
		var t = Lua_ToTuple<bool>(L, "setDisplayStats error! need 1 args: bool displayStats");
		cocos2d::Director::getInstance()->setDisplayStats(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "getSecondsPerFrame", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getSecondsPerFrame();
		return Lua_Pushs(L, r);
	});

	// 暂不实现 getOpenGLView setOpenGLView. 直接在这封一些常用函数

	Lua_NewFunc(L, "createSetOpenGLView", [](lua_State* L)
	{
		var glview = cocos2d::Director::getInstance()->getOpenGLView();
		if (!glview)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
			var t = Lua_ToTuple<std::string, float, float>(L, "createGLView error! need 3 args: string projectName, float width, height");
			glview = cocos2d::GLViewImpl::createWithRect(std::get<0>(t), cocos2d::Rect(0, 0, std::get<1>(t), std::get<2>(t)));
#else
			var t = Lua_ToTuple<std::string>(L, "createGLView error! need 1 args: string projectName");
			glview = cocos2d::GLViewImpl::create(std::get<0>(t));
#endif
			cocos2d::Director::getInstance()->setOpenGLView(glview);
		}
		return 0;
		});

	Lua_NewFunc(L, "setDesignResolutionSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, ResolutionPolicy>(L, "setDesignResolutionSize error! need 3 args: float width, height, ResolutionPolicy resolutionPolicy");
		cocos2d::Director::getInstance()->getOpenGLView()->setDesignResolutionSize(std::get<0>(t), std::get<1>(t), std::get<2>(t));
		return 0;
	});

	lua_pushstring(L, TypeNames<ResolutionPolicy>::value);
	lua_createtable(L, 11, 0);
	lua_pushstring(L, "EXACT_FIT");	lua_pushinteger(L, (int)ResolutionPolicy::EXACT_FIT);	lua_rawset(L, -3);
	lua_pushstring(L, "NO_BORDER");	lua_pushinteger(L, (int)ResolutionPolicy::NO_BORDER);	lua_rawset(L, -3);
	lua_pushstring(L, "SHOW_ALL");	lua_pushinteger(L, (int)ResolutionPolicy::SHOW_ALL);	lua_rawset(L, -3);
	lua_pushstring(L, "FIXED_HEIGHT");	lua_pushinteger(L, (int)ResolutionPolicy::FIXED_HEIGHT);	lua_rawset(L, -3);
	lua_pushstring(L, "FIXED_WIDTH");	lua_pushinteger(L, (int)ResolutionPolicy::FIXED_WIDTH);	lua_rawset(L, -3);
	lua_pushstring(L, "UNKNOWN");	lua_pushinteger(L, (int)ResolutionPolicy::UNKNOWN);	lua_rawset(L, -3);
	lua_rawset(L, -3);


	Lua_NewFunc(L, "isNextDeltaTimeZero", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->isNextDeltaTimeZero();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setNextDeltaTimeZero", [](lua_State* L)
	{
		var t = Lua_ToTuple<float>(L, "setNextDeltaTimeZero error! need 1 args: bool nextDeltaTimeZero");
		cocos2d::Director::getInstance()->setNextDeltaTimeZero(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "isPaused", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->isPaused();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getTotalFrames", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getTotalFrames();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getProjection", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getProjection();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setProjection", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Director::Projection>(L, "setProjection error! need 1 args: Projection projection");
		cocos2d::Director::getInstance()->setProjection(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "setViewport", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->setViewport();
		return 0;
	});

	Lua_NewFunc(L, "isSendCleanupToScene", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->isSendCleanupToScene();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getNotificationNode", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getNotificationNode();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setNotificationNode", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "setNotificationNode error! need 1 args: Node node");
		cocos2d::Director::getInstance()->setNotificationNode(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "getWinSize", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getWinSize();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "getWinSizeInPixels", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getWinSizeInPixels();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "getVisibleSize", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getVisibleSize();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "getVisibleOrigin", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getVisibleOrigin();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getSafeAreaRect", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getSafeAreaRect();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "convertToGL", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float>(L, "convertToGL error! need 2 args: float pointX, pointY");
		var r = cocos2d::Director::getInstance()->convertToGL({ std::get<0>(t), std::get<1>(t) });
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "convertToUI", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float>(L, "convertToUI error! need 2 args: float pointX, pointY");
		var r = cocos2d::Director::getInstance()->convertToUI({ std::get<0>(t), std::get<1>(t) });
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getZEye", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getZEye();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "runWithScene", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Scene*>(L, "runWithScene error! need 1 args: Scene scene");
		cocos2d::Director::getInstance()->runWithScene(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "pushScene", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Scene*>(L, "pushScene error! need 1 args: Scene scene");
		cocos2d::Director::getInstance()->pushScene(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "popScene", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->popScene();
		return 0;
	});

	Lua_NewFunc(L, "popToRootScene", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->popToRootScene();
		return 0;
	});

	Lua_NewFunc(L, "popToSceneStackLevel", [](lua_State* L)
	{
		var t = Lua_ToTuple<int>(L, "popToSceneStackLevel error! need 1 args: int level");
		cocos2d::Director::getInstance()->popToSceneStackLevel(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "replaceScene", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Scene*>(L, "replaceScene error! need 1 args: Scene scene");
		cocos2d::Director::getInstance()->replaceScene(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "end", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->end();
		return 0;
	});

	Lua_NewFunc(L, "pause", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->pause();
		return 0;
	});

	Lua_NewFunc(L, "resume", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->resume();
		return 0;
	});

	// restart 的实现在上面

	Lua_NewFunc(L, "stopAnimation", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->stopAnimation();
		return 0;
	});

	Lua_NewFunc(L, "startAnimation", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->startAnimation();
		return 0;
	});

	// 不实现 drawScene

	Lua_NewFunc(L, "purgeCachedData", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->purgeCachedData();
		return 0;
	});

	Lua_NewFunc(L, "setDefaultValues", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->setDefaultValues();
		return 0;
	});

	Lua_NewFunc(L, "setGLDefaultValues", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->setGLDefaultValues();
		return 0;
	});

	Lua_NewFunc(L, "setAlphaBlending", [](lua_State* L)
	{
		var t = Lua_ToTuple<bool>(L, "setAlphaBlending error! need 1 args: bool on");
		cocos2d::Director::getInstance()->setAlphaBlending(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "setClearColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float>(L, "setClearColor error! need 4 args: float r, g, b, a");
		cocos2d::Director::getInstance()->setClearColor({ std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return 0;
	});

	Lua_NewFunc(L, "setDepthTest", [](lua_State* L)
	{
		var t = Lua_ToTuple<bool>(L, "setDepthTest error! need 1 args: bool on");
		cocos2d::Director::getInstance()->setDepthTest(std::get<0>(t));
		return 0;
	});

	// 不实现 mainLoop

	Lua_NewFunc(L, "setContentScaleFactor", [](lua_State* L)
	{
		var t = Lua_ToTuple<float>(L, "setContentScaleFactor error! need 1 args: float scaleFactor");
		cocos2d::Director::getInstance()->setContentScaleFactor(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "getContentScaleFactor", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getContentScaleFactor();
		return Lua_Pushs(L, r);
	});

	// 不实现 getScheduler setScheduler getActionManager setActionManager getEventDispatcher setEventDispatcher getRenderer getConsole

	Lua_NewFunc(L, "getDeltaTime", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getDeltaTime();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getFrameRate", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getFrameRate();
		return Lua_Pushs(L, r);
	});

	// 不实现 pushMatrix pushProjectionMatrix popMatrix popProjectionMatrix loadIdentityMatrix loadProjectionIdentityMatrix loadMatrix loadProjectionMatrix multiplyMatrix multiplyProjectionMatrix getMatrix getProjectionMatrix resetMatrixStack initProjectionMatrixStack getProjectionMatrixStackSize getCocos2dThreadId

	Lua_NewFunc(L, "isValid", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->isValid();
		return Lua_Pushs(L, r);
	});


	/**************************************************************************************************/
	// Application
	/**************************************************************************************************/


	Lua_NewFunc(L, "getTargetPlatform", [](lua_State* L)
	{
		var r = cocos2d::Application::getInstance()->getTargetPlatform();
		return Lua_Pushs(L, r);
	});

	lua_pushstring(L, TypeNames<cocos2d::ApplicationProtocol::Platform>::value);
	lua_createtable(L, 11, 0);
	lua_pushstring(L, "OS_WINDOWS");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_WINDOWS);	lua_rawset(L, -3);
	lua_pushstring(L, "OS_LINUX");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_LINUX);	lua_rawset(L, -3);
	lua_pushstring(L, "OS_MAC");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_MAC);	lua_rawset(L, -3);
	lua_pushstring(L, "OS_ANDROID");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_ANDROID);	lua_rawset(L, -3);
	lua_pushstring(L, "OS_IPHONE");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_IPHONE);	lua_rawset(L, -3);
	lua_pushstring(L, "OS_IPAD");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_IPAD);	lua_rawset(L, -3);
	lua_pushstring(L, "OS_BLACKBERRY");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_BLACKBERRY);	lua_rawset(L, -3);
	lua_pushstring(L, "OS_NACL");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_NACL);	lua_rawset(L, -3);
	lua_pushstring(L, "OS_EMSCRIPTEN");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_EMSCRIPTEN);	lua_rawset(L, -3);
	lua_pushstring(L, "OS_TIZEN");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_TIZEN);	lua_rawset(L, -3);
	lua_pushstring(L, "OS_WINRT");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_WINRT);	lua_rawset(L, -3);
	lua_pushstring(L, "OS_WP8");	lua_pushinteger(L, (int)cocos2d::ApplicationProtocol::Platform::OS_WP8);	lua_rawset(L, -3);
	lua_rawset(L, -3);



	/**************************************************************************************************/
	// EventDispacher
	/**************************************************************************************************/

	Lua_NewFunc(L, "addEventListenerWithSceneGraphPriority", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListener*, cocos2d::Node*>(L, "addEventListenerWithSceneGraphPriority error! need 2 args: EventListener listener, Node target");
		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "addEventListenerWithFixedPriority", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListener*, int>(L, "addEventListenerWithFixedPriority error! need 2 args: EventListener listener, int fixedPriority");
		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	// 不实现 addCustomEventListener

	Lua_NewFunc(L, "removeEventListener", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListener*>(L, "removeEventListener error! need 1 args: EventListener listener");
		cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeEventListenersForType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListener::Type>(L, "removeEventListenersForType error! need 1 args: EventListenerType type");
		cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListenersForType(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeEventListenersForTarget", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::Node*>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(std::get<0>(t));
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Node*, bool>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "removeEventListenersForTarget error! need 1 ~ 2 args: Node target, bool recursive = false");
		}
		}
		return 0;
	});

	// 不实现 removeCustomEventListeners

	Lua_NewFunc(L, "removeAllEventListeners", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		return 0;
	});

	Lua_NewFunc(L, "pauseEventListenersForTarget", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::Node*>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(std::get<0>(t));
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Node*, bool>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "pauseEventListenersForTarget error! need 1 ~ 2 args: Node target, bool recursive = false");
		}
		}
		return 0;
	});

	Lua_NewFunc(L, "resumeEventListenersForTarget", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::Node*>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(std::get<0>(t));
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Node*, bool>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "resumeEventListenersForTarget error! need 1 ~ 2 args: Node target, bool recursive = false");
		}
		}
		return 0;
	});

	Lua_NewFunc(L, "setPriority", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListener*, int>(L, "setPriority error! need 2 args: EventListener* listener, int fixedPriority");
		cocos2d::Director::getInstance()->getEventDispatcher()->setPriority(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListener*, int>(L, "setEnabled error! need 1 args: bool isEnabled");
		cocos2d::Director::getInstance()->getEventDispatcher()->setEnabled(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "isEnabled", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getEventDispatcher()->isEnabled();
		return Lua_Pushs(L, r);
	});

	// 不实现 dispatchEvent dispatchCustomEvent

	Lua_NewFunc(L, "isEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L);
		var r = cocos2d::Director::getInstance()->getEventDispatcher()->hasEventListener(std::get<0>(t));
		return Lua_Pushs(L, r);
	});


	lua_pushstring(L, TypeNames<cocos2d::EventListener::Type>::value);
	lua_createtable(L, 9, 0);
	lua_pushstring(L, "UNKNOWN");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::UNKNOWN);	lua_rawset(L, -3);
	lua_pushstring(L, "TOUCH_ONE_BY_ONE");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::TOUCH_ONE_BY_ONE);	lua_rawset(L, -3);
	lua_pushstring(L, "TOUCH_ALL_AT_ONCE");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::TOUCH_ALL_AT_ONCE);	lua_rawset(L, -3);
	lua_pushstring(L, "KEYBOARD");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::KEYBOARD);	lua_rawset(L, -3);
	lua_pushstring(L, "MOUSE");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::MOUSE);	lua_rawset(L, -3);
	lua_pushstring(L, "ACCELERATION");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::ACCELERATION);	lua_rawset(L, -3);
	lua_pushstring(L, "FOCUS");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::FOCUS);	lua_rawset(L, -3);
	lua_pushstring(L, "GAME_CONTROLLER");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::GAME_CONTROLLER);	lua_rawset(L, -3);
	lua_pushstring(L, "CUSTOM");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::CUSTOM);	lua_rawset(L, -3);
	lua_rawset(L, -3);


	/**************************************************************************************************/
	// FileUtils
	/**************************************************************************************************/

	Lua_NewFunc(L, "purgeCachedEntries", [](lua_State* L)
	{
		cocos2d::FileUtils::getInstance()->purgeCachedEntries();
		return 0;
	});

	Lua_NewFunc(L, "getStringFromFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->getStringFromFile(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->getStringFromFile(std::get<0>(t), [f = std::move(std::get<1>(t))](std::string str)
			{
				Lua_PCall(gLua, f, str);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "getStringFromFile error! need 1 ~ 2 args: string filename, function<void(string)> callback");
		}
	});

	Lua_NewFunc(L, "getDataFromFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->getDataFromFile(std::get<0>(t));
			var data = new cocos2d::Data(std::move(r));
			return Lua_Pushs(L, data);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->getDataFromFile(std::get<0>(t), [f = std::move(std::get<1>(t))](cocos2d::Data data)
			{
				var d = new cocos2d::Data(std::move(data));
				Lua_PCall(gLua, f, d);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "getDataFromFile error! need 1 ~ 2 args: string filename, function<void(Data)> callback");
		}
	});

	Lua_NewFunc(L, "getFileDataFromZip", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, std::string>(L, "getFileDataFromZip error! need 2 args: string zipFilePath, filename");
		ssize_t size = 0;
		var r = cocos2d::FileUtils::getInstance()->getFileDataFromZip(std::get<0>(t), std::get<1>(t), &size);
		var d = new cocos2d::Data();
		d->fastSet(r, size);
		return Lua_Pushs(L, d);
	});

	Lua_NewFunc(L, "fullPathForFilename", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "fullPathForFilename error! need 1 args: string filename");
		cocos2d::FileUtils::getInstance()->fullPathForFilename(std::get<0>(t));
		return 0;
	});

	// 不实现 loadFilenameLookupDictionaryFromFile setFilenameLookupDictionary

	Lua_NewFunc(L, "fullPathFromRelativeFile", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, std::string>(L, "fullPathFromRelativeFile error! need 2 args: string filename, relativeFile");
		var r = cocos2d::FileUtils::getInstance()->fullPathFromRelativeFile(std::get<0>(t), std::get<1>(t));
		return Lua_Pushs(L, r);
	});

	// 不实现 setSearchResolutionsOrder, addSearchResolutionsOrder getSearchResolutionsOrder

	Lua_NewFunc(L, "setSearchPaths", [](lua_State* L)
	{
		gStrings.clear();
		var numArgs = lua_gettop(L);
		if (numArgs)
		{
			for (int i = 1; i <= numArgs; ++i)
			{
				gStrings.emplace_back();
				Lua_Get(gStrings.back(), L, i);
			}
		}
		cocos2d::FileUtils::getInstance()->setSearchPaths(gStrings);
		return 0;
	});

	Lua_NewFunc(L, "getDefaultResourceRootPath", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setDefaultResourceRootPath", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "setDefaultResourceRootPath error! need 1 args: string path");
		cocos2d::FileUtils::getInstance()->setDefaultResourceRootPath(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "addSearchPath", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, bool>(L, "addSearchPath error! need 2 args: string path, bool front");
		cocos2d::FileUtils::getInstance()->addSearchPath(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getSearchPaths", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->getSearchPaths();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getOriginalSearchPaths", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->getOriginalSearchPaths();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getWritablePath", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->getWritablePath();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setWritablePath", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, bool>(L, "setWritablePath error! need 2 args: string writablePath");
		cocos2d::FileUtils::getInstance()->setWritablePath(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "setPopupNotify", [](lua_State* L)
	{
		var t = Lua_ToTuple<bool>(L, "setPopupNotify error! need 1 args: bool notify");
		cocos2d::FileUtils::getInstance()->setPopupNotify(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "isPopupNotify", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->isPopupNotify();
		return Lua_Pushs(L, r);
	});

	// 不实现 getValueMapFromFile getValueMapFromData writeToFile

	Lua_NewFunc(L, "writeStringToFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string, std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->writeStringToFile(std::get<0>(t), std::get<1>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->writeStringToFile(std::get<0>(t), std::get<1>(t), [f = std::move(std::get<2>(t))](bool successful)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successful);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "writeStringToFile error! need 2 ~ 3 args: string dataStr, string fullPath, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "writeDataToFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::Data*, std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->writeDataToFile(*std::get<0>(t), std::get<1>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Data*, std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->writeDataToFile(std::move(*std::get<0>(t)), std::get<1>(t), [f = std::move(std::get<2>(t))](bool successful)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successful);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "writeDataToFile error! need 2 ~ 3 args: Data data, string fullPath, function<void(bool)> callback");
		}
	});

	// 不实现 writeValueMapToFile writeValueVectorToFile writeValueVectorToFile getSuitableFOpen getValueVectorFromFile

	Lua_NewFunc(L, "isFileExist", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->isFileExist(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->isFileExist(std::get<0>(t), [f = std::move(std::get<1>(t))](bool exists)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, exists);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "isFileExist error! need 1 ~ 2 args: string filename, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "getFileExtension", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "getFileExtension error! need 1 args: string filePath");
		var r = cocos2d::FileUtils::getInstance()->getFileExtension(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "isAbsolutePath", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "isAbsolutePath error! need 1 args: string path");
		var r = cocos2d::FileUtils::getInstance()->isAbsolutePath(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "isDirectoryExist", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->isDirectoryExist(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->isDirectoryExist(std::get<0>(t), [f = std::move(std::get<1>(t))](bool exists)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, exists);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "isDirectoryExist error! need 1 ~ 2 args: string dirPath, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "createDirectory", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->createDirectory(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->createDirectory(std::get<0>(t), [f = std::move(std::get<1>(t))](bool successfully)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successfully);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "createDirectory error! need 1 ~ 2 args: string dirPath, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "removeDirectory", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->removeDirectory(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->removeDirectory(std::get<0>(t), [f = std::move(std::get<1>(t))](bool successfully)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successfully);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "removeDirectory error! need 1 ~ 2 args: string dirPath, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "removeFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->removeFile(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->removeFile(std::get<0>(t), [f = std::move(std::get<1>(t))](bool successfully)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successfully);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "removeFile error! need 1 ~ 2 args: string filepath, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "renameFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string, std::string, std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->renameFile(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, std::string, std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->renameFile(std::get<0>(t), std::get<1>(t), std::get<2>(t), [f = std::move(std::get<3>(t))](bool successfully)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successfully);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "renameFile error! need 3 ~ 4 args: string path, oldname, name, function<void(bool)> callback");
		}
	});

	// 不实现 renameFile( string string func )

	Lua_NewFunc(L, "getFileSize", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->getFileSize(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->getFileSize(std::get<0>(t), [f = std::move(std::get<1>(t))](bool successfully)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successfully);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "getFileSize error! need 1 ~ 2 args: string filepath, function<void(long)> callback");
		}
	});

	Lua_NewFunc(L, "listFiles", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->listFiles(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->listFilesAsync(std::get<0>(t), [f = std::move(std::get<1>(t))](std::vector<std::string> fs)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, fs);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "listFiles error! need 1 ~ 2 args: string dirPath, function<void(std::vector<std::string>)> callback");
		}
	});

	Lua_NewFunc(L, "listFilesRecursively", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			gStrings.clear();
			cocos2d::FileUtils::getInstance()->listFilesRecursively(std::get<0>(t), &gStrings);
			return Lua_Pushs(L, gStrings);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->listFilesRecursivelyAsync(std::get<0>(t), [f = std::move(std::get<1>(t))](std::vector<std::string> fs)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, fs);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "listFilesRecursively error! need 1 ~ 2 args: string dirPath, function<void(std::vector<std::string>)> callback");
		}
	});

	Lua_NewFunc(L, "getFullPathCache", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->getFullPathCache();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getNewFilename", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "getNewFilename error! need 1 args: string filename");
		var r = cocos2d::FileUtils::getInstance()->getNewFilename(std::get<0>(t));
		return Lua_Pushs(L, r);
	});



	/**************************************************************************************************/
	// UserDefault
	/**************************************************************************************************/

	Lua_NewFunc(L, "getBoolForKey", [](lua_State* L)
	{
		bool r = false;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<const char*>(L);
			r = cocos2d::UserDefault::getInstance()->getBoolForKey(std::get<0>(t));
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<const char*, bool>(L);
			r = cocos2d::UserDefault::getInstance()->getBoolForKey(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "getBoolForKey error! need 1 ~ 2 args: string key, bool defaultValue");
		}
		}
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getIntegerForKey", [](lua_State* L)
	{
		int64_t r = 0;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<const char*>(L);
			r = cocos2d::UserDefault::getInstance()->getIntegerForKey(std::get<0>(t));
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<const char*, int64_t>(L);
			r = cocos2d::UserDefault::getInstance()->getIntegerForKey(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "getIntegerForKey error! need 1 ~ 2 args: string key, int64_t defaultValue");
		}
		}
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getFloatForKey", [](lua_State* L)
	{
		float r = 0;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<const char*>(L);
			r = cocos2d::UserDefault::getInstance()->getFloatForKey(std::get<0>(t));
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<const char*, float>(L);
			r = cocos2d::UserDefault::getInstance()->getFloatForKey(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "getFloatForKey error! need 1 ~ 2 args: string key, float defaultValue");
		}
		}
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getDoubleForKey", [](lua_State* L)
	{
		double r = 0;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<const char*>(L);
			r = cocos2d::UserDefault::getInstance()->getDoubleForKey(std::get<0>(t));
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<const char*, double>(L);
			r = cocos2d::UserDefault::getInstance()->getDoubleForKey(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "getDoubleForKey error! need 1 ~ 2 args: string key, double defaultValue");
		}
		}
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getStringForKey", [](lua_State* L)
	{
		std::string r;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<const char*>(L);
			r = cocos2d::UserDefault::getInstance()->getStringForKey(std::get<0>(t));
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<const char*, std::string>(L);
			r = cocos2d::UserDefault::getInstance()->getStringForKey(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "getStringForKey error! need 1 ~ 2 args: string key, double defaultValue");
		}
		}
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getDataForKey", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<const char*>(L);
			var r = cocos2d::UserDefault::getInstance()->getDataForKey(std::get<0>(t));
			var data = new cocos2d::Data(std::move(r));
			return Lua_Pushs(L, data);
		}
		case 2:
		{
			var t = Lua_ToTuple<const char*, cocos2d::Data*>(L);
			var r = cocos2d::UserDefault::getInstance()->getDataForKey(std::get<0>(t), *std::get<1>(t));
			var data = new cocos2d::Data(std::move(r));
			return Lua_Pushs(L, data);
		}
		default:
		{
			return luaL_error(L, "%s", "getDataForKey error! need 1 ~ 2 args: string key, Data defaultValue");
		}
		}
	});

	Lua_NewFunc(L, "setBoolForKey", [](lua_State* L)
	{
		var t = Lua_ToTuple<char const*, bool>(L, "setBoolForKey error! need 2 args: string key, bool value");
		cocos2d::UserDefault::getInstance()->setBoolForKey(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setIntegerForKey", [](lua_State* L)
	{
		var t = Lua_ToTuple<char const*, int64_t>(L, "setIntegerForKey error! need 2 args: string key, int64_t value");
		cocos2d::UserDefault::getInstance()->setIntegerForKey(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setFloatForKey", [](lua_State* L)
	{
		var t = Lua_ToTuple<char const*, float>(L, "setFloatForKey error! need 2 args: string key, float value");
		cocos2d::UserDefault::getInstance()->setFloatForKey(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setDoubleForKey", [](lua_State* L)
	{
		var t = Lua_ToTuple<char const*, double>(L, "setDoubleForKey error! need 2 args: string key, double value");
		cocos2d::UserDefault::getInstance()->setDoubleForKey(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setStringForKey", [](lua_State* L)
	{
		var t = Lua_ToTuple<char const*, std::string>(L, "setStringForKey error! need 2 args: string key, string value");
		cocos2d::UserDefault::getInstance()->setStringForKey(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setDataForKey", [](lua_State* L)
	{
		var t = Lua_ToTuple<char const*, cocos2d::Data*>(L, "setDataForKey error! need 2 args: string key, Data data");
		cocos2d::UserDefault::getInstance()->setDataForKey(std::get<0>(t), *std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "flush", [](lua_State* L)
	{
		cocos2d::UserDefault::getInstance()->flush();
		return 0;
	});

	Lua_NewFunc(L, "deleteValueForKey", [](lua_State* L)
	{
		var t = Lua_ToTuple<char const*>(L, "deleteValueForKey error! need 1 args: string key");
		cocos2d::UserDefault::getInstance()->deleteValueForKey(std::get<0>(t));
		return 0;
	});


	/**************************************************************************************************/
	// TextureCache
	/**************************************************************************************************/

	Lua_NewFunc(L, "addImage", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::Director::getInstance()->getTextureCache()->addImage(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::Director::getInstance()->getTextureCache()->addImageAsync(std::get<0>(t), [f = std::move(std::get<1>(t))](cocos2d::Texture2D* t2d)
			{
				Lua_PCall(gLua, f, t2d);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "addImage error! need 1 ~ 2 args: string filepath, function<void(Texture2D*)> callback");
		}
	});

	Lua_NewFunc(L, "unbindImageAsync", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "unbindImageAsync error! need 1 args: string filepath");
		cocos2d::Director::getInstance()->getTextureCache()->unbindImageAsync(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "unbindAllImageAsync", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->getTextureCache()->unbindAllImageAsync();
		return 0;
	});

	Lua_NewFunc(L, "getTextureForKey", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "getTextureForKey error! need 1 args: string key");
		var r = cocos2d::Director::getInstance()->getTextureCache()->getTextureForKey(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "reloadTexture", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "reloadTexture error! need 1 args: string fileName");
		var r = cocos2d::Director::getInstance()->getTextureCache()->reloadTexture(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "removeAllTextures", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->getTextureCache()->removeAllTextures();
		return 0;
	});

	Lua_NewFunc(L, "removeUnusedTextures", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->getTextureCache()->removeUnusedTextures();
		return 0;
	});

	Lua_NewFunc(L, "removeTexture", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Texture2D*>(L, "removeTexture error! need 1 args: Texture2D texture");
		cocos2d::Director::getInstance()->getTextureCache()->removeTexture(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeTextureForKey", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "removeTextureForKey error! need 1 args: string fileName");
		cocos2d::Director::getInstance()->getTextureCache()->removeTextureForKey(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "getCachedTextureInfo", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getTextureCache()->getCachedTextureInfo();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getTextureFilePath", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Texture2D*>(L, "getTextureFilePath error! need 1 args: Texture2D texture");
		var r = cocos2d::Director::getInstance()->getTextureCache()->getTextureFilePath(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "renameTextureWithKey", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, std::string>(L, "renameTextureWithKey error! need 1 args: string srcName, dstName");
		cocos2d::Director::getInstance()->getTextureCache()->renameTextureWithKey(std::get<0>(t), std::get<1>(t));
		return 0;
	});



	/**************************************************************************************************/
	// SpriteFrameCache
	/**************************************************************************************************/


	Lua_NewFunc(L, "addSpriteFramesWithFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(std::get<0>(t));
			break;
		}
		case 2:
		{
			if (lua_isstring(L, 2))
			{
				var t = Lua_ToTuple<std::string, std::string>(L);
				cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(std::get<0>(t), std::get<1>(t));
			}
			else
			{
				var t = Lua_ToTuple<std::string, cocos2d::Texture2D*>(L);
				cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(std::get<0>(t), std::get<1>(t));
			}
			break;
		}
		default:
			return luaL_error(L, "%s", "addSpriteFramesWithFile error! need 1 ~ 2 args: string plist, string textureFileName / Texture2D texture");
		}
		return 0;
	});


	Lua_NewFunc(L, "addSpriteFramesWithFileContent", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, cocos2d::Texture2D*>(L, "addSpriteFramesWithFileContent error! need 2 args: string plist_content, Texture2D texture");
		cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFileContent(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "addSpriteFrame", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*, std::string>(L, "addSpriteFrame error! need 2 args: SpriteFrame frame, string frameName");
		cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isSpriteFramesWithFileLoaded", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "isSpriteFramesWithFileLoaded error! need 1 args: string plist");
		var r = cocos2d::SpriteFrameCache::getInstance()->isSpriteFramesWithFileLoaded(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "removeSpriteFrames", [](lua_State* L)
	{
		cocos2d::SpriteFrameCache::getInstance()->removeSpriteFrames();
		return 0;
	});

	Lua_NewFunc(L, "removeUnusedSpriteFrames", [](lua_State* L)
	{
		cocos2d::SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
		return 0;
	});

	Lua_NewFunc(L, "removeSpriteFrameByName", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "removeSpriteFrameByName error! need 1 args: string name");
		cocos2d::SpriteFrameCache::getInstance()->removeSpriteFrameByName(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeSpriteFramesFromFile", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "removeSpriteFramesFromFile error! need 1 args: string plist");
		cocos2d::SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeSpriteFramesFromFileContent", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "removeSpriteFramesFromFileContent error! need 1 args: string plist_content");
		cocos2d::SpriteFrameCache::getInstance()->removeSpriteFramesFromFileContent(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeSpriteFramesFromTexture", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Texture2D*>(L, "removeSpriteFramesFromTexture error! need 1 args: Texture2D texture");
		cocos2d::SpriteFrameCache::getInstance()->removeSpriteFramesFromTexture(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "getSpriteFrameByName", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "getSpriteFrameByName error! need 1 args: string name");
		var r = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "reloadTexture", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "reloadTexture error! need 1 args: string plist");
		var r = cocos2d::SpriteFrameCache::getInstance()->reloadTexture(std::get<0>(t));
		return Lua_Pushs(L, r);
	});




	/**************************************************************************************************/
	// AnimationCache
	/**************************************************************************************************/

	// todo




	/**************************************************************************************************/
	// cocos2d classes
	/**************************************************************************************************/


	// 创建 cc.Xxxxxx 元表及函数									// cc
	Lua_Register_Data(L);
	Lua_Register_Ref(L);
	Lua_Register_Node(L);
	Lua_Register_Scene(L);
	Lua_Register_Touch(L);
	Lua_Register_Event(L);
	Lua_Register_EventListener(L);
	Lua_Register_EventListenerTouchAllAtOnce(L);
	Lua_Register_EventListenerTouchOneByOne(L);
	Lua_Register_Sprite(L);
	Lua_Register_Label(L);
	Lua_Register_ScrollViews(L);
	Lua_Register_SpriteFrame(L);
	Lua_Register_Texture(L);
	Lua_Register_Actions(L);
	// .....
	Lua_Register_uiWidget(L);
	Lua_Register_uiButton(L);
	Lua_Register_uiImageView(L);
	Lua_Register_uiEditBox(L);
	// .....



	/**************************************************************************************************/
	// cocos2d some enums
	/**************************************************************************************************/

	lua_pushstring(L, "Projection");
	lua_createtable(L, 4, 0);
	lua_pushstring(L, "_2D");	lua_pushinteger(L, (int)cocos2d::Director::Projection::_2D);	lua_rawset(L, -3);
	lua_pushstring(L, "_3D");	lua_pushinteger(L, (int)cocos2d::Director::Projection::_3D);	lua_rawset(L, -3);
	lua_pushstring(L, "CUSTOM");	lua_pushinteger(L, (int)cocos2d::Director::Projection::CUSTOM);	lua_rawset(L, -3);
	lua_pushstring(L, "DEFAULT");	lua_pushinteger(L, (int)cocos2d::Director::Projection::DEFAULT);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, "TextHAlignment");
	lua_createtable(L, 3, 0);
	lua_pushstring(L, "LEFT");	lua_pushinteger(L, (int)cocos2d::TextHAlignment::LEFT);	lua_rawset(L, -3);
	lua_pushstring(L, "CENTER");	lua_pushinteger(L, (int)cocos2d::TextHAlignment::CENTER);	lua_rawset(L, -3);
	lua_pushstring(L, "RIGHT");	lua_pushinteger(L, (int)cocos2d::TextHAlignment::RIGHT);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, "TextHAlignment");
	lua_createtable(L, 3, 0);
	lua_pushstring(L, "TOP");	lua_pushinteger(L, (int)cocos2d::TextVAlignment::TOP);	lua_rawset(L, -3);
	lua_pushstring(L, "CENTER");	lua_pushinteger(L, (int)cocos2d::TextVAlignment::CENTER);	lua_rawset(L, -3);
	lua_pushstring(L, "BOTTOM");	lua_pushinteger(L, (int)cocos2d::TextVAlignment::BOTTOM);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	// .....




	lua_pop(L, 1);													//
	assert(lua_gettop(L) == 0);
}
