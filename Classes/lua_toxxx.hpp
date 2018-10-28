#pragma once

// 试着将 idx 所在转为 string. string 在使用时要自己转为 std::string 以尽量避免先构造出来又遇到 exception 要回滚的情况
template<int idx>
inline std::pair<char const*, size_t> Lua_ToString(lua_State* const& L)
{
	std::pair<char const*, size_t> rtv;
	if (!lua_isstring(L, idx))
	{
		luaL_error(L, "error! args[%d] is not string.", idx);
	}
	rtv.first = lua_tolstring(L, idx, &rtv.second);
	return rtv;
}


// 试着将 idx 所在转为 boolean. 
template<int idx>
inline bool Lua_ToBoolean(lua_State* const& L)
{
	if (!lua_isboolean(L, idx))
	{
		luaL_error(L, "error! args[%d] is not bool.", idx);
	}
	return lua_toboolean(L, idx);
}


// 试着将 idx 所在转为 number:T
template<typename T, int idx>
inline T Lua_ToNumber(lua_State* const& L)
{
	static_assert(std::is_arithmetic<T>::value);

	int isNum = 0;
	T rtv;
	if constexpr (std::is_integral<T>::value)
	{
		rtv = (T)lua_tointegerx(L, idx, &isNum);
		if (!isNum)
		{
			luaL_error(L, "error! args[%d] is not integral.", idx);
		}
	}
	else
	{
		rtv = (T)lua_tonumberx(L, idx, &isNum);
		if (!isNum)
		{
			luaL_error(L, "error! args[%d] is not float/double.", idx);
		}
	}
	return rtv;
}


// 试着将 idx 所在转为 funcHolder
template<int idx>
inline Lua_FuncHolder Lua_ToFuncHolder(lua_State* const& L)
{
	if (lua_islightuserdata(L, idx) && lua_touserdata(L, idx) == nullptr)
	{
		return Lua_FuncHolder(0);
	}
	if (!lua_isfunction(L, idx))
	{
		luaL_error(L, "error! args[%d] is not function.", idx);
	}
	return Lua_FuncHolder(idx);
}


// 试着将 idx 所在转为 T** 取出 T*
// 还需要进一步检测 mt 父子关系, 以及最终指针的 dynamic cast 来进一步判断, 以后上全局内存池方案再说
template<typename T, int idx>
inline T* Lua_ToPointer(lua_State* const& L, char const* const& typeName)
{
	if (!lua_isuserdata(L, idx))
	{
		luaL_error(L, "error! args[%d] is not %s.", idx, typeName);
	}
	var p = (T**)lua_touserdata(L, idx);
	if (!p)
	{
		luaL_error(L, "error! args[%d] is nullptr.", idx);
	}
	return *p;
}




// 下面的是一些组合用法

// 试着将 idx:1 开始位置所在转为 T, float, float
template<typename T>
inline std::tuple<T*, float, float> Lua_ToTFF(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 3)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToNumber<float, 2>(L), Lua_ToNumber<float, 3>(L));
}

// 试着将 idx:1 开始位置所在转为 T, float
template<typename T>
inline std::tuple<T*, float> Lua_ToTF(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToNumber<float, 2>(L));
}

// 试着将 idx:1 开始位置所在转为 T, int
template<typename T>
inline std::tuple<T*, int> Lua_ToTI(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToNumber<int, 2>(L));
}

// 试着将 idx:1 开始位置所在转为 T, bool
template<typename T>
inline std::tuple<T*, bool> Lua_ToTB(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToBoolean<2>(L));
}

// 试着将 idx:1 所在转为 T, string
template<typename T>
inline std::tuple<T*, std::pair<char const*, size_t>> Lua_ToTS(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToString<2>(L));
}

// 试着将 idx:1 所在转为 T, func
template<typename T>
inline std::tuple<T*, Lua_FuncHolder> Lua_ToTf(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToFuncHolder<2>(L));
}


// 试着将 idx:1 开始位置所在转为 T
template<typename T>
inline std::tuple<T*> Lua_ToT(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 1)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName));
}

// 试着将 idx:1 开始位置所在转为 T1, T2
template<typename T1, typename T2>
inline std::tuple<T1*, T2*> Lua_ToTT(lua_State* const& L, char const* const& errMsg, char const* const& t1Name, char const* const& t2Name)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T1, 1>(L, t1Name), Lua_ToPointer<T2, 2>(L, t2Name));
}





//// 试着将 idx 所在 table 转为 vector<T>. . 外部已判断过类型是 table.
//template<typename T, int idx>
//inline std::vector<T> const& Lua_ToValues(lua_State* const& L)
//{
//	// clear
//	if constexpr (std::is_same<T, int>::value) { gInts.clear(); }
//	else if constexpr (std::is_same<T, int64_t>::value) { gLongs.clear(); }
//	else if constexpr (std::is_same<T, float>::value) { gFloats.clear(); }
//	else if constexpr (std::is_same<T, double>::value) { gDoubles.clear(); }
//	else { gStrings.clear(); }
//
//	// foreach fill
//	lua_pushnil(L);								// ... t, nil
//	int n = 1;
//	while (lua_next(L, idx) != 0)				// ... t, k, v
//	{
//		if constexpr (std::is_same<T, int>::value)
//		{
//			int isNum = 0;
//			var v = (T)lua_tointegerx(L, -1, &isNum);
//			if (!isNum)
//			{
//				luaL_error(L, "args[%d][%d] is not int.", idx, n);
//			}
//			gInts.push_back(v);
//		}
//		else if constexpr (std::is_same<T, int64_t>::value)
//		{
//			int isNum = 0;
//			var v = (T)lua_tointegerx(L, -1, &isNum);
//			if (!isNum)
//			{
//				luaL_error(L, "args[%d][%d] is not long.", idx, n);
//			}
//			gLongs.push_back(v);
//		}
//		else if constexpr (std::is_same<T, float>::value)
//		{
//			int isNum = 0;
//			var v = (T)lua_tonumberx(L, -1, &isNum);
//			if (!isNum)
//			{
//				luaL_error(L, "args[%d][%d] is not float.", idx, n);
//			}
//			gFloats.push_back(v);
//		}
//		else if constexpr (std::is_same<T, double>::value)
//		{
//			int isNum = 0;
//			var v = (T)lua_tonumberx(L, -1, &isNum);
//			if (!isNum)
//			{
//				luaL_error(L, "args[%d][%d] is not double.", idx, n);
//			}
//			gDoubles.push_back(v);
//		}
//		else	// string
//		{
//			if (!lua_isstring(L, -1))
//			{
//				luaL_error(L, "args[%d][%d] is not string.", idx, n);
//			}
//			size_t len;
//			var buf = lua_tolstring(L, -1, &len);
//			gStrings.emplace_back(buf, len);
//		}
//		lua_pop(L, 1);							// ... t, k
//		++n;
//	}											// ... t
//
//	// return
//	if constexpr (std::is_same<T, int>::value) { return gInts; }
//	else if constexpr (std::is_same<T, int64_t>::value) { return gLongs; }
//	else if constexpr (std::is_same<T, float>::value) { return gFloats; }
//	else if constexpr (std::is_same<T, double>::value) { return gDoubles; }
//	else { return gStrings; }
//}

