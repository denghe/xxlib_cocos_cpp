#pragma once

inline void Lua_Register_Scene(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Scene*>::value, TypeNames<cocos2d::Node*>::value);
	lua_pop(L, 1);
}
