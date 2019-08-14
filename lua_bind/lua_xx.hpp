﻿#pragma once

inline void Lua_Register_xx(lua_State* const& L)
{
	// 创建全局 xx 基表
	lua_createtable(L, 0, 100);										// xx
	lua_pushvalue(L, -1);											// xx, xx
	lua_setglobal(L, LuaKey_xx);									// xx

	// 映射几个工具函数

	Lua_NewFunc(L, "NowSteadyEpochMS", [](lua_State* L)
	{
		return Lua_Pushs(L, xx::NowSteadyEpochMS());
	});


	// 创建 xx.Xxxxxx 元表及函数
	Lua_Register_Uv(L);
	Lua_Register_UvResolver(L);
	Lua_Register_UvDialer(L);
	Lua_Register_UvPeer(L);
	Lua_BBuffer::LuaRegister(L);
	// .....
	// .....

	lua_pop(L, 1);													//
	assert(lua_gettop(L) == 0);
}
