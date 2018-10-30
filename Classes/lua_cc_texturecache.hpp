#pragma once

inline void Lua_Register_TextureCache(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_TextureCache);

	Lua_NewFunc(L, "addImage", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "addImage error! need 1 args: pic file name");
		var o = cocos2d::TextureCache::getInstance()->addImage(std::get<0>(t));
		return Lua_NewUserdataMT(L, o, LuaKey_Texture);
	});

	lua_pop(L, 1);
}
