#pragma once

inline const char * const LuaKey_FrameUpdateFunc = "FrameUpdateFunc";
inline const char * const LuaKey_UvLoop = "UvLoop";
inline const char * const LuaKey_null = "null";

// todo mappings
// 

inline void Lua_Register_Sprite(lua_State* L)
{
	// todo: 在 cc 下创建子表, 同时子表也是 metatable
}

inline void Lua_Register_cc(lua_State* L)
{
	lua_createtable(L, 0, 100);										// cc
	lua_pushvalue(L, -1);											// cc, cc
	lua_setglobal(L, "cc");											// cc
	lua_pushstring(L, "restart");									// cc, "restart"
	lua_pushcfunction(L, [](lua_State* L)							// cc, "restart", func
	{
		std::cout << "restart" << std::endl;
		cocos2d::Director::getInstance()->mainLoopCallback = [] 
		{
			AppDelegate::instance->Restart();
		};
		return 0;
	});
	lua_rawset(L, -3);												// cc

	Lua_Register_Sprite(L);											// cc
	// .....
	// .....
	lua_pop(L, 1);													//
}


inline int Lua_Main(lua_State* L)
{
	luaL_openlibs(L);

	Lua_Register_cc(L);

	if (int r = luaL_loadfile(L, "main.lua"))
	{
		std::cout << "r = " << r << ", errmsg = " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return 0;
	}

	assert(lua_gettop(L) == 1);										// main
	lua_pushlightuserdata(L, (void*)LuaKey_FrameUpdateFunc);		// main, key
	lua_pushvalue(L, 1);											// main, key, main
	lua_rawset(L, LUA_REGISTRYINDEX);								// main
	assert(lua_gettop(L) == 1);										// main
	lua_pop(L, 1);													//

	cocos2d::Director::getInstance()->mainLoopCallback = [L]
	{
		lua_pushlightuserdata(L, (void*)LuaKey_FrameUpdateFunc);	// key
		lua_rawget(L, LUA_REGISTRYINDEX);							// main
		if (int r = lua_pcall(L, 0, LUA_MULTRET, 0))				//
		{
			std::cout << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	};

	return 0;
}

inline int Lua_Init()
{
	auto L = AppDelegate::L = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
	{
		return ((xx::MemPool*)ud)->Realloc(ptr, nsize, osize);
	}
	, &AppDelegate::instance->mp);

	assert(L);

	lua_pushcclosure(L, &Lua_Main, 0);
	if (int r = lua_pcall(L, 0, LUA_MULTRET, 0))
	{
		std::cout << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return r;
	}
	return 0;
}
