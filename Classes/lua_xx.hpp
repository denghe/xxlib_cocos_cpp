#pragma once

inline void Lua_Register_xx(lua_State* const& L)
{
	// 创建全局 cc 基表
	lua_createtable(L, 0, 100);										// xx
	lua_pushvalue(L, -1);											// xx, xx
	lua_setglobal(L, LuaKey_xx);									// xx

	// 创建 xx.Xxxxxx 元表及函数
	Lua_Register_MemPool(L);
	Lua_Register_UvLoop(L);
	Lua_BBuffer::LuaRegister(L);	// todo: 这个当前不在 xx 中, 需要整改
	// .....
	// .....

	lua_pop(L, 1);													//
	assert(lua_gettop(L) == 0);
}
