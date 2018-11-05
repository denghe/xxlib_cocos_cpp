#pragma once

inline void Lua_Register_ccui(lua_State* const& L)
{
	// 创建全局 cc 基表
	lua_createtable(L, 0, 100);										// ccui
	lua_pushvalue(L, -1);											// ccui, ccui
	lua_setglobal(L, LuaKey_ccui);									// ccui

	// 创建 ccui.Xxxxxx 元表及函数									// ccui
	Lua_Register_uiButton(L);
	// .....

	lua_pop(L, 1);													//
	assert(lua_gettop(L) == 0);
}
