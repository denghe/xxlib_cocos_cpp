	Lua_NewMT(L, TypeNames<cocos2d::ui::Layout*>::value, TypeNames<cocos2d::ui::Widget*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::ui::Layout();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var o = cocos2d::ui::Layout::create();
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "setBackGroundImage", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::ui::Layout*, std::string>(L);
			std::get<0>(t)->setBackGroundImage(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::Layout*, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->setBackGroundImage(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "setBackGroundImage error! need 2 ~ 3 args: self, string fileName, TextureResType texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "setBackGroundImageCapInsets", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, float, float, float, float>(L, "setBackGroundImageCapInsets error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setBackGroundImageCapInsets({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getBackGroundImageCapInsets", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getBackGroundImageCapInsets error! need 1 args: self");
		var r = std::get<0>(t)->getBackGroundImageCapInsets();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "setBackGroundColorType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, cocos2d::ui::Layout::BackGroundColorType>(L, "setBackGroundColorType error! need 2 args: self, BackGroundColorType type");
		std::get<0>(t)->setBackGroundColorType(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getBackGroundColorType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getBackGroundColorType error! need 1 args: self");
		var r = std::get<0>(t)->getBackGroundColorType();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setBackGroundImageScale9Enabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, bool>(L, "setBackGroundImageScale9Enabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setBackGroundImageScale9Enabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isBackGroundImageScale9Enabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "isBackGroundImageScale9Enabled error! need 1 args: self");
		var r = std::get<0>(t)->isBackGroundImageScale9Enabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setBackGroundColor", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::ui::Layout*, GLubyte, GLubyte, GLubyte>(L);
			std::get<0>(t)->setBackGroundColor({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
			break;
		}
		case 7:
		{
			var t = Lua_ToTuple<cocos2d::ui::Layout*, GLubyte, GLubyte, GLubyte, GLubyte, GLubyte, GLubyte>(L);
			std::get<0>(t)->setBackGroundColor({ std::get<1>(t), std::get<2>(t), std::get<3>(t) }, { std::get<4>(t), std::get<5>(t), std::get<6>(t) });
			break;
		}
		default:
			return luaL_error(L, "setBackGroundColor error! need 4 args: self, GLubyte startColor r, g, b, endColor r, g, b");
		}
		return 0;
	});

	Lua_NewFunc(L, "getBackGroundColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getBackGroundColor error! need 1 args: self");
		var r = std::get<0>(t)->getBackGroundColor();
		return Lua_Pushs(L, r.r, r.g, r.b);
	});

	Lua_NewFunc(L, "getBackGroundStartColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getBackGroundStartColor error! need 1 args: self");
		var r = std::get<0>(t)->getBackGroundStartColor();
		return Lua_Pushs(L, r.r, r.g, r.b);
	});

	Lua_NewFunc(L, "getBackGroundEndColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getBackGroundEndColor error! need 1 args: self");
		var r = std::get<0>(t)->getBackGroundEndColor();
		return Lua_Pushs(L, r.r, r.g, r.b);
	});

	Lua_NewFunc(L, "setBackGroundColorOpacity", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, int>(L, "setBackGroundColorOpacity error! need 2 args: self, GLubyte opacity");
		std::get<0>(t)->setBackGroundColorOpacity(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getBackGroundColorOpacity", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getBackGroundColorOpacity error! need 1 args: self");
		var r = std::get<0>(t)->getBackGroundColorOpacity();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setBackGroundColorVector", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, float, float>(L, "setBackGroundColorVector error! need 3 args: self, vectorX, vectorY");
		std::get<0>(t)->setBackGroundColorVector({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getBackGroundColorVector", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getBackGroundColorVector error! need 1 args: self");
		var r = std::get<0>(t)->getBackGroundColorVector();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setBackGroundImageColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, GLubyte, GLubyte, GLubyte>(L, "setBackGroundImageColor error! need 4 args: self, GLubyte color r, g, b");
		std::get<0>(t)->setBackGroundImageColor({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return 0;
	});

	Lua_NewFunc(L, "setBackGroundImageOpacity", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, int>(L, "setBackGroundImageOpacity error! need 2 args: self, GLubyte opacity");
		std::get<0>(t)->setBackGroundImageOpacity(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getBackGroundImageColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getBackGroundImageColor error! need 1 args: self");
		var r = std::get<0>(t)->getBackGroundImageColor();
		return Lua_Pushs(L, r.r, r.g, r.b);
	});

	Lua_NewFunc(L, "getBackGroundImageOpacity", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getBackGroundImageOpacity error! need 1 args: self");
		var r = std::get<0>(t)->getBackGroundImageOpacity();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "removeBackGroundImage", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "removeBackGroundImage error! need 1 args: self");
		std::get<0>(t)->removeBackGroundImage();
		return 0;
	});

	Lua_NewFunc(L, "getBackGroundImageTextureSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getBackGroundImageTextureSize error! need 1 args: self");
		var r = std::get<0>(t)->getBackGroundImageTextureSize();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "setClippingEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, bool>(L, "setClippingEnabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setClippingEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setClippingType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, cocos2d::ui::Layout::ClippingType>(L, "setClippingType error! need 2 args: self, ClippingType type");
		std::get<0>(t)->setClippingType(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getClippingType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getClippingType error! need 1 args: self");
		var r = std::get<0>(t)->getClippingType();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "isClippingEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "isClippingEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isClippingEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setLayoutType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, cocos2d::ui::Layout::Type>(L, "setLayoutType error! need 2 args: self, Type type");
		std::get<0>(t)->setLayoutType(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getLayoutType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getLayoutType error! need 1 args: self");
		var r = std::get<0>(t)->getLayoutType();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "forceDoLayout", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "forceDoLayout error! need 2 args: self");
		std::get<0>(t)->forceDoLayout();
		return 0;
	});

	Lua_NewFunc(L, "requestDoLayout", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "requestDoLayout error! need 2 args: self");
		std::get<0>(t)->requestDoLayout();
		return 0;
	});

	Lua_NewFunc(L, "setLoopFocus", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, bool>(L, "setLoopFocus error! need 2 args: self, bool loop");
		std::get<0>(t)->setLoopFocus(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isLoopFocus", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "isLoopFocus error! need 1 args: self");
		var r = std::get<0>(t)->isLoopFocus();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setPassFocusToChild", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*, bool>(L, "setPassFocusToChild error! need 2 args: self, bool pass");
		std::get<0>(t)->setPassFocusToChild(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isPassFocusToChild", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "isPassFocusToChild error! need 1 args: self");
		var r = std::get<0>(t)->isPassFocusToChild();
		return Lua_Pushs(L, r);
	});

	// 不实现 onPassFocusToChild

	Lua_NewFunc(L, "getRenderFile", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Layout*>(L, "getRenderFile error! need 1 args: self");
		var r = std::get<0>(t)->getRenderFile();
		return Lua_Pushs(L, r.type, r.file, r.plist);
	});

	lua_pop(L, 1);

	lua_pushstring(L, TypeNames<cocos2d::ui::Layout::ClippingType>::value);
	lua_createtable(L, 2, 0);
	lua_pushstring(L, "STENCIL");	lua_pushinteger(L, (int)cocos2d::ui::Layout::ClippingType::STENCIL);	lua_rawset(L, -3);
	lua_pushstring(L, "SCISSOR");	lua_pushinteger(L, (int)cocos2d::ui::Layout::ClippingType::SCISSOR);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, TypeNames<cocos2d::ui::Layout::Type>::value);
	lua_createtable(L, 4, 0);
	lua_pushstring(L, "ABSOLUTE");	lua_pushinteger(L, (int)cocos2d::ui::Layout::Type::ABSOLUTE);	lua_rawset(L, -3);
	lua_pushstring(L, "VERTICAL");	lua_pushinteger(L, (int)cocos2d::ui::Layout::Type::VERTICAL);	lua_rawset(L, -3);
	lua_pushstring(L, "HORIZONTAL");	lua_pushinteger(L, (int)cocos2d::ui::Layout::Type::HORIZONTAL);	lua_rawset(L, -3);
	lua_pushstring(L, "RELATIVE");	lua_pushinteger(L, (int)cocos2d::ui::Layout::Type::RELATIVE);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, TypeNames<cocos2d::ui::Layout::BackGroundColorType>::value);
	lua_createtable(L, 3, 0);
	lua_pushstring(L, "NONE");	lua_pushinteger(L, (int)cocos2d::ui::Layout::BackGroundColorType::NONE);	lua_rawset(L, -3);
	lua_pushstring(L, "SOLID");	lua_pushinteger(L, (int)cocos2d::ui::Layout::BackGroundColorType::SOLID);	lua_rawset(L, -3);
	lua_pushstring(L, "GRADIENT");	lua_pushinteger(L, (int)cocos2d::ui::Layout::BackGroundColorType::GRADIENT);	lua_rawset(L, -3);
	lua_rawset(L, -3);
