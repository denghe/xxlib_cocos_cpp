#pragma once

inline void Lua_Register_AnimationCache(lua_State* const& L)
{
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
