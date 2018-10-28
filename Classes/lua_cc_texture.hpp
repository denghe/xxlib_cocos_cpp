#pragma once

inline void Lua_Register_Texture(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_Texture, LuaKey_Ref);
	lua_pop(L, 1);
}
