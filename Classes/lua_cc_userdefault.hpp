
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
		int r = 0;
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
			var t = Lua_ToTuple<const char*, int>(L);
			r = cocos2d::UserDefault::getInstance()->getIntegerForKey(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "getIntegerForKey error! need 1 ~ 2 args: string key, int32 defaultValue");
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
		var t = Lua_ToTuple<char const*, int>(L, "setIntegerForKey error! need 2 args: string key, int32 value");
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

