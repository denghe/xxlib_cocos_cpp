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

	Lua_NewFunc(L, "enableShadow", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::Label*>(L);
			std::get<0>(t)->enableShadow();
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::Label*, GLubyte, GLubyte, GLubyte, GLubyte>(L);
			std::get<0>(t)->enableShadow({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
			break;
		}
		case 7:
		{
			var t = Lua_ToTuple<cocos2d::Label*, GLubyte, GLubyte, GLubyte, GLubyte, float, float>(L);
			std::get<0>(t)->enableShadow({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) }, { std::get<5>(t), std::get<6>(t) });
			break;
		}
		case 8:
		{
			var t = Lua_ToTuple<cocos2d::Label*, GLubyte, GLubyte, GLubyte, GLubyte, float, float, int>(L);
			std::get<0>(t)->enableShadow({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) }, { std::get<5>(t), std::get<6>(t) }, std::get<7>(t));
			break;
		}
		default:
			return luaL_error(L, "%s", "enableShadow error! need 1, 5 ~ 8 args: self, GLubyte r = 0, g = 0, b = 0, a = 0, float offsetW = 2, offsetH = -2, int blurRadius = 0");
		}
		return 0;
	});

	Lua_NewFunc(L, "enableOutline", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::Label*, GLubyte, GLubyte, GLubyte, GLubyte>(L);
			std::get<0>(t)->enableOutline({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
			break;
		}
		case 6:
		{
			var t = Lua_ToTuple<cocos2d::Label*, GLubyte, GLubyte, GLubyte, GLubyte, int>(L);
			std::get<0>(t)->enableOutline({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) }, std::get<5>(t));
			break;
		}
		default:
			return luaL_error(L, "%s", "enableOutline error! need 5 ~ 6 args: self, GLubyte r = 0, g = 0, b = 0, a = 0, int outlineSize = -1");
		}
		return 0;
	});

	Lua_NewFunc(L, "enableGlow", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, GLubyte, GLubyte, GLubyte, GLubyte>(L, "enableGlow error! need 5 args: self, GLubyte r, g, b, a");
		std::get<0>(t)->enableGlow({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "enableItalics", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L);
		std::get<0>(t)->enableItalics();
		return 0;
	});

	Lua_NewFunc(L, "enableBold", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L);
		std::get<0>(t)->enableBold();
		return 0;
	});

	Lua_NewFunc(L, "enableUnderline", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L);
		std::get<0>(t)->enableUnderline();
		return 0;
	});

	Lua_NewFunc(L, "enableStrikethrough", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L);
		std::get<0>(t)->enableStrikethrough();
		return 0;
	});

	Lua_NewFunc(L, "disableEffect", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::Label*>(L);
			std::get<0>(t)->disableEffect();
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Label*, cocos2d::LabelEffect>(L);
			std::get<0>(t)->disableEffect(std::get<1>(t));
			break;
		}
		default:
			return luaL_error(L, "%s", "disableEffect error! need 1 ~ 2 args: self, LabelEffect effect = ALL");
		}
		return 0;
	});

	Lua_NewFunc(L, "isShadowEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "isShadowEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isShadowEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getShadowOffset", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getShadowOffset error! need 1 args: self");
		var r = std::get<0>(t)->getShadowOffset();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "getShadowBlurRadius", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getShadowBlurRadius error! need 1 args: self");
		var r = std::get<0>(t)->getShadowBlurRadius();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getShadowColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getShadowColor error! need 1 args: self");
		var r = std::get<0>(t)->getShadowColor();
		return Lua_Pushs(L, r.r, r.g, r.b, r.a);
	});

	Lua_NewFunc(L, "getOutlineSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getOutlineSize error! need 1 args: self");
		var r = std::get<0>(t)->getOutlineSize();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getLabelEffectType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getLabelEffectType error! need 1 args: self");
		var r = std::get<0>(t)->getLabelEffectType();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getEffectColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getEffectColor error! need 1 args: self");
		var r = std::get<0>(t)->getEffectColor();
		return Lua_Pushs(L, r.r, r.g, r.b, r.a);
	});

	Lua_NewFunc(L, "setAlignment", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Label*, cocos2d::TextHAlignment>(L);
			std::get<0>(t)->setAlignment(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::Label*, cocos2d::TextHAlignment, cocos2d::TextVAlignment>(L);
			std::get<0>(t)->setAlignment(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "%s", "setAlignment error! need 2 ~ 3 args: self, TextHAlignment hAlignment, TextVAlignment vAlignment");
		}
		return 0;
	});

	Lua_NewFunc(L, "getTextAlignment", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getTextAlignment error! need 1 args: self");
		var r = std::get<0>(t)->getTextAlignment();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setHorizontalAlignment", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, cocos2d::TextHAlignment>(L, "setHorizontalAlignment error! need 2 args: self, TextHAlignment hAlignment");
		std::get<0>(t)->setHorizontalAlignment(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getHorizontalAlignment", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getHorizontalAlignment error! need 1 args: self");
		var r = std::get<0>(t)->getHorizontalAlignment();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setVerticalAlignment", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, cocos2d::TextVAlignment>(L, "setVerticalAlignment error! need 2 args: self, TextVAlignment vAlignment");
		std::get<0>(t)->setVerticalAlignment(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getVerticalAlignment", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getVerticalAlignment error! need 1 args: self");
		var r = std::get<0>(t)->getVerticalAlignment();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setLineBreakWithoutSpace", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, bool>(L, "setLineBreakWithoutSpace error! need 2 args: self, bool breakWithoutSpace");
		std::get<0>(t)->setLineBreakWithoutSpace(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setMaxLineWidth", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, float>(L, "setMaxLineWidth error! need 2 args: self, float maxLineWidth");
		std::get<0>(t)->setMaxLineWidth(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getMaxLineWidth", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getMaxLineWidth error! need 1 args: self");
		var r = std::get<0>(t)->getMaxLineWidth();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setBMFontSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, float>(L, "setBMFontSize error! need 2 args: self, float fontSize");
		std::get<0>(t)->setBMFontSize(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getBMFontSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getBMFontSize error! need 1 args: self");
		var r = std::get<0>(t)->getBMFontSize();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "enableWrap", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, bool>(L, "enableWrap error! need 2 args: self, bool enable");
		std::get<0>(t)->enableWrap(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isWrapEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "isWrapEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isWrapEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setOverflow", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, cocos2d::Label::Overflow>(L, "setOverflow error! need 2 args: self, Overflow overflow");
		std::get<0>(t)->setOverflow(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getOverflow", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getOverflow error! need 1 args: self");
		var r = std::get<0>(t)->getOverflow();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setWidth", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, float>(L, "setWidth error! need 2 args: self, float width");
		std::get<0>(t)->setWidth(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getWidth", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getWidth error! need 1 args: self");
		var r = std::get<0>(t)->getWidth();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setHeight", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, float>(L, "setHeight error! need 2 args: self, float height");
		std::get<0>(t)->setHeight(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getHeight", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getHeight error! need 1 args: self");
		var r = std::get<0>(t)->getHeight();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setDimensions", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, float, float>(L, "setDimensions error! need 2 args: self, float width, height");
		std::get<0>(t)->setDimensions(std::get<1>(t), std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "getDimensions", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getDimensions error! need 1 args: self");
		var r = std::get<0>(t)->getDimensions();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "getLetter", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, int>(L, "getLetter error! need 2 args: self, int letterIndex");
		var r = std::get<0>(t)->getLetter(std::get<1>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setClipMarginEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, bool>(L, "setClipMarginEnabled error! need 2 args: self, bool clipEnabled");
		std::get<0>(t)->setClipMarginEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isClipMarginEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "isClipMarginEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isClipMarginEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setLineHeight", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, float>(L, "setLineHeight error! need 2 args: self, float height");
		std::get<0>(t)->setLineHeight(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getLineHeight", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getLineHeight error! need 1 args: self");
		var r = std::get<0>(t)->getLineHeight();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setLineSpacing", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, float>(L, "setLineSpacing error! need 2 args: self, float height");
		std::get<0>(t)->setLineSpacing(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getLineSpacing", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getLineSpacing error! need 1 args: self");
		var r = std::get<0>(t)->getLineSpacing();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getLabelType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getLabelType error! need 1 args: self");
		var r = std::get<0>(t)->getLabelType();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getRenderingFontSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getRenderingFontSize error! need 1 args: self");
		var r = std::get<0>(t)->getRenderingFontSize();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setAdditionalKerning", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*, float>(L, "setAdditionalKerning error! need 2 args: self, float space");
		std::get<0>(t)->setAdditionalKerning(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getAdditionalKerning", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Label*>(L, "getAdditionalKerning error! need 1 args: self");
		var r = std::get<0>(t)->getAdditionalKerning();
		return Lua_Pushs(L, r);
	});

	// getFontAtlas



	lua_pop(L, 1);

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

	lua_pushstring(L, "LabelEffect");
	lua_createtable(L, 9, 0);
	lua_pushstring(L, "NORMAL");	lua_pushinteger(L, (int)cocos2d::LabelEffect::NORMAL);	lua_rawset(L, -3);
	lua_pushstring(L, "OUTLINE");	lua_pushinteger(L, (int)cocos2d::LabelEffect::OUTLINE);	lua_rawset(L, -3);
	lua_pushstring(L, "SHADOW");	lua_pushinteger(L, (int)cocos2d::LabelEffect::SHADOW);	lua_rawset(L, -3);
	lua_pushstring(L, "GLOW");	lua_pushinteger(L, (int)cocos2d::LabelEffect::GLOW);	lua_rawset(L, -3);
	lua_pushstring(L, "ITALICS");	lua_pushinteger(L, (int)cocos2d::LabelEffect::ITALICS);	lua_rawset(L, -3);
	lua_pushstring(L, "BOLD");	lua_pushinteger(L, (int)cocos2d::LabelEffect::BOLD);	lua_rawset(L, -3);
	lua_pushstring(L, "UNDERLINE");	lua_pushinteger(L, (int)cocos2d::LabelEffect::UNDERLINE);	lua_rawset(L, -3);
	lua_pushstring(L, "STRIKETHROUGH");	lua_pushinteger(L, (int)cocos2d::LabelEffect::STRIKETHROUGH);	lua_rawset(L, -3);
	lua_pushstring(L, "ALL");	lua_pushinteger(L, (int)cocos2d::LabelEffect::ALL);	lua_rawset(L, -3);
	lua_rawset(L, -3);

}
