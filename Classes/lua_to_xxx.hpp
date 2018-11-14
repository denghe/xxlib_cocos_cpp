#pragma once

template<typename T>
void Lua_Get(T& v, lua_State* const& L, int const& idx)
{
	if constexpr (std::is_same_v<T, bool>)
	{
		if (!lua_isboolean(L, idx)) goto LabError;
		v = lua_toboolean(L, idx);
		return;
	}
	else if constexpr (std::is_enum_v<T> || std::is_integral_v<T>)	// 这个判断包含了 bool 所以 bool 判断放上面
	{
		int isNum = 0;
		v = (T)lua_tointegerx(L, idx, &isNum);
		if (!isNum) goto LabError;
		return;
	}
	else if constexpr (std::is_floating_point_v<T>)
	{
		int isNum = 0;
		v = (T)lua_tonumberx(L, idx, &isNum);
		if (!isNum) goto LabError;
		return;
	}
	else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>)
	{
		if (!lua_isstring(L, idx)) goto LabError;
		v = (char*)lua_tostring(L, idx);
		return;
	}
	else if constexpr (std::is_same_v<T, std::string>)
	{
		if (!lua_isstring(L, idx)) goto LabError;
		size_t len;
		var s = lua_tolstring(L, idx, &len);
		v.assign(s, len);
		return;
	}
	else if constexpr (std::is_same_v<T, Lua_Func>)
	{
		if (lua_islightuserdata(L, idx) && lua_touserdata(L, idx) == nullptr) return;
		if (!lua_isfunction(L, idx)) goto LabError;
		v = std::move(Lua_Func(L, idx));
		return;
	}
	else if constexpr (std::is_pointer_v<T> || xx::IsWeak_v<T> || xx::IsRef_v<T>)
	{
		// 还需要进一步检测 mt 父子关系, 以及最终指针的 dynamic cast 来进一步判断, 以后上全局内存池方案再说
		if (!lua_isuserdata(L, idx)) goto LabError;
		// todo: check length == sizeof(T)
#ifndef NDEBUG
		if constexpr (std::is_pointer_v<T> && std::is_base_of_v<cocos2d::Ref, std::remove_pointer_t<T>>)
		{
			var p = (T*)lua_touserdata(L, idx);
			var versionNumber = *(size_t*)(p + 1);
			v = *p;
			if(cocos2d::Ref::ptrs.find(*p) == cocos2d::Ref::ptrs.cend() || cocos2d::Ref::ptrs[*p] != versionNumber) goto LabError;
		}
		else
		{
			v = *(T*)lua_touserdata(L, idx);
		}
#else
		v = *(T*)lua_touserdata(L, idx);
#endif
		return;
	}
	else
	{
		//static_assert(false);
		assert(false);
	}
LabError:
	luaL_error(L, "error! args[%d] is not %s", idx, TypeNames<T>::value);
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
		Lua_Get<std::tuple_element_t<idx - 1, Tuple>>(std::get<idx - 1>(t), L, idx);
		Lua_ForEach<idx - 1, Tuple>::FillTuple(t, L);
	}
};

// 不传 errMsg 表示不检查参数个数
template<typename...TS>
inline std::tuple<TS...> Lua_ToTuple(lua_State* const& L, char const* const& errMsg = "")
{
	if (errMsg)
	{
		if (lua_gettop(L) < sizeof...(TS))
		{
			luaL_error(L, "%s", errMsg);
		}
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

