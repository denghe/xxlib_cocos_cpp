#pragma once

// 向 lua 映射 xx.Uv 表/元表
inline void Lua_Register_Uv(lua_State* const& L)
{
	// Uv

	Lua_NewMT(L, LuaKey_Uv);													// xx, Uv

	// todo: Stop ?

	lua_pop(L, 1);																// xx



	// UvItem

	Lua_NewMT(L, TypeNames<xx::UvItem_s*>::value);								// xx, UvItem

	Lua_NewFunc(L, "Dispose", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvItem_s*>(L, "Dispose error! need 1/2 args: self, int flag( 0 / 1 )");
		assert((*std::get<0>(t)));
		int flag = 1;
		if (lua_gettop(L) >= 2) {
			Lua_Get(flag, L, 1);
		}
		return Lua_Pushs(L, (*std::get<0>(t))->Dispose(flag));
	});

	Lua_NewFunc(L, "Disposed", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvItem_s*>(L, "Disposed error! need 1 args: self");
		assert((*std::get<0>(t)));
		return Lua_Pushs(L, (*std::get<0>(t))->Disposed());
	});

	// 这个需要放进每个直接的 mt. 否则针对的就是 table
	//Lua_NewFunc(L, "__gc", [](lua_State* L)
	//{
	//	auto&& t = Lua_ToTuple<xx::UvItem_s>(L, "__gc error! need 1 args: self");
	//	assert((*std::get<0>(t)));
	//	(*std::get<0>(t)).~shared_ptr();
	//	return 0;
	//});

	lua_pop(L, 1);																// xx
}
