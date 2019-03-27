#pragma once

inline void Lua_Register_UvKcpLuaDialer(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<xx::UvKcpLuaDialer_s>::value, TypeNames<xx::UvItem_s>::value);					// xx.UvKcpLuaDialer ： UvItem

	Lua_NewFunc(L, "Create", [](lua_State* L)
	{
		auto&& o = xx::TryMake<xx::UvKcpLuaDialer>(*uv);
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "__gc", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvItem_s>(L, "__gc error! need 1 args: self");
		assert(std::get<0>(t));
		std::get<0>(t).~shared_ptr();
		return 0;
	});

	Lua_NewFunc(L, "Cancel", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvKcpLuaDialer_s>(L, "Cancel error! need 1 args: self");
		assert(std::get<0>(t));
		std::get<0>(t)->Cancel();
		return 0;
	});

	Lua_NewFunc(L, "Dial", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvKcpLuaDialer_s, std::vector<std::string>, int>(L, "Dial error! need 3/4 args: self, {strings} ipList, int port, int timeoutMS = 0");
		assert(std::get<0>(t));
		int timeoutMS = 0;
		if (lua_gettop(L) > 3)
		{
			Lua_Get(timeoutMS, L, 4);
		}
		auto&& rtv = std::get<0>(t)->Dial(std::get<1>(t), std::get<2>(t), timeoutMS);
		return Lua_Pushs(L, rtv);
	});

	Lua_NewFunc(L, "OnAccept", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvKcpLuaDialer_s, Lua_Func>(L, "OnAccept error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->OnAccept = [f = std::move(std::get<1>(t))](xx::UvKcpLuaPeer_s& peer)
			{
				if (!gLua) return;
				assert(!lua_gettop(gLua));
				auto&& L = gLua;

				Lua_PCall(L, f, peer);
				lua_settop(L, 0);
			};
		}
		else
		{
			std::get<0>(t)->OnAccept = nullptr;
		}
		return 0;
	});

	//Lua_NewFunc(L, "State", [](lua_State* L)
	//{
	//	auto&& t = Lua_ToTuple<xx::UvKcpLuaDialer_s>(L, "State error! need 1 args: self");
	//	assert(std::get<0>(t));
	//	return Lua_Pushs(L, std::get<0>(t)->State());
	//});

	lua_pop(L, 1);																									// xx
}
