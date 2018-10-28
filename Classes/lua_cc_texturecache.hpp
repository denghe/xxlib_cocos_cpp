﻿#pragma once

inline void Lua_Register_TextureCache(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_TextureCache);

	Lua_NewFunc(L, "addImage", [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "addImage error! need 1 args: texture's file name.");
		var fn = Lua_ToString<1>(L);
		var o = cocos2d::TextureCache::getInstance()->addImage(std::string(fn.first, fn.second));
		if (!o) return 0;
		return Lua_NewUserdataMT(L, o, LuaKey_Texture);
	});

	lua_pop(L, 1);
}
