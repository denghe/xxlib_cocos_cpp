
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
				assert(!lua_gettop(gLua));
				//lua_settop(gLua, 0);
				//std::cout << ::gFrameNumber++ << std::endl;
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
	lua_createtable(L, 6, 0);
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


	lua_pushstring(L, TypeNames<cocos2d::Director::Projection>::value);
	lua_createtable(L, 4, 0);
	lua_pushstring(L, "_2D");	lua_pushinteger(L, (int)cocos2d::Director::Projection::_2D);	lua_rawset(L, -3);
	lua_pushstring(L, "_3D");	lua_pushinteger(L, (int)cocos2d::Director::Projection::_3D);	lua_rawset(L, -3);
	lua_pushstring(L, "CUSTOM");	lua_pushinteger(L, (int)cocos2d::Director::Projection::CUSTOM);	lua_rawset(L, -3);
	lua_pushstring(L, "DEFAULT");	lua_pushinteger(L, (int)cocos2d::Director::Projection::DEFAULT);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, TypeNames<cocos2d::TextHAlignment>::value);
	lua_createtable(L, 3, 0);
	lua_pushstring(L, "LEFT");	lua_pushinteger(L, (int)cocos2d::TextHAlignment::LEFT);	lua_rawset(L, -3);
	lua_pushstring(L, "CENTER");	lua_pushinteger(L, (int)cocos2d::TextHAlignment::CENTER);	lua_rawset(L, -3);
	lua_pushstring(L, "RIGHT");	lua_pushinteger(L, (int)cocos2d::TextHAlignment::RIGHT);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, TypeNames<cocos2d::TextVAlignment>::value);
	lua_createtable(L, 3, 0);
	lua_pushstring(L, "TOP");	lua_pushinteger(L, (int)cocos2d::TextVAlignment::TOP);	lua_rawset(L, -3);
	lua_pushstring(L, "CENTER");	lua_pushinteger(L, (int)cocos2d::TextVAlignment::CENTER);	lua_rawset(L, -3);
	lua_pushstring(L, "BOTTOM");	lua_pushinteger(L, (int)cocos2d::TextVAlignment::BOTTOM);	lua_rawset(L, -3);
	lua_rawset(L, -3);
