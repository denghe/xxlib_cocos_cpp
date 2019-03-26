#pragma once

inline void Lua_Register_Touch(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Touch*>::value, TypeNames<cocos2d::Ref*>::value);

	Lua_NewFunc(L, "getLocation", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Touch*>(L, "getLocation error! need 1 args: self");
		auto&& r = std::get<0>(t)->getLocation();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getPreviousLocation", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Touch*>(L, "getPreviousLocation error! need 1 args: self");
		auto&& r = std::get<0>(t)->getPreviousLocation();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getStartLocation", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Touch*>(L, "getStartLocation error! need 1 args: self");
		auto&& r = std::get<0>(t)->getStartLocation();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getDelta", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Touch*>(L, "getDelta error! need 1 args: self");
		auto&& r = std::get<0>(t)->getDelta();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getLocationInView", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Touch*>(L, "getLocationInView error! need 1 args: self");
		auto&& r = std::get<0>(t)->getLocationInView();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getPreviousLocationInView", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Touch*>(L, "getPreviousLocationInView error! need 1 args: self");
		auto&& r = std::get<0>(t)->getPreviousLocationInView();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getStartLocationInView", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Touch*>(L, "getStartLocationInView error! need 1 args: self");
		auto&& r = std::get<0>(t)->getStartLocationInView();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getID", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Touch*>(L, "getID error! need 1 args: self");
		auto&& r = std::get<0>(t)->getID();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getCurrentForce", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Touch*>(L, "getCurrentForce error! need 1 args: self");
		auto&& r = std::get<0>(t)->getCurrentForce();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getMaxForce", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Touch*>(L, "getMaxForce error! need 1 args: self");
		auto&& r = std::get<0>(t)->getMaxForce();
		return Lua_Pushs(L, r);
	});

	// 不实现 setTouchInfo

	lua_pop(L, 1);
}
