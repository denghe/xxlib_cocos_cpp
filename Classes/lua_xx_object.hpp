#pragma once

inline void Lua_Register_Object(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_Object);									// xx, t
	lua_pop(L, 1);													// xx
}
