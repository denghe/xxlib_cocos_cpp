#pragma once

inline void Lua_Register_Event(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_Event, LuaKey_Ref);

	// event->getCurrentTarget()

	lua_pop(L, 1);
}
