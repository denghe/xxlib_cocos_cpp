#pragma once

inline void Lua_Register_sys(lua_State* const& L)
{
	lua_createtable(L, 0, 100);										// sys
	lua_pushvalue(L, -1);											// sys, sys
	lua_setglobal(L, LuaKey_sys);									// sys

	Lua_NewFunc(L, "IsNetworkReachable", [](lua_State* L)
	{
		auto&& r = IsNetworkReachable();
		return Lua_Pushs(L, r);
	});

	lua_pop(L, 1);
	assert(lua_gettop(L) == 0);
}
