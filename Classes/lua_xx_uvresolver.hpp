#pragma once

inline void Lua_Register_UvResolver(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<xx::UvResolver_s>::value, TypeNames<xx::UvItem_s>::value);				// xx.UvResolver ： UvItem

	Lua_NewFunc(L, "Create", [](lua_State* L)
	{
		auto&& o = xx::TryMake<xx::UvResolver>(*uv);
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "Cancel", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvResolver_s>(L, "Cancel error! need 1 args: self");
		assert(std::get<0>(t));
		std::get<0>(t)->Cancel();
		return 0;
	});

	Lua_NewFunc(L, "Resolve", [](lua_State* L)
	{
		int rtv = 0;
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			auto&& t = Lua_ToTuple<xx::UvResolver_s, std::string>(L);
			assert(std::get<0>(t));
			rtv = std::get<0>(t)->Resolve(std::get<1>(t));
			break;
		}
		case 3:
		{
			auto&& t = Lua_ToTuple<xx::UvResolver_s, std::string, int>(L);
			assert(std::get<0>(t));
			rtv = std::get<0>(t)->Resolve(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "Resolve error! need 2/3 args: self, string domainName, int timeoutMS = 0");
		}
		return Lua_Pushs(L, rtv);
	});

	Lua_NewFunc(L, "OnFinish", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvResolver_s, Lua_Func>(L, "OnConnect error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->OnFinish = [self = std::move(std::get<0>(t)), f = std::move(std::get<1>(t))]
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f);
				lua_settop(gLua, 0);
			};
		}
		else
		{
			std::get<0>(t)->OnFinish = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "GetIPList", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvResolver_s>(L, "GetIPList error! need 1 args: self");
		assert(std::get<0>(t));
		return Lua_Pushs(L, std::get<0>(t)->ips);
	});

	lua_pop(L, 1);																									// xx
}
