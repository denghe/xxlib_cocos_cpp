	Lua_NewMT(L, TypeNames<cocos2d::ui::Button*>::value, TypeNames<cocos2d::ui::Widget*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::ui::Button();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "init", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*>(L);
			std::get<0>(t)->init();
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string>(L);
			std::get<0>(t)->init(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string, std::string>(L);
			std::get<0>(t)->init(std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string, std::string, std::string>(L);
			std::get<0>(t)->init(std::get<1>(t), std::get<2>(t), std::get<3>(t));
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string, std::string, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->init(std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t));
			break;
		}
		default:
			return luaL_error(L, "init error! need 1 ~ 5 args: self, string normalImage, selectedImage = "", disableImage = "", TextureResType texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::ui::Button* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			o = cocos2d::ui::Button::create(std::get<0>(t));
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, std::string>(L);
			o = cocos2d::ui::Button::create(std::get<0>(t), std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<std::string, std::string, std::string>(L);
			o = cocos2d::ui::Button::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<std::string, std::string, std::string, cocos2d::ui::Widget::TextureResType>(L);
			o = cocos2d::ui::Button::create(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
			break;
		}
		default:
			return luaL_error(L, "init error! need 1 ~ 4 args: string normalImage, selectedImage = "", disableImage = "", TextureResType texType = LOCAL");
		}
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "loadTextures", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string, std::string>(L);
			std::get<0>(t)->loadTextures(std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string, std::string, std::string>(L);
			std::get<0>(t)->loadTextures(std::get<1>(t), std::get<2>(t), std::get<3>(t));
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string, std::string, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->loadTextures(std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t));
			break;
		}
		default:
			return luaL_error(L, "loadTextures error! need 3 ~ 5 args: self, string normalImage, selectedImage, disableImage = "", TextureResType texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "loadTextureNormal", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string>(L);
			std::get<0>(t)->loadTextureNormal(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->loadTextureNormal(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "loadTextureNormal error! need 2 ~ 3 args: self, string normal, TextureResType texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "loadTexturePressed", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string>(L);
			std::get<0>(t)->loadTexturePressed(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->loadTexturePressed(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "loadTexturePressed error! need 2 ~ 3 args: self, string selected, TextureResType texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "loadTextureDisabled", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string>(L);
			std::get<0>(t)->loadTextureDisabled(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->loadTextureDisabled(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "loadTextureDisabled error! need 2 ~ 3 args: self, string disabled, TextureResType texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "setCapInsets", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, float, float, float, float>(L, "setCapInsets error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setCapInsets({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "setCapInsetsNormalRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, float, float, float, float>(L, "setCapInsetsNormalRenderer error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setCapInsetsNormalRenderer({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getCapInsetsNormalRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getCapInsetsNormalRenderer error! need 1 args: self");
		var r = std::get<0>(t)->getCapInsetsNormalRenderer();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "setCapInsetsPressedRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, float, float, float, float>(L, "setCapInsetsPressedRenderer error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setCapInsetsPressedRenderer({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getCapInsetsPressedRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getCapInsetsPressedRenderer error! need 1 args: self");
		var r = std::get<0>(t)->getCapInsetsPressedRenderer();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "setCapInsetsDisabledRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, float, float, float, float>(L, "setCapInsetsDisabledRenderer error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setCapInsetsDisabledRenderer({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getCapInsetsDisabledRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getCapInsetsDisabledRenderer error! need 1 args: self");
		var r = std::get<0>(t)->getCapInsetsDisabledRenderer();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "setScale9Enabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, bool>(L, "setScale9Enabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setScale9Enabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isScale9Enabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "isScale9Enabled error! need 1 args: self");
		var r = std::get<0>(t)->isScale9Enabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setPressedActionEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, bool>(L, "setPressedActionEnabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setPressedActionEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getTitleRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getTitleRenderer error! need 1 args: self");
		var r = std::get<0>(t)->getTitleRenderer();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setTitleText", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, std::string>(L, "setTitleText error! need 2 args: self, string text");
		std::get<0>(t)->setTitleText(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getTitleText", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getTitleText error! need 1 args: self");
		var r = std::get<0>(t)->getTitleText();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setTitleColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, GLubyte, GLubyte, GLubyte>(L, "setTitleColor error! need 4 args: self, GLubyte r, g, b");
		std::get<0>(t)->setTitleColor({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getTitleColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getTitleColor error! need 1 args: self");
		var r = std::get<0>(t)->getTitleColor();
		return Lua_Pushs(L, r.r, r.g, r.b);
	});

	Lua_NewFunc(L, "setTitleFontSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, float>(L, "setTitleFontSize error! need 2 args: self, float size");
		std::get<0>(t)->setTitleFontSize(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getTitleFontSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getTitleFontSize error! need 1 args: self");
		var r = std::get<0>(t)->getTitleFontSize();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setTitleFontName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, std::string>(L, "setTitleFontName error! need 2 args: self, string name");
		std::get<0>(t)->setTitleFontName(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getTitleFontName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getTitleFontName error! need 1 args: self");
		var r = std::get<0>(t)->getTitleFontName();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setTitleAlignment", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, cocos2d::TextHAlignment>(L);
			std::get<0>(t)->setTitleAlignment(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::Button*, cocos2d::TextHAlignment, cocos2d::TextVAlignment>(L);
			std::get<0>(t)->setTitleAlignment(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "setTitleAlignment error! need 2 ~ 3 args: self, TextHAlignment ha, TextVAlignment va");
		}
		return 0;

	});

	Lua_NewFunc(L, "setTitleLabel", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, cocos2d::Label*>(L, "setTitleLabel error! need 2 args: self, Label label");
		std::get<0>(t)->setTitleLabel(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getTitleLabel", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getTitleLabel error! need 1 args: self");
		var r = std::get<0>(t)->getTitleLabel();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setZoomScale", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, float>(L, "setZoomScale error! need 2 args: self, float scale");
		std::get<0>(t)->setZoomScale(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getZoomScale", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getZoomScale error! need 1 args: self");
		var r = std::get<0>(t)->getZoomScale();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getRendererNormal", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getRendererNormal error! need 1 args: self");
		var r = std::get<0>(t)->getRendererNormal();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getRendererClicked", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getRendererClicked error! need 1 args: self");
		var r = std::get<0>(t)->getRendererClicked();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getRendererDisabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getRendererDisabled error! need 1 args: self");
		var r = std::get<0>(t)->getRendererDisabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "resetNormalRender", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "resetNormalRender error! need 1 args: self");
		std::get<0>(t)->resetNormalRender();
		return 0;
	});

	Lua_NewFunc(L, "resetPressedRender", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "resetPressedRender error! need 1 args: self");
		std::get<0>(t)->resetPressedRender();
		return 0;
	});

	Lua_NewFunc(L, "resetDisabledRender", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "resetDisabledRender error! need 1 args: self");
		std::get<0>(t)->resetDisabledRender();
		return 0;
	});

	Lua_NewFunc(L, "getNormalTextureSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*>(L, "getNormalTextureSize error! need 1 args: self");
		var r = std::get<0>(t)->getNormalTextureSize();
		return Lua_Pushs(L, r.width, r.height);
	});



	lua_pop(L, 1);
