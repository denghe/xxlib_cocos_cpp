#pragma once

inline void Lua_Register_TextureCache(lua_State* const& L)
{
	Lua_NewFunc(L, "addImage", [](lua_State* L)
	{
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			auto&& t = Lua_ToTuple<std::string>(L);
			auto&& r = cocos2d::Director::getInstance()->getTextureCache()->addImage(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			auto&& t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::Director::getInstance()->getTextureCache()->addImageAsync(std::get<0>(t), [f = std::move(std::get<1>(t))](cocos2d::Texture2D* t2d)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, t2d);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "addImage error! need 1 ~ 2 args: string filepath, function<void(Texture2D*)> callback");
		}
	});

	Lua_NewFunc(L, "unbindImageAsync", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string>(L, "unbindImageAsync error! need 1 args: string filepath");
		cocos2d::Director::getInstance()->getTextureCache()->unbindImageAsync(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "unbindAllImageAsync", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->getTextureCache()->unbindAllImageAsync();
		return 0;
	});

	Lua_NewFunc(L, "getTextureForKey", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string>(L, "getTextureForKey error! need 1 args: string key");
		auto&& r = cocos2d::Director::getInstance()->getTextureCache()->getTextureForKey(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "reloadTexture", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string>(L, "reloadTexture error! need 1 args: string fileName");
		auto&& r = cocos2d::Director::getInstance()->getTextureCache()->reloadTexture(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "removeAllTextures", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->getTextureCache()->removeAllTextures();
		return 0;
	});

	Lua_NewFunc(L, "removeUnusedTextures", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->getTextureCache()->removeUnusedTextures();
		return 0;
	});

	Lua_NewFunc(L, "removeTexture", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Texture2D*>(L, "removeTexture error! need 1 args: Texture2D texture");
		cocos2d::Director::getInstance()->getTextureCache()->removeTexture(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeTextureForKey", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string>(L, "removeTextureForKey error! need 1 args: string fileName");
		cocos2d::Director::getInstance()->getTextureCache()->removeTextureForKey(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "getCachedTextureInfo", [](lua_State* L)
	{
		auto&& r = cocos2d::Director::getInstance()->getTextureCache()->getCachedTextureInfo();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getTextureFilePath", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::Texture2D*>(L, "getTextureFilePath error! need 1 args: Texture2D texture");
		auto&& r = cocos2d::Director::getInstance()->getTextureCache()->getTextureFilePath(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "renameTextureWithKey", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string, std::string>(L, "renameTextureWithKey error! need 1 args: string srcName, dstName");
		cocos2d::Director::getInstance()->getTextureCache()->renameTextureWithKey(std::get<0>(t), std::get<1>(t));
		return 0;
	});
}
