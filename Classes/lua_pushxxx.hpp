#pragma once

// todo
// 常用于压入一串返回值并 return 其个数

template<typename T>
void Lua_Push(T const& v)
{
	if constexpr (std::is_integral_v<T>)
	{

	}
	else if constexpr (std::is_same_v<T, int>)
	{

	}
}


template<typename A, typename...Args>
void Lua_PushCore(A const& arg, Args const&...args)
{
}

template<typename...Args>
int Lua_Push(Args const&...args)
{
	Lua_PushCore(args...);
	return sizeof...(args);
}
