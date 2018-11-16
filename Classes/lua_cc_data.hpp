	Lua_NewMT(L, TypeNames<cocos2d::Data*>::value);

	Lua_NewFunc(L, "__gc", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Data*>(L, "__gc error! need 1 args: self");
		assert(std::get<0>(t));
		delete std::get<0>(t);
		return 0;
	});

	Lua_NewFunc(L, "clear", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Data*>(L, "clear error! need 1 args: self");
		std::get<0>(t)->clear();
		return 0;
	});

	lua_pop(L, 1);
