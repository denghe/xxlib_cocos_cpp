#pragma once

inline void Lua_Register_Sprite(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_Sprite, LuaKey_Node);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::Sprite();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_NewUserdataMT(L, o, LuaKey_Sprite);
	});

	Lua_NewFunc(L, "initWithTexture", [](lua_State* L)
	{
		var t = Lua_ToTT<cocos2d::Sprite*, cocos2d::Texture2D*>(L, "initWithTexture error! need 2 args: self, texture", LuaKey_Sprite, LuaKey_Texture);
		std::get<0>(t)->initWithTexture(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "initWithFileName", [](lua_State* L)
	{
		var t = Lua_ToTs<cocos2d::Sprite*>(L, "initWithFile error! need 2 args: self, fileName", LuaKey_Sprite);
		std::get<0>(t)->initWithFile(std::string(std::get<1>(t).first, std::get<1>(t).second));
		return 0;
	});

	Lua_NewFunc(L, "initWithSpriteFrameName", [](lua_State* L)
	{
		var t = Lua_ToTs<cocos2d::Sprite*>(L, "initWithSpriteFrameName error! need 2 args: self, spriteFrameName", LuaKey_Sprite);
		std::get<0>(t)->initWithSpriteFrameName(std::string(std::get<1>(t).first, std::get<1>(t).second));
		return 0;
	});

	Lua_NewFunc(L, "initWithSpriteFrame", [](lua_State* L)
	{
		var t = Lua_ToTT<cocos2d::Sprite*, cocos2d::SpriteFrame*>(L, "initWithSpriteFrame error! need 2 args: self, spriteFrame", LuaKey_Sprite, LuaKey_SpriteFrame);
		std::get<0>(t)->initWithSpriteFrame(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var o = cocos2d::Sprite::create();
		if (!o) return 0;
		return Lua_NewUserdataMT(L, o, LuaKey_Sprite);
	});

	Lua_NewFunc(L, "createWithFileName", [](lua_State* L)
	{
		var t = Lua_Tos(L, "createWithFileName error! need 1 args: pic file name");
		var o = cocos2d::Sprite::create(std::string(std::get<0>(t).first, std::get<0>(t).second));
		if (!o) return 0;
		return Lua_NewUserdataMT(L, o, LuaKey_Sprite);
	});

	Lua_NewFunc(L, "createWithSpriteFrame", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::SpriteFrame*>(L, "createWithSpriteFrame error! need 1 args: spriteFrame", LuaKey_SpriteFrame);
		var o = cocos2d::Sprite::createWithSpriteFrame(std::get<0>(t));
		if (!o) return 0;
		return Lua_NewUserdataMT(L, o, LuaKey_Sprite);
	});

	Lua_NewFunc(L, "createWithSpriteFrameName", [](lua_State* L)
	{
		var t = Lua_Tos(L, "createWithSpriteFrameName error! need 1 args: sprite frame name");
		var o = cocos2d::Sprite::createWithSpriteFrameName(std::string(std::get<0>(t).first, std::get<0>(t).second));
		if (!o) return 0;
		return Lua_NewUserdataMT(L, o, LuaKey_Sprite);
	});

	Lua_NewFunc(L, "createWithTexture", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::Texture2D*>(L, "createWithTexture error! need 1 args: texture", LuaKey_Texture);
		var o = cocos2d::Sprite::createWithTexture(std::get<0>(t));
		if (!o) return 0;
		return Lua_NewUserdataMT(L, o, LuaKey_Sprite);
	});

	Lua_NewFunc(L, "setSpriteFrame", [](lua_State* L)
	{
		var t = Lua_ToTT<cocos2d::Sprite*, cocos2d::SpriteFrame*>(L, "setSpriteFrame error! need 2 args: self, spriteFrame", LuaKey_Sprite, LuaKey_SpriteFrame);
		std::get<0>(t)->setSpriteFrame(std::get<1>(t));
		return 0;
	});







	// 增加一组易于使用的函数. 大写开头. 和 cocos 本体函数以示区别

	Lua_NewFunc(L, "CreateWithFOPA", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, cocos2d::Node*, float, float, float, float>
			(L, "CreateWithFOPA(Create with FileName, add to Owner, set Positon, set Anchor) error! need 6 args: owner, fileName, px, py, ax, ay");

		var o = cocos2d::Sprite::create(std::get<0>(t));
		if (!o) return 0;
		Lua_NewUserdataMT(L, o, LuaKey_Node);
		std::get<1>(t)->addChild(o);
		o->setPosition(std::get<2>(t), std::get<3>(t));
		o->setAnchorPoint({ std::get<4>(t), std::get<5>(t) });
		return 1;
	});

	lua_pop(L, 1);
}
