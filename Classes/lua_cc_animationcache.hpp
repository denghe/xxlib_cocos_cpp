#pragma once

inline void Lua_Register_AnimationCache(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Animation*>::value, TypeNames<cocos2d::Ref*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::Animation();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var o = cocos2d::Animation::create();
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "createWithSpriteFrames", [](lua_State* L)
	{
		cocos2d::Animation* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::Vector<cocos2d::SpriteFrame*>*>(L);
			o = cocos2d::Animation::createWithSpriteFrames(*std::get<0>(t));
			std::get<0>(t)->clear();
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Vector<cocos2d::SpriteFrame*>*, float>(L);
			o = cocos2d::Animation::createWithSpriteFrames(*std::get<0>(t), std::get<1>(t));
			std::get<0>(t)->clear();
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::Vector<cocos2d::SpriteFrame*>*, float, int>(L);
			o = cocos2d::Animation::createWithSpriteFrames(*std::get<0>(t), std::get<1>(t), std::get<2>(t));
			std::get<0>(t)->clear();
			break;
		}
		default:
			return luaL_error(L, "createWithSpriteFrames error! need 1 ~ 3 args: SpriteFrame[] frames, float delay = 0, loop = 1");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});
	
	// 不实现 AnimationFrame 相关

	Lua_NewFunc(L, "addSpriteFrame", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*, cocos2d::SpriteFrame*>(L, "addSpriteFrame error! need 2 args: self, SpriteFrame frame");
		std::get<0>(t)->addSpriteFrame(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "addSpriteFrameWithFile", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*, std::string>(L, "addSpriteFrameWithFile error! need 2 args: self, string filename");
		std::get<0>(t)->addSpriteFrameWithFile(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "addSpriteFrameWithTexture", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*, cocos2d::Texture2D*, float, float, float, float>(L, "addSpriteFrameWithTexture error! need 6 args: self, Texture2D pobTexture, float rectX, Y, W, H");
		std::get<0>(t)->addSpriteFrameWithTexture(std::get<1>(t), { std::get<2>(t), std::get<3>(t), std::get<4>(t), std::get<5>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getTotalDelayUnits", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*>(L, "getTotalDelayUnits error! need 1 args: self");
		var r = std::get<0>(t)->getTotalDelayUnits();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "setDelayPerUnit", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*, float>(L, "setDelayPerUnit error! need 2 args: self, float delayPerUnit");
		std::get<0>(t)->setDelayPerUnit(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getDelayPerUnit", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*>(L, "getDelayPerUnit error! need 1 args: self");
		var r = std::get<0>(t)->getDelayPerUnit();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "getDuration", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*>(L, "getDuration error! need 1 args: self");
		var r = std::get<0>(t)->getDuration();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "getRestoreOriginalFrame", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*>(L, "getRestoreOriginalFrame error! need 1 args: self");
		var r = std::get<0>(t)->getRestoreOriginalFrame();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "setRestoreOriginalFrame", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*, bool>(L, "setRestoreOriginalFrame error! need 2 args: self, bool restoreOriginalFrame");
		std::get<0>(t)->setRestoreOriginalFrame(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getLoops", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*>(L, "getLoops error! need 1 args: self");
		var r = std::get<0>(t)->getLoops();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "setLoops", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*, int>(L, "setLoops error! need 2 args: self, int loops");
		std::get<0>(t)->setLoops(std::get<1>(t));
		return 0;
	});

	lua_pop(L, 1);





	Lua_NewMT(L, TypeNames<cocos2d::AnimationCache*>::value, TypeNames<cocos2d::Ref*>::value);

	Lua_NewFunc(L, "addAnimation", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Animation*, std::string>(L, "addAnimation error! need 2 args: Animation a, string name");
		cocos2d::AnimationCache::getInstance()->addAnimation(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeAnimation", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "removeAnimation error! need 1 args: string name");
		cocos2d::AnimationCache::getInstance()->removeAnimation(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "getAnimation", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "getAnimation error! need 1 args: self");
		var r = cocos2d::AnimationCache::getInstance()->getAnimation(std::get<0>(t));
		return Lua_Push(L, r);
	});

	// 不实现 addAnimationsWithDictionary

	Lua_NewFunc(L, "addAnimationsWithFile", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "addAnimationsWithFile error! need 1 args: string plist");
		cocos2d::AnimationCache::getInstance()->addAnimationsWithFile(std::get<0>(t));
		return 0;
	});

	lua_pop(L, 1);
}
