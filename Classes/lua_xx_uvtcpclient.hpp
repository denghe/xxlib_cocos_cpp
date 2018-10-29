#pragma once

// 这个对象受 lua 管理生命周期. Create 出来之后不需要主动释放
inline void Lua_Register_UvTcpClient(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_UvTcpClient);									// xx, t

	Lua_NewFunc(L, "Create", [](lua_State* L)
	{
		var o = uv->CreateTcpClient();
		if (!o) return 0;
		Lua_NewUserdataMT(L, o, LuaKey_UvTcpClient);
		return 1;
	});

	Lua_NewFunc(L, "__gc", [](lua_State* L)
	{
		var t = Lua_ToT<xx::UvTcpClient_w>(L, "__gc error! need 1 args: self", LuaKey_UvTcpClient);
		assert(std::get<0>(t));
		std::get<0>(t)->Release();
		return 0;
	});

	Lua_NewFunc(L, "Send", [](lua_State* L)
	{
		var t = Lua_ToTT<xx::UvTcpClient_w, xx::BBuffer*>(L, "Send error! need 2 args: self, BBuffer", LuaKey_UvTcpClient, LuaKey_BBuffer);
		assert(std::get<0>(t));
		var r = std::get<0>(t)->Send(*std::get<1>(t));
		lua_pushinteger(L, r);
		return 1;
	});

	Lua_NewFunc(L, "SendRequest", [](lua_State* L)
	{
		var t = Lua_ToTTFd<xx::UvTcpClient_w, xx::BBuffer*>(L, "SendRequest error! need 4 args: self, BBuffer, callback, timeoutSec", LuaKey_UvTcpClient, LuaKey_BBuffer);
		assert(std::get<0>(t));
		if (!std::get<2>(t).funcId) return luaL_error(L, "SendRequest error! callback can't be null.");
		var r = std::get<0>(t)->SendRequest(*std::get<1>(t), [self = std::get<0>(t), f = std::move(std::get<2>(t))](uint32_t ser, xx::BBuffer* bb)
		{
			if (!self) return;
			if (self->rpcSerials) self->rpcSerials->Remove(ser);

			var L = gLua;
			gFuncId = f.funcId;
			gBBuffer = bb;
			lua_pushcclosure(L, [](lua_State* L)							// cfunc
			{
				lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
				lua_rawgeti(L, 1, gFuncId);									// funcs, func
				if (!gBBuffer)
				{
					lua_call(L, 0, 0);										// funcs, ...?
					lua_settop(L, 0);										// 
					return 0;
				}
				Lua_BBuffer::Create(L);										// funcs, func, bb
				var bb = *(xx::BBuffer**)lua_touserdata(L, -1);
				bb->WriteBuf(gBBuffer->buf + gBBuffer->offset, gBBuffer->readLengthLimit - gBBuffer->offset);

				lua_call(L, 1, 0);											// funcs, ...?
				lua_settop(L, 0);											// 
				return 0;
			}, 0);
			if (int r = lua_pcall(L, 0, 0, 0))								//
			{
				cocos2d::log("%s", lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		}, std::get<3>(t) * 1000);

		if (r)
		{
			if (!std::get<0>(t)->rpcSerials)
			{
				std::get<0>(t)->rpcSerials.MPCreate(std::get<0>(t)->mempool);
			}
			std::get<0>(t)->rpcSerials->Add(r);
		}

		lua_pushinteger(L, r);
		return 1;
	});

	Lua_NewFunc(L, "SendResponse", [](lua_State* L)
	{
		var t = Lua_ToTiT<xx::UvTcpClient_w, xx::BBuffer*>(L, "SendResponse error! need 3 args: self, serial, BBuffer", LuaKey_UvTcpClient, LuaKey_BBuffer);
		assert(std::get<0>(t));
		var r = std::get<0>(t)->SendResponse(std::get<1>(t), *std::get<2>(t));
		lua_pushinteger(L, r);
		return 1;
	});

	Lua_NewFunc(L, "SetAddress", [](lua_State* L)
	{
		var t = Lua_ToTsi<xx::UvTcpClient_w>(L, "SetAddress error! need 3 args: self, ipv4, port", LuaKey_UvTcpClient);
		assert(std::get<0>(t));
		var r = std::get<0>(t)->SetAddress(std::get<1>(t).first, std::get<2>(t));
		lua_pushinteger(L, r);
		return 1;
	});

	Lua_NewFunc(L, "SetAddress6", [](lua_State* L)
	{
		var t = Lua_ToTsi<xx::UvTcpClient_w>(L, "SetAddress6 error! need 3 args: self, ipv6, port", LuaKey_UvTcpClient);
		assert(std::get<0>(t));
		var r = std::get<0>(t)->SetAddress6(std::get<1>(t).first, std::get<2>(t));
		lua_pushinteger(L, r);
		return 1;
	});

	Lua_NewFunc(L, "Connect", [](lua_State* L)
	{
		var t = Lua_ToTd<xx::UvTcpClient_w>(L, "Connect error! need 2 args: self, timeoutMS", LuaKey_UvTcpClient);
		assert(std::get<0>(t));
		var r = std::get<0>(t)->Connect(std::get<1>(t) * 1000);
		lua_pushinteger(L, r);
		return 1;
	});

	Lua_NewFunc(L, "Disconnect", [](lua_State* L)
	{
		var t = Lua_ToTb<xx::UvTcpClient_w>(L, "Disconnect error! need 2 args: self, runCallback:bool", LuaKey_UvTcpClient);
		assert(std::get<0>(t));
		std::get<0>(t)->Disconnect(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "GetState", [](lua_State* L)
	{
		var t = Lua_ToT<xx::UvTcpClient_w>(L, "GetState error! need 1 args: self", LuaKey_UvTcpClient);
		assert(std::get<0>(t));
		lua_pushinteger(L, (int)std::get<0>(t)->state);
		return 1;
	});

	Lua_NewFunc(L, "OnConnect", [](lua_State* L)
	{
		var t = Lua_ToTF<xx::UvTcpClient_w>(L, "OnConnect error! need 2 args: self, func/null", LuaKey_UvTcpClient);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->OnConnect = [self = std::get<0>(t), f = std::move(std::get<1>(t))](int status)
			{
				if (!self) return;
				var L = gLua;
				gFuncId = f.funcId;
				gInteger = status;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					lua_pushinteger(L, gInteger);								// funcs, func, status
					lua_call(L, 1, 0);											// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			std::get<0>(t)->OnConnect = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "OnDisconnect", [](lua_State* L)
	{
		var t = Lua_ToTF<xx::UvTcpClient_w>(L, "OnDisconnect error! need 2 args: self, func/null", LuaKey_UvTcpClient);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->OnDisconnect = [self = std::get<0>(t), f = std::move(std::get<1>(t))]()
			{
				if (!self) return;
				var L = gLua;
				gFuncId = f.funcId;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					lua_call(L, 0, 0);											// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			std::get<0>(t)->OnDisconnect = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "OnReceivePackage", [](lua_State* L)
	{
		var t = Lua_ToTF<xx::UvTcpClient_w>(L, "OnReceivePackage error! need 2 args: self, func(bb)/null", LuaKey_UvTcpClient);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->OnReceivePackage = [self = std::get<0>(t), f = std::move(std::get<1>(t))](xx::BBuffer& bb)
			{
				if (!self) return;
				var L = gLua;
				gFuncId = f.funcId;
				gBBuffer = &bb;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					Lua_BBuffer::Create(L);										// funcs, func, bb
					var bb = *(xx::BBuffer**)lua_touserdata(L, -1);
					bb->WriteBuf(gBBuffer->buf + gBBuffer->offset, gBBuffer->readLengthLimit - gBBuffer->offset);

					lua_call(L, 1, 0);											// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			std::get<0>(t)->OnDisconnect = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "OnReceiveRequest", [](lua_State* L)
	{
		var t = Lua_ToTF<xx::UvTcpClient_w>(L, "OnReceiveRequest error! need 2 args: self, func(bb)/null", LuaKey_UvTcpClient);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->OnReceiveRequest = [self = std::get<0>(t), f = std::move(std::get<1>(t))](uint32_t serial, xx::BBuffer& bb)
			{
				if (!self) return;
				var L = gLua;
				gFuncId = f.funcId;
				gInteger = serial;
				gBBuffer = &bb;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					lua_pushinteger(L, gInteger);								// funcs, func, serial
					Lua_BBuffer::Create(L);										// funcs, func, serial, bb
					var bb = *(xx::BBuffer**)lua_touserdata(L, -1);
					bb->WriteBuf(gBBuffer->buf + gBBuffer->offset, gBBuffer->readLengthLimit - gBBuffer->offset);

					lua_call(L, 2, 0);											// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			std::get<0>(t)->OnDisconnect = nullptr;
		}
		return 0;
	});

	lua_pop(L, 1);													// xx
}
