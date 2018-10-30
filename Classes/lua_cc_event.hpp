#pragma once

inline void Lua_Register_Event(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Event*>::value, TypeNames<cocos2d::Ref*>::value);

	// event->getCurrentTarget()

	lua_pop(L, 1);
}
