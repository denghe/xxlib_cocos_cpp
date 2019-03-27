#pragma once

inline void Lua_Register_ScrollViews(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::extension::ScrollView*>::value, TypeNames<cocos2d::Layer*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		auto&& o = new (std::nothrow) cocos2d::extension::ScrollView();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		auto&& o = cocos2d::extension::ScrollView::create();
		return Lua_Push(L, o);
	});

	// todo

	lua_pop(L, 1);






	Lua_NewMT(L, TypeNames<cocos2d::extension::TableView*>::value, TypeNames<cocos2d::extension::ScrollView*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		auto&& o = new (std::nothrow) cocos2d::extension::TableView();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		auto&& o = cocos2d::extension::TableView::create();
		return Lua_Push(L, o);
	});

	// todo

	lua_pop(L, 1);

}
