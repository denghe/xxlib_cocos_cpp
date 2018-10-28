#pragma once

inline void Lua_Register_Touch(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_Touch, LuaKey_Ref);

	Lua_NewFunc(L, "getLocation", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::Touch>(L, "getLocation error! need 1 args: self", LuaKey_Touch);
		var r = std::get<0>(t)->getLocation();
		lua_pushnumber(L, r.x);
		lua_pushnumber(L, r.y);
		return 2;
	});

	Lua_NewFunc(L, "getLocationInView", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::Touch>(L, "getLocationInView error! need 1 args: self", LuaKey_Touch);
		var r = std::get<0>(t)->getLocationInView();
		lua_pushnumber(L, r.x);
		lua_pushnumber(L, r.y);
		return 2;
	});

	lua_pop(L, 1);
}
