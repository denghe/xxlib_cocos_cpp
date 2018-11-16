	Lua_NewMT(L, TypeNames<cocos2d::Layer*>::value, TypeNames<cocos2d::Node*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::Layer();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var o = cocos2d::Layer::create();
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);
