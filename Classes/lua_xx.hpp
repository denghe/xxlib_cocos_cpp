#pragma once

inline void Lua_Register_xx(lua_State* const& L)
{
	// 创建全局 xx 基表
	lua_createtable(L, 0, 100);										// xx
	lua_pushvalue(L, -1);											// xx, xx
	lua_setglobal(L, LuaKey_xx);									// xx

	// 创建 xx.Xxxxxx 元表及函数
	Lua_Register_Uv(L);
	Lua_Register_UvResolver(L);
	Lua_Register_UvTcpLuaDialer(L);
	Lua_Register_UvTcpLuaPeer(L);
	Lua_BBuffer::LuaRegister(L);
	// .....
	// .....

	lua_pop(L, 1);													//
	assert(lua_gettop(L) == 0);
}
