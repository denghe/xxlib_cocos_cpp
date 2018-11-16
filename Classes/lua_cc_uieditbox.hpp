	Lua_NewMT(L, TypeNames<cocos2d::ui::EditBox*>::value, TypeNames<cocos2d::ui::Widget*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::ui::EditBox();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "initWithSizeAndBackgroundSprite", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, float, float, std::string>(L);
			std::get<0>(t)->initWithSizeAndBackgroundSprite({ std::get<1>(t), std::get<2>(t) }, std::get<3>(t));
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, float, float, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->initWithSizeAndBackgroundSprite({ std::get<1>(t), std::get<2>(t) }, std::get<3>(t), std::get<4>(t));
			break;
		}
		case 6:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, float, float, cocos2d::ui::Scale9Sprite*, cocos2d::ui::Scale9Sprite*, cocos2d::ui::Scale9Sprite*>(L);
			std::get<0>(t)->initWithSizeAndBackgroundSprite({ std::get<1>(t), std::get<2>(t) }, std::get<3>(t), std::get<4>(t), std::get<5>(t));
			break;
		}
		default:
			return luaL_error(L, "initWithSizeAndBackgroundSprite error! need 4, 5 args: self, float sizeW, sizeH, string normal9SpriteBg, TextureResType texType = LOCAL          6 args: self, float sizeW, sizeH, Scale9Sprite normalSprite, pressedSprite, disabledSprite");
		}
		return 0;
	});

	Lua_NewFunc(L, "initWithSizeAndTexture", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, float, float, std::string>(L);
			std::get<0>(t)->initWithSizeAndTexture({ std::get<1>(t), std::get<2>(t) }, std::get<3>(t));
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, float, float, std::string, std::string>(L);
			std::get<0>(t)->initWithSizeAndTexture({ std::get<1>(t), std::get<2>(t) }, std::get<3>(t), std::get<4>(t));
			break;
		}
		case 6:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, float, float, std::string, std::string, std::string>(L);
			std::get<0>(t)->initWithSizeAndTexture({ std::get<1>(t), std::get<2>(t) }, std::get<3>(t), std::get<4>(t), std::get<5>(t));
			break;
		}
		case 7:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, float, float, std::string, std::string, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->initWithSizeAndTexture({ std::get<1>(t), std::get<2>(t) }, std::get<3>(t), std::get<4>(t), std::get<5>(t));
			break;
		}
		default:
			return luaL_error(L, "initWithSizeAndTexture error! need 4 ~ 7 args: self, float sizeW, sizeH, string normalImage, pressedImage, disabledImage, texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "loadTextures", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, std::string, std::string>(L);
			std::get<0>(t)->loadTextures(std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, std::string, std::string, std::string>(L);
			std::get<0>(t)->loadTextures(std::get<1>(t), std::get<2>(t), std::get<3>(t));
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, std::string, std::string, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->loadTextures(std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t));
			break;
		}
		default:
			return luaL_error(L, "loadTextures error! need 3 ~ 5 args: self, string normal, pressed, disabled, texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "loadTextureNormal", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, std::string>(L);
			std::get<0>(t)->loadTextureNormal(std::get<1>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->loadTextureNormal(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "loadTextureNormal error! need 2 ~ 3 args: self, string normal, texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "loadTexturePressed", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, std::string>(L);
			std::get<0>(t)->loadTexturePressed(std::get<1>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->loadTexturePressed(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "loadTexturePressed error! need 2 ~ 3 args: self, string pressed, texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "loadTextureDisabled", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, std::string>(L);
			std::get<0>(t)->loadTextureDisabled(std::get<1>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->loadTextureDisabled(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "loadTextureDisabled error! need 2 ~ 3 args: self, string disabled, texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "setCapInsets", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, float, float, float, float>(L, "setCapInsets error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setCapInsets({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "setCapInsetsNormalRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, float, float, float, float>(L, "setCapInsetsNormalRenderer error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setCapInsetsNormalRenderer({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getCapInsetsNormalRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getCapInsetsNormalRenderer error! need 1 args: self");
		var r = std::get<0>(t)->getCapInsetsNormalRenderer();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "setCapInsetsPressedRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, float, float, float, float>(L, "setCapInsetsPressedRenderer error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setCapInsetsPressedRenderer({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getCapInsetsPressedRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getCapInsetsPressedRenderer error! need 1 args: self");
		var r = std::get<0>(t)->getCapInsetsPressedRenderer();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "setCapInsetsDisabledRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, float, float, float, float>(L, "setCapInsetsDisabledRenderer error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setCapInsetsDisabledRenderer({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getCapInsetsDisabledRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getCapInsetsDisabledRenderer error! need 1 args: self");
		var r = std::get<0>(t)->getCapInsetsDisabledRenderer();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "setText", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, const char*>(L, "setText error! need 2 args: self, string text");
		std::get<0>(t)->setText(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getText", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getText error! need 1 args: self");
		var r = std::get<0>(t)->getText();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setFont", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, const char*, int>(L, "setFont error! need 3 args: self, string fontName, int fontSize");
		std::get<0>(t)->setFont(std::get<1>(t), std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "setFontName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, const char*>(L, "setFontName error! need 2 args: self, string fontName");
		std::get<0>(t)->setFontName(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getFontName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getFontName error! need 1 args: self");
		var r = std::get<0>(t)->getFontName();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setFontSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, int>(L, "setFontSize error! need 2 args: self, int fontSize");
		std::get<0>(t)->setFontSize(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getFontSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getFontSize error! need 1 args: self");
		var r = std::get<0>(t)->getFontSize();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setFontColor", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, GLubyte, GLubyte, GLubyte>(L);
			std::get<0>(t)->setFontColor({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, GLubyte, GLubyte, GLubyte, GLubyte>(L);
			std::get<0>(t)->setFontColor({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
			break;
		}
		default:
			return luaL_error(L, "setFontColor error! need 4 ~ 5 args: self, GLubyte r, g, b, a");
		}
		return 0;
	});

	Lua_NewFunc(L, "getFontColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getFontColor error! need 1 args: self");
		var r = std::get<0>(t)->getFontColor();
		return Lua_Pushs(L, r.r, r.g, r.b, r.a);
	});

	Lua_NewFunc(L, "setPlaceholderFont", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, const char*, int>(L, "setPlaceholderFont error! need 3 args: self, string fontName, int fontSize");
		std::get<0>(t)->setPlaceholderFont(std::get<1>(t), std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "setPlaceholderFontName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, const char*>(L, "setPlaceholderFontName error! need 2 args: self, string fontName");
		std::get<0>(t)->setPlaceholderFontName(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getPlaceholderFontName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getPlaceholderFontName error! need 1 args: self");
		var r = std::get<0>(t)->getPlaceholderFontName();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setPlaceholderFontSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, int>(L, "setPlaceholderFontSize error! need 2 args: self, int fontSize");
		std::get<0>(t)->setPlaceholderFontSize(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getPlaceholderFontSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getPlaceholderFontSize error! need 1 args: self");
		var r = std::get<0>(t)->getPlaceholderFontSize();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setPlaceholderFontColor", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, GLubyte, GLubyte, GLubyte>(L);
			std::get<0>(t)->setPlaceholderFontColor({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::ui::EditBox*, GLubyte, GLubyte, GLubyte, GLubyte>(L);
			std::get<0>(t)->setPlaceholderFontColor({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
			break;
		}
		default:
			return luaL_error(L, "setPlaceholderFontColor error! need 4 ~ 5 args: self, GLubyte r, g, b, a");
		}
		return 0;
	});

	Lua_NewFunc(L, "getPlaceholderFontColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getPlaceholderFontColor error! need 1 args: self");
		var r = std::get<0>(t)->getPlaceholderFontColor();
		return Lua_Pushs(L, r.r, r.g, r.b, r.a);
	});

	Lua_NewFunc(L, "setPlaceHolder", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, const char*>(L, "setPlaceHolder error! need 2 args: self, string txt");
		std::get<0>(t)->setPlaceHolder(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getPlaceHolder", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getPlaceHolder error! need 1 args: self");
		var r = std::get<0>(t)->getPlaceHolder();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setInputMode", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, cocos2d::ui::EditBox::InputMode>(L, "setInputMode error! need 2 args: self, InputMode inputMode");
		std::get<0>(t)->setInputMode(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getInputMode", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getInputMode error! need 1 args: self");
		var r = std::get<0>(t)->getInputMode();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setMaxLength", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, int>(L, "setMaxLength error! need 2 args: self, int maxLength");
		std::get<0>(t)->setMaxLength(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getMaxLength", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getMaxLength error! need 1 args: self");
		var r = std::get<0>(t)->getMaxLength();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setInputFlag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, cocos2d::ui::EditBox::InputFlag>(L, "setInputFlag error! need 2 args: self, InputFlag inputFlag");
		std::get<0>(t)->setInputFlag(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getInputFlag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getInputFlag error! need 1 args: self");
		var r = std::get<0>(t)->getInputFlag();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setReturnType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, cocos2d::ui::EditBox::KeyboardReturnType>(L, "setReturnType error! need 2 args: self, KeyboardReturnType returnType");
		std::get<0>(t)->setReturnType(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getReturnType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getReturnType error! need 1 args: self");
		var r = std::get<0>(t)->getReturnType();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setTextHorizontalAlignment", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*, cocos2d::TextHAlignment>(L, "setTextHorizontalAlignment error! need 2 args: self, TextHAlignment alignment");
		std::get<0>(t)->setTextHorizontalAlignment(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getTextHorizontalAlignment", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "getTextHorizontalAlignment error! need 1 args: self");
		var r = std::get<0>(t)->getTextHorizontalAlignment();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "openKeyboard", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::EditBox*>(L, "openKeyboard error! need 1 args: self");
		std::get<0>(t)->openKeyboard();
		return 0;
	});

	lua_pop(L, 1);

	lua_pushstring(L, TypeNames<cocos2d::ui::EditBox::InputMode>::value);
	lua_createtable(L, 7, 0);
	lua_pushstring(L, "ANY");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputMode::ANY);	lua_rawset(L, -3);
	lua_pushstring(L, "EMAIL_ADDRESS");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);	lua_rawset(L, -3);
	lua_pushstring(L, "NUMERIC");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputMode::NUMERIC);	lua_rawset(L, -3);
	lua_pushstring(L, "PHONE_NUMBER");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);	lua_rawset(L, -3);
	lua_pushstring(L, "URL");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputMode::URL);	lua_rawset(L, -3);
	lua_pushstring(L, "DECIMAL");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputMode::DECIMAL);	lua_rawset(L, -3);
	lua_pushstring(L, "SINGLE_LINE");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputMode::SINGLE_LINE);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, TypeNames<cocos2d::ui::EditBox::InputFlag>::value);
	lua_createtable(L, 6, 0);
	lua_pushstring(L, "PASSWORD");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputFlag::PASSWORD);	lua_rawset(L, -3);
	lua_pushstring(L, "SENSITIVE");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputFlag::SENSITIVE);	lua_rawset(L, -3);
	lua_pushstring(L, "INITIAL_CAPS_WORD");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_WORD);	lua_rawset(L, -3);
	lua_pushstring(L, "INITIAL_CAPS_SENTENCE");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);	lua_rawset(L, -3);
	lua_pushstring(L, "INITIAL_CAPS_ALL_CHARACTERS");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);	lua_rawset(L, -3);
	lua_pushstring(L, "LOWERCASE_ALL_CHARACTERS");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::InputFlag::LOWERCASE_ALL_CHARACTERS);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, TypeNames<cocos2d::ui::EditBox::KeyboardReturnType>::value);
	lua_createtable(L, 6, 0);
	lua_pushstring(L, "DEFAULT");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);	lua_rawset(L, -3);
	lua_pushstring(L, "DONE");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::KeyboardReturnType::DONE);	lua_rawset(L, -3);
	lua_pushstring(L, "SEND");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::KeyboardReturnType::SEND);	lua_rawset(L, -3);
	lua_pushstring(L, "SEARCH");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::KeyboardReturnType::SEARCH);	lua_rawset(L, -3);
	lua_pushstring(L, "GO");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::KeyboardReturnType::GO);	lua_rawset(L, -3);
	lua_pushstring(L, "NEXT");	lua_pushinteger(L, (int)cocos2d::ui::EditBox::KeyboardReturnType::NEXT);	lua_rawset(L, -3);
	lua_rawset(L, -3);
