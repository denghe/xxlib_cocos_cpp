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
		cocos2d::Director::getInstance()->mainLoopCallback = []
		{
			cocos2d::Director::getInstance()->restart();	// 下帧重启
		};
		return 0;
	});



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



	Lua_NewFunc(L, "scene", [](lua_State* L)
	{
		return Lua_Pushs(L, gScene);
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
		var r = cocos2d::Director::getInstance()->getAnimationInterval();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "isDisplayStats", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->isDisplayStats();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setDisplayStats", [](lua_State* L)
	{
		var t = Lua_ToTuple<float>(L, "setDisplayStats error! need 1 args: bool displayStats");
		cocos2d::Director::getInstance()->setDisplayStats(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "getSecondsPerFrame", [](lua_State* L)
	{
		var r = cocos2d::Director::getInstance()->getSecondsPerFrame();
		return Lua_Pushs(L, r);
	});

	// getOpenGLView setOpenGLView

	// 不映射 getTextureCache. 它的函数在下面

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
		return Lua_Pushs(L,r);
	});


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
		var t = Lua_ToTuple<std::string>(L, "getStringFromFile error! need 1 args: string filename");
		var r = cocos2d::FileUtils::getInstance()->getStringFromFile(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getDataFromFile", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "getDataFromFile error! need 1 args: string filename");
		var r = cocos2d::FileUtils::getInstance()->getDataFromFile(std::get<0>(t));
		// todo: 转为 BBuffer
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setPopupNotify", [](lua_State* L)
	{
		var t = Lua_ToTuple<bool>(L, "setPopupNotify error! need 1 args: bool");
		cocos2d::FileUtils::getInstance()->setPopupNotify(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "addSearchPath", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, bool>(L, "addSearchPath error! need 2 args: string path, bool front");
		cocos2d::FileUtils::getInstance()->addSearchPath(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getWritablePath", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->getWritablePath();
		return Lua_Pushs(L, r);
	});

	// todo


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
			var data = cocos2d::UserDefault::getInstance()->getDataForKey(std::get<0>(t));
			Lua_BBuffer::Create(L);
			if (data.getSize())
			{
				var bb = *(Lua_BBuffer**)lua_touserdata(L, -1);
				bb->WriteBuf((const char*)data.getBytes(), data.getSize());
			}
			return 1;
		}
		case 2:
		{
			var t = Lua_ToTuple<const char*, xx::BBuffer*>(L);
			cocos2d::Data a;
			a.fastSet((unsigned char*)std::get<1>(t)->buf, std::get<1>(t)->dataLen);
			var data = cocos2d::UserDefault::getInstance()->getDataForKey(std::get<0>(t), a);
			a.fastSet(nullptr, 0);
			Lua_BBuffer::Create(L);
			if (data.getSize())
			{
				var bb = *(Lua_BBuffer**)lua_touserdata(L, -1);
				bb->WriteBuf((const char*)data.getBytes(), data.getSize());
			}
			return 1;
		}
		default:
		{
			return luaL_error(L, "%s", "getDataForKey error! need 1 ~ 2 args: string key, BBuffer defaultValue");
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
		var t = Lua_ToTuple<char const*, xx::BBuffer*>(L, "setDataForKey error! need 2 args: string key, BBuffer data");
		cocos2d::Data a;
		a.fastSet((unsigned char*)std::get<1>(t)->buf, std::get<1>(t)->dataLen);
		cocos2d::UserDefault::getInstance()->setDataForKey(std::get<0>(t), a);
		a.fastSet(nullptr, 0);
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
		var t = Lua_ToTuple<std::string>(L, "addImage error! need 1 args: string filepath");
		var o = cocos2d::Director::getInstance()->getTextureCache()->addImage(std::get<0>(t));
		return Lua_Pushs(L, o);
	});

	// todo


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
	// cocos2d classes
	/**************************************************************************************************/


	// 创建 cc.Xxxxxx 元表及函数									// cc
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
