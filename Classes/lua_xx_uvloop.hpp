#pragma once

// 向 lua 映射全局的 UvTcpClient 表/元表
inline void Lua_Register_UvLoop(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_UvLoop);									// t

	//Lua_NewFunc(L, "CreateTcpClient", [](lua_State* L)
	//{
	//	//uv->CreateTcpClient();
	//	// todo
	//	//Lua_NewUserdataMT(L, )
	//	return 1;
	//});

	Lua_NewFunc(L, "GetIPList", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		if (numArgs < 3)
		{
			luaL_error(L, "GetIPList error! need 3 args: domain, timeoutMS, callback.");
		}
		var domain = Lua_ToString<1>(L);
		var timeoutMS = Lua_ToNumber<int, 2>(L);
		var f = Lua_ToFuncHolder<3>(L);
		if (!f.funcId)
		{
			luaL_error(L, "error! args[3] is not function.");
		}
		uv->GetIPList(domain.first, [f = std::move(f)](xx::List<xx::String>* ips)
		{
			var L = gLua;
			gFuncId = f.funcId;
			gListString = ips;
			lua_pushcclosure(L, [](lua_State* L)							// cfunc
			{
				lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
				lua_rawgeti(L, 1, gFuncId);									// funcs, func
				if (!gListString)
				{
					lua_call(L, 0, 0);
					lua_settop(L, 0);										// 
					return 0;
				}
				lua_checkstack(L, gListString->dataLen + 2);
				for (var ip : *gListString)
				{
					lua_pushlstring(L, ip.buf, ip.dataLen);					// funcs, func, ip1, ip2, ....
				}
				lua_call(L, gListString->dataLen, 0);						// funcs, ...?
				lua_settop(L, 0);											//
				return 0;
			}, 0);
			if (int r = lua_pcall(L, 0, 0, 0))								//
			{
				cocos2d::log("%s", lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		}, timeoutMS);
		return 0;
	});

	lua_pop(L, 1);													//
}
