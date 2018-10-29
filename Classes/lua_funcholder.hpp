#pragma once

// 注意: 当前 cocos 有 listener 析构泄露的 bug. 引用计数为 2. 删不掉. 导致 Lua_FuncHolder 也无法析构.

// 被 std::function 携带, 当捕获列表析构发生时, 自动从 L 中反注册函数
struct Lua_FuncHolder
{
	// 全局自增函数 id
	inline static int autoIncFuncId = 1;

	// 将函数放入 funcs 表. 保存 key.
	int funcId = 0;
	Lua_FuncHolder(lua_State* const& L, int const& idx)
	{
		if (!idx) return;
		funcId = autoIncFuncId;
		lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// ..., funcs
		lua_pushvalue(L, idx);										// ..., funcs, func
		lua_rawseti(L, -2, funcId);									// ..., funcs
		lua_pop(L, 1);												// ...
		++autoIncFuncId;
	}

	// 没拷贝构造 std::function 编译不过( 实际不会执行 )
	Lua_FuncHolder(Lua_FuncHolder const&)
	{
		throw - 1;
	}

	// 移动构造. 0 funcId 在析构时啥都不干
	Lua_FuncHolder(Lua_FuncHolder && o)
		: funcId(o.funcId)
	{
		o.funcId = 0;
	}

	// 随 lambda 析构时根据 funcId 删掉函数
	~Lua_FuncHolder()
	{
		if (!gLua || !funcId) return;
		var L = gLua;
		lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// ..., funcs
		lua_pushnil(L);												// ..., funcs, nil
		lua_rawseti(L, -2, funcId);									// ..., funcs
		lua_pop(L, 1);												// ...
	}
};
