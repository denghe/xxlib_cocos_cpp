#pragma once

// 用于临时存放为 Sequence, Spawns 传的参
inline cocos2d::Vector<cocos2d::FiniteTimeAction*> gActions;

inline void Lua_Register_cc(lua_State* const& L)
{
	// 创建全局 cc 基表
	lua_createtable(L, 0, 100);										// cc
	lua_pushvalue(L, -1);											// cc, cc
	lua_setglobal(L, LuaKey_cc);									// cc

	// 创建 重启游戏 函数
	Lua_NewFunc(L, "restart", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->mainLoopCallback = []
		{
			cocos2d::Director::getInstance()->restart();	// 下帧重启
		};
		return 0;
	});

	// 创建 拿 scene 函数
	Lua_NewFunc(L, "scene", [](lua_State* L)
	{
		return Lua_Push(L, gScene);
	});

	// 创建 帧循环事件设置 函数
	Lua_NewFunc(L, "mainLoopCallback", [](lua_State* L)
	{
		var t = Lua_ToTuple<Lua_Func>(L, "mainLoopCallback error! need 1 args: func/null");
		if (std::get<0>(t))
		{
			cocos2d::Director::getInstance()->mainLoopCallback = [f = std::move(std::get<0>(t))]
			{
				uv->Run(xx::UvRunMode::NoWait);		// 这个需要一直在的
				Lua_PCall(gLua, f);
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
				Lua_PCall(gLua, f);
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
				Lua_PCall(gLua, f);
			};
		}
		else
		{
			enterForeground = nullptr;
		}
		return 0;
	});


	Lua_NewFunc(L, "setPopupNotify", [](lua_State* L)
	{
		var t = Lua_ToTuple<bool>(L, "setPopupNotify error! need 1 args: bool");
		cocos2d::FileUtils::getInstance()->setPopupNotify(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "addSearchPath", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, bool>(L, "addSearchPath error! need 2 args: path(string), front(bool)");
		cocos2d::FileUtils::getInstance()->addSearchPath(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getWritablePath", [](lua_State* L)
	{
		return Lua_Push(L, cocos2d::FileUtils::getInstance()->getWritablePath());
	});

	Lua_NewFunc(L, "getTargetPlatform", [](lua_State* L)
	{
		return Lua_Push(L, cocos2d::Application::getInstance()->getTargetPlatform());
	});

	lua_pushstring(L, "Platform");
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


	Lua_NewFunc(L, "addEventListenerWithSceneGraphPriority", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListener*, cocos2d::Node*>(L, "addEventListenerWithSceneGraphPriority error! need 2 args: listener, target(Node)");
		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeEventListener", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListener*, cocos2d::Node*>(L, "removeEventListener error! need 1 args: listener");
		cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(std::get<0>(t));
		return 0;
	});






	// todo: more

	// 创建 cc.Xxxxxx 元表及函数									// cc
	Lua_Register_Ref(L);
	Lua_Register_Actions(L);
	Lua_Register_Node(L);
	Lua_Register_Scene(L);
	Lua_Register_Touch(L);
	Lua_Register_Event(L);
	Lua_Register_EventListener(L);
	Lua_Register_EventListenerTouchAllAtOnce(L);
	Lua_Register_EventListenerTouchOneByOne(L);
	Lua_Register_Sprite(L);
	//Lua_Register_SpriteFrame(L);							
	Lua_Register_Texture(L);
	Lua_Register_TextureCache(L);
	// .....
	Lua_Register_uiWidget(L);
	Lua_Register_uiButton(L);
	// .....

	lua_pop(L, 1);													//
	assert(lua_gettop(L) == 0);
}
