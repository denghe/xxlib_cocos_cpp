#pragma once

inline void Lua_Register_Touch(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Touch*>::value, TypeNames<cocos2d::Ref*>::value);

	Lua_NewFunc(L, "getLocation", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Touch*>(L, "getLocation error! need 1 args: self");
		var r = std::get<0>(t)->getLocation();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getLocationInView", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Touch*>(L, "getLocationInView error! need 1 args: self");
		var r = std::get<0>(t)->getLocationInView();
		return Lua_Pushs(L, r.x, r.y);
	});

	lua_pop(L, 1);
}
