#pragma once

// todo: 调查 restart 时为啥 holder 析构没有执行

// 被 std::function 携带, 当捕获列表析构发生时, 自动从 L 中反注册函数
struct Lua_FuncHolder
{
	// 全局自增函数 id
	inline static int autoIncFuncId = 1;

	// 将函数放入 funcs 表. 保存 key.
	int funcId;
	Lua_FuncHolder(int const& idx)
		: funcId(autoIncFuncId)
	{
		var L = gLua;
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
		CCLOG("moved");
	}

	// 随 lambda 析构时根据 funcId 删掉函数
	~Lua_FuncHolder()
	{
		CCLOG("~~~~~");
		if (gLua && funcId)
		{
			CCLOG("deleted");
			var L = gLua;
			assert(lua_gettop(L) == 1);								// funcs
			lua_pushnil(L);											// funcs, nil
			lua_rawseti(L, 1, funcId);								// funcs
		}
	}

	// 将函数压栈( 之后调用方接着压入 参数 )
	inline void PushFunc() const
	{
		lua_rawgeti(gLua, 1, funcId);						// funcs, func
	}

	// 调用函数, 返回产生了多少个返回值( 之后调用方读出返回值, 并 settop(1) 清除它们 )
	inline int CallFunc(int const& numArgs) const
	{
		var L = gLua;
		if (int r = lua_pcall(L, numArgs, LUA_MULTRET, 0))			// funcs
		{
			std::cout << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
		return lua_gettop(L) - 1;
	}
};
