
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

	Lua_NewFunc(L, "getTargetPlatform", [](lua_State* L)
	{
		var r = cocos2d::Application::getInstance()->getTargetPlatform();
		return Lua_Pushs(L, r);
	});

	lua_pushstring(L, TypeNames<cocos2d::ApplicationProtocol::Platform>::value);
	lua_createtable(L, 12, 0);
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
