#pragma once

inline void Lua_Register_Scene(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Scene, LuaKey_Node);
	lua_pop(L, 1);
}
