#pragma once

inline void Lua_Register_Touch(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Touch, LuaKey_Ref);

	lua_pushstring(L, "getLocation");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "getLocation error! need 1 args: self");
		var o = Lua_ToPointer<cocos2d::Touch, 1>(L, LuaKey_Touch);
		var v = o->getLocation();
		lua_pushnumber(L, v.x);
		lua_pushnumber(L, v.y);
		return 2;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "getLocationInView");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "getLocationInView error! need 1 args: self");
		var o = Lua_ToPointer<cocos2d::Touch, 1>(L, LuaKey_Touch);
		var v = o->getLocationInView();
		lua_pushnumber(L, v.x);
		lua_pushnumber(L, v.y);
		return 2;
	}, 0);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}
