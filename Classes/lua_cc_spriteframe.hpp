#pragma once

inline void Lua_Register_SpriteFrame(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::SpriteFrame*>::value, TypeNames<cocos2d::Ref*>::value);

	Lua_NewFunc(L, "initWithTexture", [](lua_State* L)
	{
		cocos2d::SpriteFrame* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 6:
		{
			var t = Lua_ToTuple<cocos2d::SpriteFrame*, cocos2d::Texture2D*, float, float, float, float>(L);
			std::get<0>(t)->initWithTexture(std::get<1>(t), { std::get<2>(t),std::get<3>(t),std::get<4>(t),std::get<5>(t) });
			break;
		}
		case 11:
		{
			var t = Lua_ToTuple<cocos2d::SpriteFrame*, cocos2d::Texture2D*, float, float, float, float, bool, float, float, float, float>(L);
			std::get<0>(t)->initWithTexture(std::get<1>(t), { std::get<2>(t),std::get<3>(t),std::get<4>(t),std::get<5>(t) }, std::get<6>(t), { std::get<7>(t), std::get<8>(t) }, { std::get<9>(t), std::get<10>(t) });
			break;
		}
		default:
			return luaL_error(L, "%s", "initWithTexture error! need 6, 11 args: self, Texture2D texture, float rectX, rectY, rectW, rectH, bool rotated, float offsetX, offsetY, originalSizeW, originalSizeH");
		}
		return 0;
	});

	Lua_NewFunc(L, "initWithTextureFilename", [](lua_State* L)
	{
		cocos2d::SpriteFrame* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 6:
		{
			var t = Lua_ToTuple<cocos2d::SpriteFrame*, std::string, float, float, float, float>(L);
			std::get<0>(t)->initWithTextureFilename(std::get<1>(t), { std::get<2>(t),std::get<3>(t),std::get<4>(t),std::get<5>(t) });
			break;
		}
		case 11:
		{
			var t = Lua_ToTuple<cocos2d::SpriteFrame*, std::string, float, float, float, float, bool, float, float, float, float>(L);
			std::get<0>(t)->initWithTextureFilename(std::get<1>(t), { std::get<2>(t),std::get<3>(t),std::get<4>(t),std::get<5>(t) }, std::get<6>(t), { std::get<7>(t), std::get<8>(t) }, { std::get<9>(t), std::get<10>(t) });
			break;
		}
		default:
			return luaL_error(L, "%s", "initWithTextureFilename error! need 6, 11 args: self, string filename, float rectX, rectY, rectW, rectH, bool rotated, float offsetX, offsetY, originalSizeW, originalSizeH");
		}
		return 0;
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::SpriteFrame* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 5:
		{
			var t = Lua_ToTuple<std::string, float, float, float, float>(L);
			o = cocos2d::SpriteFrame::create(std::get<0>(t), { std::get<1>(t),std::get<2>(t),std::get<3>(t),std::get<4>(t) });
			break;
		}
		case 10:
		{
			var t = Lua_ToTuple<std::string, float, float, float, float, bool, float, float, float, float>(L);
			o = cocos2d::SpriteFrame::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) }, std::get<5>(t), { std::get<6>(t), std::get<7>(t) }, { std::get<8>(t), std::get<9>(t) });
			break;
		}
		default:
			return luaL_error(L, "%s", "create SpriteFrame error! need 5, 10 args: string filename, float rectX, rectY, rectW, rectH, bool rotated, float offsetX, offsetY, originalSizeW, originalSizeH");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "createWithTexture", [](lua_State* L)
	{
		cocos2d::SpriteFrame* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::Texture2D*, float, float, float, float>(L);
			o = cocos2d::SpriteFrame::createWithTexture(std::get<0>(t), { std::get<1>(t),std::get<2>(t),std::get<3>(t),std::get<4>(t) });
			break;
		}
		case 10:
		{
			var t = Lua_ToTuple<cocos2d::Texture2D*, float, float, float, float, bool, float, float, float, float>(L);
			o = cocos2d::SpriteFrame::createWithTexture(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) }, std::get<5>(t), { std::get<6>(t), std::get<7>(t) }, { std::get<8>(t), std::get<9>(t) });
			break;
		}
		default:
			return luaL_error(L, "%s", "create createWithTexture error! need 5, 10 args: Texture2D texture, float rectX, rectY, rectW, rectH, bool rotated, float offsetX, offsetY, originalSizeW, originalSizeH");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "getRectInPixels", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "getRectInPixels error! need 1 args: self");
		var r = std::get<0>(t)->getRectInPixels();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "setRectInPixels", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*, float, float, float, float>(L, "setRectInPixels error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setRectInPixels({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "isRotated", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "isRotated error! need 1 args: self");
		var r = std::get<0>(t)->isRotated();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setRotated", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*, bool>(L, "setRotated error! need 2 args: self, bool rotated");
		std::get<0>(t)->setRotated(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getRect", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "getRect error! need 1 args: self");
		var r = std::get<0>(t)->getRect();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "setRect", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*, float, float, float, float>(L, "setRect error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setRect({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getCenterRect", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "getCenterRect error! need 1 args: self");
		var r = std::get<0>(t)->getCenterRect();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "setCenterRectInPixels", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*, float, float, float, float>(L, "setCenterRectInPixels error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setCenterRectInPixels({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "hasCenterRect", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "hasCenterRect error! need 1 args: self");
		var r = std::get<0>(t)->hasCenterRect();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getOffsetInPixels", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "getOffsetInPixels error! need 1 args: self");
		var r = std::get<0>(t)->getOffsetInPixels();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setOffsetInPixels", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*, float, float>(L, "setOffsetInPixels error! need 3 args: self, float offsetInPixelsX, offsetInPixelsY");
		std::get<0>(t)->setOffsetInPixels({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getOriginalSizeInPixels", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "getOriginalSizeInPixels error! need 1 args: self");
		var r = std::get<0>(t)->getOriginalSizeInPixels();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "setOriginalSizeInPixels", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*, float, float>(L, "setOriginalSizeInPixels error! need 3 args: self, float sizeInPixelsX, sizeInPixelsY");
		std::get<0>(t)->setOriginalSizeInPixels({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getOriginalSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "getOriginalSize error! need 1 args: self");
		var r = std::get<0>(t)->getOriginalSize();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "setOriginalSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*, float, float>(L, "setOriginalSize error! need 3 args: self, float sizeInPixelsX, sizeInPixelsY");
		std::get<0>(t)->setOriginalSize({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getTexture", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "getTexture error! need 1 args: self");
		var r = std::get<0>(t)->getTexture();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setTexture", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*, cocos2d::Texture2D*>(L, "setTexture error! need 3 args: self, Texture2D texture");
		std::get<0>(t)->setTexture(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getOffset", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "getOffset error! need 1 args: self");
		var r = std::get<0>(t)->getOffset();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setOffset", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*, float, float>(L, "setOffset error! need 3 args: self, float offsetsX, offsetsY");
		std::get<0>(t)->setOffset({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getAnchorPoint", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "getAnchorPoint error! need 1 args: self");
		var r = std::get<0>(t)->getAnchorPoint();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setAnchorPoint", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*, float, float>(L, "setAnchorPoint error! need 3 args: self, float offsetsX, offsetsY");
		std::get<0>(t)->setAnchorPoint({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "hasAnchorPoint", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "hasAnchorPoint error! need 1 args: self");
		var r = std::get<0>(t)->hasAnchorPoint();
		return Lua_Pushs(L, r);
	});

	// setPolygonInfo, getPolygonInfo, hasPolygonInfo


	lua_pop(L, 1);
}
