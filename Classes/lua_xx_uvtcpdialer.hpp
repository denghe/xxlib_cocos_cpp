#pragma once

inline void Lua_Register_UvTcpLuaDialer(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<xx::UvTcpLuaDialer_s>::value, TypeNames<xx::UvItem_s>::value);					// xx.UvTcpDialer ： UvItem

	Lua_NewFunc(L, "Create", [](lua_State* L)
	{
		auto&& o = xx::TryMake<xx::UvTcpLuaDialer>(*uv);
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "Cancel", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvTcpLuaDialer_s>(L, "Cancel error! need 1 args: self");
		assert(std::get<0>(t));
		std::get<0>(t)->Cancel();
		return 0;
	});

	Lua_NewFunc(L, "Dial", [](lua_State* L)
	{
		int rtv = 0;
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			auto&& t = Lua_ToTuple<xx::UvTcpLuaDialer_s, std::vector<std::string>, int>(L);
			assert(std::get<0>(t));
			rtv = std::get<0>(t)->Dial(std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			auto&& t = Lua_ToTuple<xx::UvTcpLuaDialer_s, std::vector<std::string>, int, int>(L);
			assert(std::get<0>(t));
			rtv = std::get<0>(t)->Dial(std::get<1>(t), std::get<2>(t), std::get<3>(t));
			break;
		}
		default:
			return luaL_error(L, "Dial error! need 3/4 args: self, {strings} ipList, int port, int timeoutMS = 0");
		}
		return Lua_Pushs(L, rtv);
	});

	Lua_NewFunc(L, "OnAccept", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvTcpLuaDialer_s, Lua_Func>(L, "OnAccept error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->OnAccept = [self = std::move(std::get<0>(t)), f = std::move(std::get<1>(t))](xx::UvTcpLuaPeer_s& peer)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, peer);
				lua_settop(gLua, 0);
			};
		}
		else
		{
			std::get<0>(t)->OnAccept = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "State", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvTcpLuaDialer_s>(L, "State error! need 1 args: self");
		assert(std::get<0>(t));
		return Lua_Pushs(L, std::get<0>(t)->State());
	});

	lua_pop(L, 1);																									// xx
}
