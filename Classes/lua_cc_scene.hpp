#pragma once

inline void Lua_Register_Scene(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_Scene, LuaKey_Node);

	Lua_NewFunc(L, "removeAllChildrenWithCleanup", [](lua_State* L)
	{
		var t = Lua_ToTB<cocos2d::Scene>(L, "removeAllChildrenWithCleanup error! need 2 args: self, bool", LuaKey_Scene);
		std::get<0>(t)->removeAllChildrenWithCleanup(std::get<1>(t));
		return 0;
	});

	lua_pop(L, 1);
}
