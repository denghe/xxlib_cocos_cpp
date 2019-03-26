#pragma once

inline void Lua_Register_SpriteFrameCache(lua_State* const& L)
{
	Lua_NewFunc(L, "addSpriteFramesWithFile", [](lua_State* L)
	{
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			auto&& t = Lua_ToTuple<std::string>(L);
			cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(std::get<0>(t));
			break;
		}
		case 2:
		{
			if (lua_isstring(L, 2))
			{
				auto&& t = Lua_ToTuple<std::string, std::string>(L);
				cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(std::get<0>(t), std::get<1>(t));
			}
			else
			{
				auto&& t = Lua_ToTuple<std::string, cocos2d::Texture2D*>(L);
				cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(std::get<0>(t), std::get<1>(t));
			}
			break;
		}
		default:
			return luaL_error(L, "%s", "addSpriteFramesWithFile error! need 1 ~ 2 args: string plist, string textureFileName / Texture2D texture");
		}
		return 0;
	});


	Lua_NewFunc(L, "addSpriteFramesWithFileContent", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string, cocos2d::Texture2D*>(L, "addSpriteFramesWithFileContent error! need 2 args: string plist_content, Texture2D texture");
		cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFileContent(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "addSpriteFrame", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::SpriteFrame*, std::string>(L, "addSpriteFrame error! need 2 args: SpriteFrame frame, string frameName");
		cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isSpriteFramesWithFileLoaded", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string>(L, "isSpriteFramesWithFileLoaded error! need 1 args: string plist");
		auto&& r = cocos2d::SpriteFrameCache::getInstance()->isSpriteFramesWithFileLoaded(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "removeSpriteFrames", [](lua_State* L)
	{
		cocos2d::SpriteFrameCache::getInstance()->removeSpriteFrames();
		return 0;
	});

	Lua_NewFunc(L, "removeUnusedSpriteFrames", [](lua_State* L)
	{
		cocos2d::SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
		return 0;
	});

	Lua_NewFunc(L, "removeSpriteFrameByName", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string>(L, "removeSpriteFrameByName error! need 1 args: string name");
		cocos2d::SpriteFrameCache::getInstance()->removeSpriteFrameByName(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeSpriteFramesFromFile", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string>(L, "removeSpriteFramesFromFile error! need 1 args: string plist");
		cocos2d::SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeSpriteFramesFromFileContent", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string>(L, "removeSpriteFramesFromFileContent error! need 1 args: string plist_content");
		cocos2d::SpriteFrameCache::getInstance()->removeSpriteFramesFromFileContent(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeSpriteFramesFromTexture", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Texture2D*>(L, "removeSpriteFramesFromTexture error! need 1 args: Texture2D texture");
		cocos2d::SpriteFrameCache::getInstance()->removeSpriteFramesFromTexture(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "getSpriteFrameByName", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string>(L, "getSpriteFrameByName error! need 1 args: string name");
		auto&& r = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "reloadTexture", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string>(L, "reloadTexture error! need 1 args: string plist");
		auto&& r = cocos2d::SpriteFrameCache::getInstance()->reloadTexture(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

}
