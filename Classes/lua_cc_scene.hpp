#pragma once

inline void Lua_Register_Scene(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_Scene, LuaKey_Node);

	Lua_NewFunc(L, "removeAllChildrenWithCleanup", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Scene*, bool>(L, "removeAllChildrenWithCleanup error! need 2 args: self, bool");
		std::get<0>(t)->removeAllChildrenWithCleanup(std::get<1>(t));
		return 0;
	});

	lua_pop(L, 1);
}
