#pragma once

// 压入单个值并返回实际压入 stack 的个数
template<typename T>
int Lua_Push(lua_State* const& L, T const& v)
{
	if constexpr (std::is_same_v<T, bool>)
	{
		lua_checkstack(L, 1);
		lua_pushboolean(L, (int)v);
	}
	else if constexpr (std::is_enum_v<T>)
	{
		lua_checkstack(L, 1);
		lua_pushinteger(L, (int)v);
	}
	else if constexpr (std::is_integral_v<T>)
	{
		lua_checkstack(L, 1);
		lua_pushinteger(L, v);
	}
	else if constexpr (std::is_floating_point_v<T>)
	{
		lua_checkstack(L, 1);
		lua_pushnumber(L, v);
	}
	else if constexpr (std::is_same_v<T, std::string>)
	{
		lua_checkstack(L, 1);
		lua_pushlstring(L, v.data(), v.size());
	}
	else if constexpr (std::is_same_v<T, char*> || std::is_same_v<T, char const*>)
	{
		lua_checkstack(L, 1);
		lua_pushstring(L, v);
	}
	else if constexpr (std::is_same_v<T, Lua_Func>)
	{
		lua_checkstack(L, 2);
		assert(v.funcId);
		lua_pushlightuserdata(L, (void*)LuaKey_Callbacks);			// ..., key
		lua_rawget(L, LUA_REGISTRYINDEX);							// ..., funcs
		lua_rawgeti(L, -1, *v.funcId);								// ..., funcs, func
		if (!lua_isfunction(L, -1))
		{
			luaL_error(L, "v.funcId:%d is bad.", *v.funcId);
		}
		lua_replace(L, -2);											// ..., func
	}
	else if constexpr (std::is_pointer_v<T> || xx::IsWeak_v<T> || xx::IsShared_v<T>)
	{
		lua_checkstack(L, 2);
		if (v) {
			if constexpr (std::is_pointer_v<T> && std::is_base_of_v<cocos2d::Ref, std::remove_pointer_t<T>>)
			{
#ifndef NDEBUG
				auto&& p = (T*)lua_newuserdata(L, sizeof(T) + sizeof(size_t));	// ..., &o + versionNumber
				*(size_t*)(p + 1) = cocos2d::Ref::versionNumber;	// 填充自增版本号
				cocos2d::Ref::ptrs[(void*)v] = cocos2d::Ref::versionNumber;
				++cocos2d::Ref::versionNumber;
#else
				auto&& p = lua_newuserdata(L, sizeof(T));				// ..., &o
#endif
				new (p) T(v);	// copy
				lua_rawgetp(L, LUA_REGISTRYINDEX, TypeNames<T>::value);	// ..., &o, mt
			}
			else if constexpr(xx::IsWeak_v<T> || xx::IsShared_v<T>)
			{
				auto&& p = lua_newuserdata(L, sizeof(T));				// ..., &o
				new (p) T(v);	// copy. need gc mt func release
				lua_pushlightuserdata(L, (void*)TypeNames<T*>::value);	// ..., &o, key
				lua_rawget(L, LUA_REGISTRYINDEX );						// ..., &o, mt
			}
			else {
				auto&& p = lua_newuserdata(L, sizeof(T));				// ..., &o
				new (p) T(v);	// copy. need gc mt func release
				lua_pushlightuserdata(L, (void*)TypeNames<T*>::value);	// ..., &o, key
				lua_rawget(L, LUA_REGISTRYINDEX );						// ..., &o, mt
			}
			lua_setmetatable(L, -2);									// ..., &o
		}
		else {
			lua_pushnil(L);
		}
	}
	else if constexpr (
		std::is_same_v<T, cocos2d::Vector<cocos2d::Node*>>
		|| std::is_same_v<T, std::vector<std::string>>
		)
	{
		lua_checkstack(L, 2);
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
		lua_checkstack(L, 3);
		lua_createtable(L, v.size(), 0);
		for (auto&& o : v)
		{
			Lua_Pushs(L, o.first, o.second);
			lua_rawset(L, -3);
		}
	}
	else if constexpr (std::is_same_v<T, std::vector<cocos2d::Touch*>>)
	{
		lua_checkstack(L, v.size());
		for (auto&& o : v)
		{
			Lua_Push(L, o);
		}
		return v.size();
	}
	return 1;
}

template<typename Arg, typename...Args>
int Lua_PushsCore(lua_State* const& L, Arg const& arg)
{
	return Lua_Push(L, arg);
}

template<typename Arg, typename...Args>
int Lua_PushsCore(lua_State* const& L, Arg const& arg, Args const&...args)
{
	int n = Lua_Push(L, arg);
	return n + Lua_PushsCore(L, args...);
}

// 压入一串返回值并 return 其个数
template<typename...Args>
int Lua_Pushs(lua_State* const& L, Args const&...args)
{
	return Lua_PushsCore(L, args...);
}

// 安全调用一个函数( 同时压入一串参数 ). 返回 r.
template<typename...Args>
int Lua_PCall(lua_State* const& L, Lua_Func const& f, Args const&...args)
{
	int n = Lua_Pushs(L, f, args...) - 1;
	int r = 0;
	if (r = lua_pcall(L, n, LUA_MULTRET, 0))
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
