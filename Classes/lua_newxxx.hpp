#pragma once

// 创建一个 userdata 存对象指针, 并设置其元表. 返回 1
inline int Lua_NewUserdataMT(lua_State* const& L, void* const& o, char const* const& mtKey)
{
	var ph = (void**)lua_newuserdata(L, sizeof(void**));	// ..., &o
	*ph = o;
	lua_rawgetp(L, LUA_REGISTRYINDEX, mtKey);						// ..., &o, mt
	lua_setmetatable(L, -2);										// ..., &o
	return 1;
}

// 创建 ??.元表 到栈顶. 同时在注册表里面也存一份
inline void Lua_NewMT(lua_State* const& L, char const* const& mtKey, char const* const& parentMtKey = nullptr)
{
	lua_createtable(L, 0, 20);										// ..., MT
	lua_pushvalue(L, -1);											// ..., MT, MT
	lua_rawsetp(L, LUA_REGISTRYINDEX, (void*)mtKey);				// ..., MT

	lua_pushstring(L, mtKey);										// ..., MT, "mtKey"
	lua_pushvalue(L, -2);											// ..., MT, "mtKey", MT
	lua_rawset(L, -4);												// ..., MT

	lua_pushstring(L, "__index");									// ..., MT, "__index"
	lua_pushvalue(L, -2);											// ..., MT, "__index", MT
	lua_rawset(L, -3);												// ..., MT

	if (parentMtKey)
	{
		lua_rawgetp(L, LUA_REGISTRYINDEX, parentMtKey);				// ..., MT, PMT
		lua_setmetatable(L, -2);									// ..., MT
	}
}

// 创建 函数 到栈顶的表
inline void Lua_NewFunc(lua_State* const& L, char const* const& funcName, lua_CFunction func)
{
	lua_pushstring(L, funcName);									// ..., Xxxxx, funcName
	lua_pushcclosure(L, func, 0);									// ..., Xxxxx, funcName, func
	lua_rawset(L, -3);												// ..., Xxxxx
}
