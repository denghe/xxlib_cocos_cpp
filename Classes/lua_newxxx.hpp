#pragma once

// 创建一个 userdata 存对象指针, 并设置其元表
inline int Lua_NewUserdataMT(lua_State* const& L, void* const& o, char const* const& mtKey)
{
	var ph = (void**)lua_newuserdata(L, sizeof(void**));	// ..., &o
	*ph = o;
	lua_rawgetp(L, LUA_REGISTRYINDEX, mtKey);						// ..., &o, mt
	lua_setmetatable(L, -2);										// ..., &o
	return 1;
}

// 创建 cc.Xxxxx 元表. 同时在注册表里面也存一份
inline void Lua_NewCcMT(lua_State* const& L, char const* const& mtKey, char const* const& parentMtKey = nullptr)
{
	lua_createtable(L, 0, 20);										// cc, MT
	lua_pushvalue(L, -1);											// cc, MT, MT
	lua_rawsetp(L, LUA_REGISTRYINDEX, (void*)mtKey);				// cc, MT

	lua_pushstring(L, mtKey);										// cc, MT, "mtKey"
	lua_pushvalue(L, -2);											// cc, MT, "mtKey", MT
	lua_rawset(L, -4);												// cc, MT

	lua_pushstring(L, "__index");									// cc, MT, "__index"
	lua_pushvalue(L, -2);											// cc, MT, "__index", MT
	lua_rawset(L, -3);												// cc, MT

	if (parentMtKey)
	{
		lua_rawgetp(L, LUA_REGISTRYINDEX, parentMtKey);				// cc, MT, PMT
		lua_setmetatable(L, -2);									// cc, MT
	}
}

// 创建 cc.Xxxxx.Xxx 函数
inline void Lua_NewFunc(lua_State* const& L, char const* const& funcName, lua_CFunction func)
{
	lua_pushstring(L, funcName);									// cc, Xxxxx, funcName
	lua_pushcclosure(L, func, 0);									// cc, Xxxxx, funcName, func
	lua_rawset(L, -3);												// cc, Xxxxx
}
