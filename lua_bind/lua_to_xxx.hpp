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
		auto&& s = lua_tolstring(L, idx, &len);
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
	else if constexpr (std::is_same_v<T, cocos2d::Vector<cocos2d::SpriteFrame*>*>)
	{
		if (!lua_istable(L, idx)) goto LabError;
		v = &gSpriteFrames;
		gSpriteFrames.clear();
		int i = 1;
		while (true)
		{
			auto&& t = lua_rawgeti(L, idx, i);		// ... t, ..., v
			if (t == LUA_TNIL)
			{
				lua_pop(L, 1);						// ... t, ...
				return;
			}
			cocos2d::SpriteFrame* item = nullptr;
			Lua_Get(item, L, -1);
			gSpriteFrames.pushBack(item);
			lua_pop(L, 1);							// ... t, ...
			++i;
		}
		return;
	}
	else if constexpr (std::is_same_v<T, std::vector<std::string>>)
	{
		if (!lua_istable(L, idx)) goto LabError;
		v.clear();
		int i = 1;
		while (true)
		{
			auto&& t = lua_rawgeti(L, idx, i);		// ... t, ..., v
			if (t == LUA_TNIL)
			{
				lua_pop(L, 1);						// ... t, ...
				return;
			}
			std::string item;
			Lua_Get(item, L, -1);
			v.push_back(std::move(item));
			lua_pop(L, 1);							// ... t, ...
			++i;
		}
		return;
	}
	else if constexpr (std::is_pointer_v<T>)
	{
		// 还需要进一步检测 mt 父子关系, 以及最终指针的 dynamic cast 来进一步判断, 以后上全局内存池方案再说
		if (!lua_isuserdata(L, idx)) goto LabError;
		// todo: check length == sizeof(T)
		if constexpr (xx::IsWeak_v<std::remove_pointer_t<T>> || xx::IsShared_v<std::remove_pointer_t<T>>) {
			v = (T)lua_touserdata(L, idx);
		}
		else if constexpr (std::is_base_of_v<cocos2d::Ref, std::remove_pointer_t<T>>) {
			auto&& p = (T*)lua_touserdata(L, idx);
			v = *p;
#ifndef NDEBUG
			auto&& versionNumber = *(size_t*)(p + 1);
			if (cocos2d::Ref::ptrs.find(*p) == cocos2d::Ref::ptrs.cend() || cocos2d::Ref::ptrs[*p] != versionNumber) goto LabError;
#endif
		}
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

