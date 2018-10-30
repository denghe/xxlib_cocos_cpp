#pragma once

inline void Lua_Register_Sprite(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Sprite*>::value, TypeNames<cocos2d::Node*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::Sprite();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "initWithTexture", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*, cocos2d::Texture2D*>(L, "initWithTexture error! need 2 args: self, texture");
		std::get<0>(t)->initWithTexture(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "initWithFileName", [](lua_State* L)
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
		var o = cocos2d::Sprite::create();
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "createWithFileName", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "createWithFileName error! need 1 args: pic file name");
		var o = cocos2d::Sprite::create(std::get<0>(t));
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "createWithSpriteFrame", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::SpriteFrame*>(L, "createWithSpriteFrame error! need 1 args: spriteFrame");
		var o = cocos2d::Sprite::createWithSpriteFrame(std::get<0>(t));
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "createWithSpriteFrameName", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "createWithSpriteFrameName error! need 1 args: sprite frame name");
		var o = cocos2d::Sprite::createWithSpriteFrameName(std::get<0>(t));
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "createWithTexture", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Texture2D*>(L, "createWithTexture error! need 1 args: texture");
		var o = cocos2d::Sprite::createWithTexture(std::get<0>(t));
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "setSpriteFrame", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Sprite*, cocos2d::SpriteFrame*>(L, "setSpriteFrame error! need 2 args: self, spriteFrame");
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
		Lua_Push(L, o);
		std::get<1>(t)->addChild(o);
		o->setPosition(std::get<2>(t), std::get<3>(t));
		o->setAnchorPoint({ std::get<4>(t), std::get<5>(t) });
		return 1;
	});

	lua_pop(L, 1);
}
