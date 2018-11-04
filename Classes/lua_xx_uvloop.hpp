#pragma once

// 向 lua 映射全局的 UvTcpClient 表/元表
inline void Lua_Register_UvLoop(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_UvLoop);									// xx, t

	Lua_NewFunc(L, "GetIPList", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, float, Lua_Func>(L, "GetIPList error! need 3 args: domain, timeoutSec, callback(...).");
		if (!std::get<2>(t))
		{
			luaL_error(L, "error! args[3] is not function.");
		}
		uv->GetIPList(std::get<0>(t).c_str(), [f = std::move(std::get<2>(t))](xx::List<xx::String_p>* ips)
		{
			var L = gLua;
			if (!L) return;

			if (!ips || !ips->dataLen)
			{
				Lua_PCall(L, f);
			}
			else
			{
				lua_checkstack(L, ips->dataLen + 1);
				Lua_Push(L, f);												// func
				for (var ip : *ips)
				{
					Lua_Push(L, ip);										// func, ip1, ip2, ....
				}
				Lua_PCall(L, ips->dataLen);
			}
		}, std::get<1>(t) * 1000);
		return 0;
	});

	Lua_NewFunc(L, "CreateTcpClientEx", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, int, Lua_Func, float>(L, "CreateTcpClient error! need 4 args: domain, port, callback(conn), timeoutSec.");
		if (!std::get<2>(t))
		{
			luaL_error(L, "error! args[3] is not function.");
		}
		var b = uv->CreateTcpClientEx(std::get<0>(t).c_str(), std::get<1>(t), [f = std::move(std::get<2>(t))](xx::UvTcpClient_w c)
		{
			var L = gLua;
			if (!L) return;

			if (!c)
			{
				Lua_PCall(L, f);
			}
			else
			{
				Lua_PCall(L, f, c);
			}
		}, std::get<1>(t) * 1000);
		return Lua_Push(L, b);
	});

	lua_pop(L, 1);													// xx
}
