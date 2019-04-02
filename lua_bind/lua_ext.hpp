#pragma once

inline void Lua_Register_ext(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<CatchFish_s>::value, nullptr, true);							// CatchFish

	Lua_NewFunc(L, "Create", [](lua_State* L)
	{
		return Lua_Pushs(L, std::make_shared<CatchFish>());
	});

	Lua_NewFunc(L, "__gc", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<CatchFish_s>(L, "__gc error! need 1 args: self");
		assert(std::get<0>(t));
		std::get<0>(t).~shared_ptr();
		return 0;
	});

	Lua_NewFunc(L, "Init", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<CatchFish_s, std::string>(L, "Init error! need 2 args: self, string cfgName");
		assert(std::get<0>(t));
		auto&& r = std::get<0>(t)->Init(std::move(std::get<1>(t)));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "Update", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<CatchFish_s>(L, "Update error! need 1 args: self");
		assert(std::get<0>(t));
		auto&& r = std::get<0>(t)->Update();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "Dispose", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<CatchFish_s>(L, "Dispose error! need 1 args: self");
		assert(std::get<0>(t));
		std::get<0>(t)->Dispose(1);
		return 0;
	});

	Lua_NewFunc(L, "Disposed", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<CatchFish_s>(L, "Disposed error! need 1 args: self");
		assert(std::get<0>(t));
		return Lua_Pushs(L, std::get<0>(t)->disposed);
	});


	lua_pop(L, 1);																		//
	assert(lua_gettop(L) == 0);
}
