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
		return Lua_FuncHolder(L, 0);
	}
	if (!lua_isfunction(L, idx))
	{
		luaL_error(L, "error! args[%d] is not function.", idx);
	}
	return Lua_FuncHolder(L, idx);
}


// 试着将 idx 所在 userdata 转为 T* 取出 T
// 还需要进一步检测 mt 父子关系, 以及最终指针的 dynamic cast 来进一步判断, 以后上全局内存池方案再说
template<typename T>
inline T Lua_ToPointer(lua_State* const& L, int const& idx, char const* const& typeName)
{
	if (!lua_isuserdata(L, idx))
	{
		luaL_error(L, "error! args[%d] is not %s.", idx, typeName);
	}
	// todo: check length == sizeof(T)
	return *(T*)lua_touserdata(L, idx);
}

// 试着将 idx 所在转为 T** 取出 T*
// 还需要进一步检测 mt 父子关系, 以及最终指针的 dynamic cast 来进一步判断, 以后上全局内存池方案再说
template<typename T, int idx>
inline T Lua_ToPointer(lua_State* const& L, char const* const& typeName)
{
	return Lua_ToPointer<T>(L, idx, typeName);
}





// 一些常用转换. 试着将 idx:1 开始位置所在转为 返回值 tuple 结构

template<typename T>
inline std::tuple<T, float, float, float> Lua_ToTfff(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 4)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToNumber<float, 2>(L), Lua_ToNumber<float, 3>(L), Lua_ToNumber<float, 4>(L));
}

template<typename T>
inline std::tuple<T, float, float> Lua_ToTff(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 3)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToNumber<float, 2>(L), Lua_ToNumber<float, 3>(L));
}

template<typename T>
inline std::tuple<T, float> Lua_ToTf(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToNumber<float, 2>(L));
}

template<typename T>
inline std::tuple<T, int> Lua_ToTi(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToNumber<int, 2>(L));
}

template<typename T>
inline std::tuple<T, bool> Lua_ToTb(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToBoolean<2>(L));
}

template<typename T>
inline std::tuple<T, std::pair<char const*, size_t>> Lua_ToTs(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToString<2>(L));
}

inline std::tuple<std::pair<char const*, size_t>> Lua_Tos(lua_State* const& L, char const* const& errMsg)
{
	if (lua_gettop(L) < 1)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToString<1>(L));
}

template<typename T>
inline std::tuple<T, Lua_FuncHolder> Lua_ToTF(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToFuncHolder<2>(L));
}


template<typename T>
inline std::tuple<T> Lua_ToT(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 1)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName));
}

template<typename T1, typename T2>
inline std::tuple<T1, T2> Lua_ToTT(lua_State* const& L, char const* const& errMsg, char const* const& t1Name, char const* const& t2Name)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T1, 1>(L, t1Name), Lua_ToPointer<T2, 2>(L, t2Name));
}

template<typename T1, typename T2>
inline std::tuple<T1, int, T2> Lua_ToTiT(lua_State* const& L, char const* const& errMsg, char const* const& t1Name, char const* const& t2Name)
{
	if (lua_gettop(L) < 3)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T1, 1>(L, t1Name), Lua_ToNumber<int, 2>(L), Lua_ToPointer<T2, 3>(L, t2Name));
}

template<typename T1, typename T2>
inline std::tuple<T1, T2, Lua_FuncHolder, double> Lua_ToTTFd(lua_State* const& L, char const* const& errMsg, char const* const& t1Name, char const* const& t2Name)
{
	if (lua_gettop(L) < 4)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T1, 1>(L, t1Name), Lua_ToPointer<T2, 2>(L, t2Name), Lua_ToFuncHolder<3>(L), Lua_ToNumber<double, 4>(L));
}

template<typename T>
inline std::tuple<T, std::pair<char const*, size_t>, int> Lua_ToTsi(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 3)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToString<2>(L), Lua_ToNumber<int, 3>(L));
}

template<typename T>
inline std::tuple<T, bool, int> Lua_ToTbi(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 3)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToBoolean<2>(L), Lua_ToNumber<int, 3>(L));
}

template<typename T>
inline std::tuple<T, double> Lua_ToTd(lua_State* const& L, char const* const& errMsg, char const* const& tName)
{
	if (lua_gettop(L) < 2)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T, 1>(L, tName), Lua_ToNumber<double, 2>(L));
}

template<typename T1, typename T2>
inline std::tuple<T1, T2, float, float, float, float, float, float> Lua_ToTTffffff(lua_State* const& L, char const* const& errMsg, char const* const& t1Name, char const* const& t2Name)
{
	if (lua_gettop(L) < 8)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T1, 1>(L, t1Name), Lua_ToPointer<T2, 2>(L, t2Name)
		, Lua_ToNumber<float, 3>(L), Lua_ToNumber<float, 4>(L)
		, Lua_ToNumber<float, 5>(L), Lua_ToNumber<float, 6>(L)
		, Lua_ToNumber<float, 7>(L), Lua_ToNumber<float, 8>(L));
}

template<typename T1, typename T2>
inline std::tuple<T1, T2, float, float, float, float, float> Lua_ToTTfffff(lua_State* const& L, char const* const& errMsg, char const* const& t1Name, char const* const& t2Name)
{
	if (lua_gettop(L) < 7)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T1, 1>(L, t1Name), Lua_ToPointer<T2, 2>(L, t2Name)
		, Lua_ToNumber<float, 3>(L), Lua_ToNumber<float, 4>(L)
		, Lua_ToNumber<float, 5>(L), Lua_ToNumber<float, 6>(L)
		, Lua_ToNumber<float, 7>(L));
}

template<typename T1, typename T2>
inline std::tuple<T1, T2, float, float, float, float> Lua_ToTTffff(lua_State* const& L, char const* const& errMsg, char const* const& t1Name, char const* const& t2Name)
{
	if (lua_gettop(L) < 6)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T1, 1>(L, t1Name), Lua_ToPointer<T2, 2>(L, t2Name)
		, Lua_ToNumber<float, 3>(L), Lua_ToNumber<float, 4>(L)
		, Lua_ToNumber<float, 5>(L), Lua_ToNumber<float, 6>(L));
}

template<typename T1, typename T2>
inline std::tuple<T1, T2, float, float> Lua_ToTTff(lua_State* const& L, char const* const& errMsg, char const* const& t1Name, char const* const& t2Name)
{
	if (lua_gettop(L) < 4)
	{
		luaL_error(L, "%s", errMsg);
	}
	return std::make_tuple(Lua_ToPointer<T1, 1>(L, t1Name), Lua_ToPointer<T2, 2>(L, t2Name)
		, Lua_ToNumber<float, 3>(L), Lua_ToNumber<float, 4>(L));
}






template<typename T>
struct TypeNames
{
	inline static const char* value = "";
};
template<>
struct TypeNames<int>
{
	inline static const char* value = "int";
};
template<>
struct TypeNames<int64_t>
{
	inline static const char* value = "int64";
};
template<>
struct TypeNames<float>
{
	inline static const char* value = "float";
};
template<>
struct TypeNames<double>
{
	inline static const char* value = "double";
};
template<>
struct TypeNames<char*>
{
	inline static const char* value = "string";
};
template<>
struct TypeNames<std::string>
{
	inline static const char* value = "string";
};
template<>
struct TypeNames<Lua_FuncHolder>
{
	inline static const char* value = "func";
};
template<>
struct TypeNames<cocos2d::Sprite*>
{
	inline static const char* value = "Sprite*";
};
template<>
struct TypeNames<cocos2d::Node*>
{
	inline static const char* value = "Node*";
};

// todo: more names


template<typename T, int idx>
T Lua_Get(lua_State* const& L)
{
	if constexpr (std::is_integral_v<T>)
	{
		int isNum = 0;
		var rtv = (T)lua_tointegerx(L, idx, &isNum);
		if (!isNum) goto LabError;
		return rtv;
	}
	else if constexpr (std::is_floating_point_v<T>)
	{
		int isNum = 0;
		var rtv = (T)lua_tonumberx(L, idx, &isNum);
		if (!isNum) goto LabError;
		return rtv;
	}
	else if constexpr (std::is_same_v<T, char*>)
	{
		if (!lua_isstring(L, idx)) goto LabError;
		return lua_tostring(L, idx);
	}
	else if constexpr (std::is_same_v<T, std::string>)
	{
		if (!lua_isstring(L, idx)) goto LabError;
		size_t len;
		var s = lua_tolstring(L, idx, &len);
		return std::string(s, len);
	}
	else if constexpr (std::is_same_v<T, boolean>)
	{
		if (!lua_isboolean(L, idx)) goto LabError;
		return lua_toboolean(L, idx);
	}
	else if constexpr (std::is_same_v<T, Lua_FuncHolder>)
	{
		if (lua_islightuserdata(L, idx) && lua_touserdata(L, idx) == nullptr) return Lua_FuncHolder(L, 0);
		if (!lua_isfunction(L, idx)) goto LabError;
		return Lua_FuncHolder(L, idx);
	}
	else if constexpr (std::is_pointer_v<T> || xx::IsWeak_v<T> || xx::IsRef_v<T>)
	{
		if (!lua_isuserdata(L, idx)) goto LabError;
		// todo: check length == sizeof(T)
		return *(T*)lua_touserdata(L, idx);
	}
LabError:
	luaL_error(L, "error! args[%d] is not %s", idx, TypeNames<T>::value);
	return T();	// 避免 warning
}





template<int idx, typename Tuple>
struct Lua_ForEach;

template<typename Tuple>
struct Lua_ForEach<0, Tuple>
{
	static void FillTuple(Tuple& t, lua_State* const& L) {}
};

template<int idx, typename Tuple>
struct Lua_ForEach
{
	static void FillTuple(Tuple& t, lua_State* const& L)
	{
		std::get<idx - 1>(t) = Lua_Get<std::tuple_element_t<idx - 1, Tuple>, idx>(L);
		Lua_ForEach<idx - 1, Tuple>::FillTuple(t, L);
	}
};

template<typename...TS>
inline std::tuple<TS...> Lua_ToTuple(lua_State* const& L, char const* const& errMsg)
{
	if (lua_gettop(L) < sizeof...(TS))
	{
		luaL_error(L, "%s", errMsg);
	}
	std::tuple<TS...> t;
	Lua_ForEach<sizeof...(TS), std::tuple<TS...>>::FillTuple(t, L);
	return t;
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

