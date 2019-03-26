#pragma once

// 注意: lua_checkstack

// 压入单个值并返回 1
template<typename T>
int Lua_Push(lua_State* const& L, T const& v)
{
	if constexpr (std::is_same_v<T, bool>)
	{
		lua_pushboolean(L, (int)v);
	}
	else if constexpr (std::is_enum_v<T>)
	{
		lua_pushinteger(L, (int)v);
	}
	else if constexpr (std::is_integral_v<T>)
	{
		lua_pushinteger(L, v);
	}
	else if constexpr (std::is_floating_point_v<T>)
	{
		lua_pushnumber(L, v);
	}
	else if constexpr (std::is_same_v<T, std::string>)
	{
		lua_pushlstring(L, v.data(), v.size());
	}
	else if constexpr (std::is_same_v<T, Lua_Func>)
	{
		assert(v.funcId);
		lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// ..., funcs
		lua_rawgeti(L, -1, *v.funcId);								// ..., funcs, func
		if (!lua_isfunction(L, -1))
		{
			luaL_error(L, "v.funcId:%d is bad.", *v.funcId);
		}
		lua_replace(L, -2);											// ..., func
	}
	else if constexpr (std::is_pointer_v<T> || xx::IsWeak_v<T> || xx::IsShared_v<T>)
	{
#ifndef NDEBUG
		if constexpr (std::is_pointer_v<T> && std::is_base_of_v<cocos2d::Ref, std::remove_pointer_t<T>>)
		{
			auto&& p = (T*)lua_newuserdata(L, sizeof(T) + sizeof(size_t));	// ..., &o + versionNumber
			*(size_t*)(p + 1) = cocos2d::Ref::versionNumber;	// 填充自增版本号
			cocos2d::Ref::ptrs[(void*)v] = cocos2d::Ref::versionNumber;
			++cocos2d::Ref::versionNumber;
			new (p) T(v);	// copy
		}
		else
#endif
		{
			auto&& p = lua_newuserdata(L, sizeof(T));				// ..., &o
			new (p) T(v);	// copy. need gc mt func release
		}
		lua_rawgetp(L, LUA_REGISTRYINDEX, TypeNames<T>::value);		// ..., &o, mt
		lua_setmetatable(L, -2);									// ..., &o
	}
	else if constexpr (
		std::is_same_v<T, cocos2d::Vector<cocos2d::Node*>>
		|| std::is_same_v<T, std::vector<std::string>>
		)
	{
		lua_createtable(L, v.size(), 0);
		int i = 0;
		for (auto&& o : v)
		{
			Lua_Push(L, o);
			lua_rawseti(L, -2, ++i);
		}
	}
	else if constexpr (std::is_same_v<T, std::unordered_map<std::string, std::string>>)
	{
		lua_createtable(L, v.size(), 0);
		for (auto&& o : v)
		{
			Lua_Pushs(L, o.first, o.second);
			lua_rawset(L, -3);
		}
	}
	return 1;
}

template<typename Arg, typename...Args>
void Lua_PushsCore(lua_State* const& L, Arg const& arg)
{
	Lua_Push(L, arg);
}

template<typename Arg, typename...Args>
void Lua_PushsCore(lua_State* const& L, Arg const& arg, Args const&...args)
{
	Lua_Push(L, arg);
	Lua_PushsCore(L, args...);
}

// 压入一串返回值并 return 其个数
template<typename...Args>
int Lua_Pushs(lua_State* const& L, Args const&...args)
{
	Lua_PushsCore(L, args...);
	return sizeof...(args);
}

// 安全调用一个函数( 同时压入一串参数 ). 返回 r.
template<typename...Args>
int Lua_PCall(lua_State* const& L, Lua_Func const& f, Args const&...args)
{
	lua_checkstack(L, sizeof...(args) + 1);				// 这句就先不查是否成功了. 无视一切内存不足的问题
	Lua_Pushs(L, f, args...);
	int r = 0;
	if (r = lua_pcall(L, sizeof...(args), LUA_MULTRET, 0))
	{
		cocos2d::log("%s", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	return r;
}

// 安全调用一个函数( 告知参数个数 ). 返回 r.
template<typename...Args>
int Lua_PCall(lua_State* const& L, int const& numArgs)
{
	int r = 0;
	if (r = lua_pcall(L, numArgs, LUA_MULTRET, 0))
	{
		cocos2d::log("%s", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	return r;
}




/*

// 感觉上无 lua 异常产生的回调 bind 代码. 备个份. 当前所有回调都是可能异常的精简版.

cocos2d::Director::getInstance()->mainLoopCallback = [f = std::move(f)]
{
	uv->Run(xx::UvRunMode::NoWait);

	auto&& L = gLua;
	gFuncId = f.funcId;
	lua_pushcclosure(L, [](lua_State* L)							// cfunc
	{
		lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
		lua_rawgeti(L, 1, gFuncId);									// funcs, func
		lua_call(L, 0, 0);											// funcs, ...?
		lua_settop(L, 0);											//
		return 0;
	}, 0);
	if (int r = lua_pcall(L, 0, 0, 0))								//
	{
		cocos2d::log("%s", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
};

*/
