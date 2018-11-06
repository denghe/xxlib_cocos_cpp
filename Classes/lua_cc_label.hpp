#pragma once

inline void Lua_Register_Label(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Label*>::value, TypeNames<cocos2d::Node*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var r = cocos2d::Label::create();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "createWithSystemFont", [](lua_State* L)
	{
		cocos2d::Label* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			var t = Lua_ToTuple<std::string, std::string, float>(L);
			o = cocos2d::Label::createWithSystemFont(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<std::string, std::string, float, float, float>(L);
			o = cocos2d::Label::createWithSystemFont(std::get<0>(t), std::get<1>(t), std::get<2>(t), { std::get<3>(t), std::get<4>(t) });
			break;
		}
		case 6:
		{
			var t = Lua_ToTuple<std::string, std::string, float, float, float, cocos2d::TextHAlignment>(L);
			o = cocos2d::Label::createWithSystemFont(std::get<0>(t), std::get<1>(t), std::get<2>(t), { std::get<3>(t), std::get<4>(t) }, std::get<5>(t));
			break;
		}
		case 7:
		{
			var t = Lua_ToTuple<std::string, std::string, float, float, float, cocos2d::TextHAlignment, cocos2d::TextVAlignment>(L);
			o = cocos2d::Label::createWithSystemFont(std::get<0>(t), std::get<1>(t), std::get<2>(t), { std::get<3>(t), std::get<4>(t) }, std::get<5>(t), std::get<6>(t));
			break;
		}
		default:
			return luaL_error(L, "%s", "createWithSystemFont Label error! need 3, 5 ~ 7 args: string text, font, float fontSize, dimensionsX = 0, dimensionsY = 0, TextHAlignment hAlignment = TextHAlignment::LEFT, TextVAlignment vAlignment = TextVAlignment::TOP");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "createWithTTF", [](lua_State* L)
	{
		cocos2d::Label* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			var t = Lua_ToTuple<std::string, std::string, float>(L);
			o = cocos2d::Label::createWithTTF(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<std::string, std::string, float, float, float>(L);
			o = cocos2d::Label::createWithTTF(std::get<0>(t), std::get<1>(t), std::get<2>(t), { std::get<3>(t), std::get<4>(t) });
			break;
		}
		case 6:
		{
			var t = Lua_ToTuple<std::string, std::string, float, float, float, cocos2d::TextHAlignment>(L);
			o = cocos2d::Label::createWithTTF(std::get<0>(t), std::get<1>(t), std::get<2>(t), { std::get<3>(t), std::get<4>(t) }, std::get<5>(t));
			break;
		}
		case 7:
		{
			var t = Lua_ToTuple<std::string, std::string, float, float, float, cocos2d::TextHAlignment, cocos2d::TextVAlignment>(L);
			o = cocos2d::Label::createWithTTF(std::get<0>(t), std::get<1>(t), std::get<2>(t), { std::get<3>(t), std::get<4>(t) }, std::get<5>(t), std::get<6>(t));
			break;
		}
		default:
			return luaL_error(L, "%s", "createWithTTF Label error! need 3, 5 ~ 7 args: string text, fontFilePath, float fontSize, dimensionsX = 0, dimensionsY = 0, TextHAlignment hAlignment = TextHAlignment::LEFT, TextVAlignment vAlignment = TextVAlignment::TOP");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	// todo: createWithTTFConfig

	Lua_NewFunc(L, "createWithBMFont", [](lua_State* L)
	{
		cocos2d::Label* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			var t = Lua_ToTuple<std::string, std::string>(L);
			o = cocos2d::Label::createWithBMFont(std::get<0>(t), std::get<1>(t));
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<std::string, std::string, cocos2d::TextHAlignment>(L);
			o = cocos2d::Label::createWithBMFont(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			break;
		}
		case 6:
		{
			var t = Lua_ToTuple<std::string, std::string, cocos2d::TextHAlignment, int>(L);
			o = cocos2d::Label::createWithBMFont(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
			break;
		}
		case 7:
		{
			var t = Lua_ToTuple<std::string, std::string, cocos2d::TextHAlignment, int, float, float>(L);
			o = cocos2d::Label::createWithBMFont(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t), { std::get<4>(t) , std::get<5>(t) });
			break;
		}
		default:
			return luaL_error(L, "%s", "createWithBMFont Label error! need 2 ~ 4, 6 args: string bmfontPath, text, TextHAlignment hAlignment = TextHAlignment::LEFT, int maxLineWidth = 0,	float imageOffsetX, imageOffsetY");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	// todo: createWithCharMap, setTTFConfig, getTTFConfig .......


	Lua_NewFunc(L, "setString", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, std::string>(L, "setString error! need 2 args: self, string txt");
		std::get<0>(t)->setString(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getString", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getString error! need 1 args: self");
		var r = std::get<0>(t)->getString();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getStringNumLines", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getStringNumLines error! need 1 args: self");
		var r = std::get<0>(t)->getStringNumLines();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getStringLength", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getStringLength error! need 1 args: self");
		var r = std::get<0>(t)->getStringLength();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setTextColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, GLubyte, GLubyte, GLubyte, GLubyte>(L, "setTextColor error! need 4 args: self, GLubyte r, GLubyte g, GLubyte b, GLubyte a");
		std::get<0>(t)->setTextColor({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});
	Lua_NewFunc(L, "getTextColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getTextColor error! need 1 args: self");
		var r = std::get<0>(t)->getTextColor();
		return Lua_Pushs(L, r.r, r.g, r.b, r.a);
	});


	lua_pop(L, 1);													// cc

	lua_pushstring(L, "Overflow");
	lua_createtable(L, 4, 0);
	lua_pushstring(L, "NONE");	lua_pushinteger(L, (int)cocos2d::Label::Overflow::NONE);	lua_rawset(L, -3);
	lua_pushstring(L, "CLAMP");	lua_pushinteger(L, (int)cocos2d::Label::Overflow::CLAMP);	lua_rawset(L, -3);
	lua_pushstring(L, "SHRINK");	lua_pushinteger(L, (int)cocos2d::Label::Overflow::SHRINK);	lua_rawset(L, -3);
	lua_pushstring(L, "RESIZE_HEIGHT");	lua_pushinteger(L, (int)cocos2d::Label::Overflow::RESIZE_HEIGHT);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, "LabelType");
	lua_createtable(L, 4, 0);
	lua_pushstring(L, "TTF");	lua_pushinteger(L, (int)cocos2d::Label::LabelType::TTF);	lua_rawset(L, -3);
	lua_pushstring(L, "BMFONT");	lua_pushinteger(L, (int)cocos2d::Label::LabelType::BMFONT);	lua_rawset(L, -3);
	lua_pushstring(L, "CHARMAP");	lua_pushinteger(L, (int)cocos2d::Label::LabelType::CHARMAP);	lua_rawset(L, -3);
	lua_pushstring(L, "STRING_TEXTURE");	lua_pushinteger(L, (int)cocos2d::Label::LabelType::STRING_TEXTURE);	lua_rawset(L, -3);
	lua_rawset(L, -3);

}
