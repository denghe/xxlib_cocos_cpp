#pragma once

inline void Lua_Register_TMXTiledMap(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::experimental::TMXTiledMap*>::value, TypeNames<cocos2d::Node*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "create TMXTiledMap error! need 1 args: string tmxFile");
		var o = cocos2d::experimental::TMXTiledMap::create(std::get<0>(t));
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);
}
