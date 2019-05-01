#pragma once

inline void Lua_Register_UvResolver(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<xx::UvResolver_s*>::value, TypeNames<xx::UvItem_s*>::value);				// xx.UvResolver ： UvItem

	Lua_NewFunc(L, "Create", [](lua_State* L)
	{
		auto&& o = xx::TryMake<xx::UvResolver>(*uv);
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "__gc", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvItem_s*>(L, "__gc error! need 1 args: self");
		assert((*std::get<0>(t)));
		(*std::get<0>(t)).~shared_ptr();
		return 0;
	});

	Lua_NewFunc(L, "Cancel", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvResolver_s*>(L, "Cancel error! need 1 args: self");
		assert((*std::get<0>(t)));
		(*std::get<0>(t))->Cancel();
		return 0;
	});

	Lua_NewFunc(L, "Resolve", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvResolver_s*, std::string>(L, "Resolve error! need 2/3 args: self, string domainName, int timeoutMS = 0");
		assert((*std::get<0>(t)));
		int timeoutMS = 0;
		if (lua_gettop(L) > 3)
		{
			Lua_Get(timeoutMS, L, 4);
		}
		int rtv = (*std::get<0>(t))->Resolve(std::get<1>(t), timeoutMS);
		return Lua_Pushs(L, rtv);
	});

	Lua_NewFunc(L, "OnFinish", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvResolver_s*, Lua_Func>(L, "OnConnect error! need 2 args: self, func/null");
		assert((*std::get<0>(t)));
		if (std::get<1>(t))
		{
			(*std::get<0>(t))->onFinish = [f = std::move(std::get<1>(t))]
			{
				if (!gLua) return;
				assert(!lua_gettop(gLua));
				auto&& L = gLua;

				assert(!lua_gettop(L));
				Lua_PCall(L, f);
				lua_settop(L, 0);
			};
		}
		else
		{
			(*std::get<0>(t))->onFinish = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "GetIPList", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvResolver_s*>(L, "GetIPList error! need 1 args: self");
		assert((*std::get<0>(t)));
		return Lua_Pushs(L, (*std::get<0>(t))->ips);
	});

	lua_pop(L, 1);																									// xx
}
