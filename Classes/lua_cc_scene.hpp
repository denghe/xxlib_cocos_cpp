	Lua_NewMT(L, TypeNames<cocos2d::Scene*>::value, TypeNames<cocos2d::Node*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::Scene* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 0:
		{
			o = cocos2d::Scene::create();
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<float, float>(L);
			o = cocos2d::Scene::createWithSize({ std::get<0>(t), std::get<1>(t) });
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "create Scene error! need 0, 2 args: float sizeW, sizeH");
		}
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	// getCameras getDefaultCamera getLights

	lua_pop(L, 1);
