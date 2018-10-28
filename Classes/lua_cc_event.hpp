#pragma once

inline void Lua_Register_Event(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Event, LuaKey_Ref);

	// event->getCurrentTarget()

	lua_pop(L, 1);
}
