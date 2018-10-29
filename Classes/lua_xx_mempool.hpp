#pragma once

// 向 lua 映射全局的 MemPool 表/元表
inline void Lua_Register_MemPool(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_MemPool);									// xx, t

	//Lua_NewFunc(L, "CreateBBuffer", [](lua_State* L)
	//{
	//	mp->MPCreate<Lua_BBuffer>();
	//	// todo
	//	//Lua_NewUserdataMT(L, )
	//	return 1;
	//});

	lua_pop(L, 1);													// xx
}
