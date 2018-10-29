#pragma once

inline void Lua_Register_Ref(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_Ref);										// cc, Ref

	Lua_NewFunc(L, "retain", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::Ref*>(L, "retain error! need 1 args: self", LuaKey_Ref);
		std::get<0>(t)->retain();
		return 0;
	});

	Lua_NewFunc(L, "release", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::Ref*>(L, "release error! need 1 args: self", LuaKey_Ref);
		std::get<0>(t)->release();
		return 0;
	});

	Lua_NewFunc(L, "autorelease", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::Ref*>(L, "autorelease error! need 1 args: self", LuaKey_Ref);
		std::get<0>(t)->autorelease();
		return 0;
	});

	Lua_NewFunc(L, "getReferenceCount", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::Ref*>(L, "getReferenceCount error! need 1 args: self", LuaKey_Ref);
		lua_pushinteger(L, std::get<0>(t)->getReferenceCount());
		return 1;
	});

	lua_pop(L, 1);													// cc
}
