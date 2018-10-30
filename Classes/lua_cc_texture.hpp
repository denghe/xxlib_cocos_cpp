#pragma once

inline void Lua_Register_Texture(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Texture2D*>::value, TypeNames<cocos2d::Ref*>::value);
	lua_pop(L, 1);
}
