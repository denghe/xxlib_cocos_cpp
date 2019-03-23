#pragma once

// 被 std::function 捕获携带, 当捕获列表析构发生时, 自动从 L 中反注册函数
struct Lua_Func
{
	// 全局自增函数 id
	inline static int autoIncFuncId = 1;

	// 将函数放入 funcs 表. 保存 key.
	//int funcId = 0;
	std::shared_ptr<int> funcId;

	inline operator bool() const
	{
		return (bool)funcId;
	}

	Lua_Func() = default;
	Lua_Func(lua_State* const& L, int const& idx)
	{
		if (!idx) return;
		funcId = std::make_shared<int>(autoIncFuncId);
		lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// ..., funcs
		lua_pushvalue(L, idx);										// ..., funcs, func
		lua_rawseti(L, -2, *funcId);								// ..., funcs
		lua_pop(L, 1);												// ...
		++autoIncFuncId;
	}

	Lua_Func(Lua_Func const& o)
		: funcId(o.funcId)
	{
	}

	Lua_Func(Lua_Func&& o)
		: funcId(std::move(o.funcId))
	{
	}

	inline Lua_Func& operator=(Lua_Func const& o)
	{
		funcId = o.funcId;
		return *this;
	}
	inline Lua_Func& operator=(Lua_Func&& o)
	{
		std::swap(funcId, o.funcId);
		return *this;
	}

	// 随 lambda 析构时根据 funcId 删掉函数
	~Lua_Func()
	{
		if (!gLua || funcId.use_count() != 1) return;
		var top = lua_gettop(gLua);
		var L = gLua;
		lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// ..., funcs
		lua_pushnil(L);												// ..., funcs, nil
		lua_rawseti(L, -2, *funcId);								// ..., funcs
		lua_pop(L, 1);												// ...
		assert(top == lua_gettop(gLua));
	}
};
