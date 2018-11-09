#pragma once

// 被 std::function 携带, 移动捕获. 当捕获列表析构发生时, 自动从 L 中反注册函数
struct Lua_Func
{
	// 全局自增函数 id
	inline static int autoIncFuncId = 1;

	// 将函数放入 funcs 表. 保存 key.
	int funcId = 0;

	inline operator bool() const
	{
		return funcId != 0;
	}

	Lua_Func() = default;
	Lua_Func(lua_State* const& L, int const& idx)
	{
		if (!idx) return;
		funcId = autoIncFuncId;
		lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// ..., funcs
		lua_pushvalue(L, idx);										// ..., funcs, func
		lua_rawseti(L, -2, funcId);									// ..., funcs
		lua_pop(L, 1);												// ...
		++autoIncFuncId;
	}

	// 不写这个拷贝构造 std::function 编译报错( 实际不会执行 )
	Lua_Func(Lua_Func const&)
	{
		throw - 1;
	}

	// 移动构造
	Lua_Func(Lua_Func && o)
		: funcId(o.funcId)
	{
		o.funcId = 0;
	}

	// 移动赋值
	inline Lua_Func& operator=(Lua_Func && o)
	{
		funcId = o.funcId;
		o.funcId = 0;
		return *this;
	}

	// 随 lambda 析构时根据 funcId 删掉函数
	~Lua_Func()
	{
		if (!gLua || !funcId) return;
		var top = lua_gettop(gLua);
		var L = gLua;
		lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// ..., funcs
		lua_pushnil(L);												// ..., funcs, nil
		lua_rawseti(L, -2, funcId);									// ..., funcs
		lua_pop(L, 1);												// ...
		assert(top == lua_gettop(gLua));
	}
};
