#pragma once

inline void Lua_Register_Sprite(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Sprite*>::value, TypeNames<cocos2d::Node*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::Sprite();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "initWithTexture", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*, cocos2d::Texture2D*>(L, "initWithTexture error! need 2 args: self, texture");
		std::get<0>(t)->initWithTexture(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "initWithFile", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*, std::string>(L, "initWithFile error! need 2 args: self, fileName");
		std::get<0>(t)->initWithFile(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "initWithSpriteFrameName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*, std::string>(L, "initWithSpriteFrameName error! need 2 args: self, spriteFrameName");
		std::get<0>(t)->initWithSpriteFrameName(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "initWithSpriteFrame", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*, cocos2d::SpriteFrame*>(L, "initWithSpriteFrame error! need 2 args: self, spriteFrame");
		std::get<0>(t)->initWithSpriteFrame(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::Sprite* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 0:
		{
			o = cocos2d::Sprite::create();
			break;
		}
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			o = cocos2d::Sprite::create(std::get<0>(t));
			break;
		}
		default:
			return luaL_error(L, "create Node error! need 0 ~ 1 args: string imageFileName");
		}
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "createWithSpriteFrame", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "createWithSpriteFrame error! need 1 args: spriteFrame");
		var o = cocos2d::Sprite::createWithSpriteFrame(std::get<0>(t));
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "createWithSpriteFrameName", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "createWithSpriteFrameName error! need 1 args: sprite frame name");
		var o = cocos2d::Sprite::createWithSpriteFrameName(std::get<0>(t));
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "createWithTexture", [](lua_State* L)
	{
		cocos2d::Sprite* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::Texture2D*>(L);
			o = cocos2d::Sprite::createWithTexture(std::get<0>(t));
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::Texture2D*, float, float, float, float>(L);
			o = cocos2d::Sprite::createWithTexture(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
			break;
		}
		case 6:
		{
			var t = Lua_ToTuple<cocos2d::Texture2D*, float, float, float, float, bool>(L);
			o = cocos2d::Sprite::createWithTexture(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) }, std::get<5>(t));
			break;
		}
		default:
			return luaL_error(L, "create Node error! need 1, 5, 6 args: Texture2D texture, float rectX, rectY, rectW, rectH, bool rotated = false");
		}
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "setSpriteFrame", [](lua_State* L)
	{
		var msg = "setSpriteFrame error! need 2 args: self, spriteFrame / string spriteFrameName";
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			if (lua_isstring(L, 1))
			{
				var t = Lua_ToTuple<cocos2d::Sprite*, std::string>(L);
				std::get<0>(t)->setSpriteFrame(std::get<1>(t));
			}
			else
			{
				var t = Lua_ToTuple<cocos2d::Sprite*, cocos2d::SpriteFrame*>(L, msg);
				std::get<0>(t)->setSpriteFrame(std::get<1>(t));
			}
			break;
		}
		default:
			return luaL_error(L, "%s", msg);
		}
		return 0;
	});

	Lua_NewFunc(L, "setTexture", [](lua_State* L)
	{
		var msg = "setTexture error! need 2 args: self, Texture2D texture / string fileName";
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			if (lua_isstring(L, 1))
			{
				var t = Lua_ToTuple<cocos2d::Sprite*, std::string>(L);
				std::get<0>(t)->setTexture(std::get<1>(t));
			}
			else
			{
				var t = Lua_ToTuple<cocos2d::Sprite*, cocos2d::Texture2D*>(L, msg);
				std::get<0>(t)->setTexture(std::get<1>(t));
			}
			break;
		}
		default:
			return luaL_error(L, "%s", msg);
		}
		return 0;
	});

	Lua_NewFunc(L, "isFrameDisplayed", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*, cocos2d::SpriteFrame*>(L, "isFrameDisplayed error! need 2 args: self, SpriteFrame frame");
		var r = std::get<0>(t)->isFrameDisplayed(std::get<1>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getSpriteFrame", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*>(L, "getSpriteFrame error! need 1 args: self");
		var r = std::get<0>(t)->getSpriteFrame();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setDisplayFrameWithAnimationName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*, std::string, int>(L, "setDisplayFrameWithAnimationName error! need 3 args: self, string animationName, ssize_t frameIndex");
		std::get<0>(t)->setDisplayFrameWithAnimationName(std::get<1>(t), std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "isFlippedX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*>(L, "isFlippedX error! need 1 args: self");
		var r = std::get<0>(t)->isFlippedX();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setFlippedX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*, bool>(L, "setFlippedX error! need 2 args: self, bool flippedX");
		std::get<0>(t)->setFlippedX(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isFlippedY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*>(L, "isFlippedY error! need 1 args: self");
		var r = std::get<0>(t)->isFlippedY();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setFlippedY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*, bool>(L, "setFlippedY error! need 2 args: self, bool flippedY");
		std::get<0>(t)->setFlippedY(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isStretchEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*>(L, "isStretchEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isStretchEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setStretchEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*, bool>(L, "setStretchEnabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setStretchEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getResourceType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*>(L, "getResourceType error! need 1 args: self");
		var r = std::get<0>(t)->getResourceType();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getResourceName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*>(L, "getResourceName error! need 1 args: self");
		var r = std::get<0>(t)->getResourceName();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setBlendFunc", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::ImageView*, GLenum, GLenum>(L, "setBlendFunc error! need 3 args: self, GLenum src, dst");
		std::get<0>(t)->setBlendFunc({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getBlendFunc", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*>(L, "getBlendFunc error! need 1 args: self");
		var r = std::get<0>(t)->getBlendFunc();
		return Lua_Pushs(L, r.src, r.dst);
	});








	// 增加一组易于使用的函数. 大写开头. 和 cocos 本体函数以示区别

	Lua_NewFunc(L, "Create_FileName_Owner_Positon_Anchor", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, cocos2d::Node*, float, float, float, float>
			(L, "Create_FileName_Owner_Positon_Anchor error! need 6 args: owner, fileName, px, py, ax, ay");

		var o = cocos2d::Sprite::create(std::get<0>(t));
		if (!o) return 0;
		Lua_Pushs(L, o);
		std::get<1>(t)->addChild(o);
		o->setPosition(std::get<2>(t), std::get<3>(t));
		o->setAnchorPoint({ std::get<4>(t), std::get<5>(t) });
		return 1;
	});

	lua_pop(L, 1);
}
