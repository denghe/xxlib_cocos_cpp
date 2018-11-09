#pragma once

// 这个对象受 lua 管理生命周期. Create 出来之后不需要主动释放
inline void Lua_Register_UvTcpClient(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<xx::UvTcpClient_w>::value);									// xx, t

	Lua_NewFunc(L, "Create", [](lua_State* L)
	{
		var o = uv->CreateTcpClient();
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "__gc", [](lua_State* L)
	{
		var t = Lua_ToTuple<xx::UvTcpClient_w>(L, "__gc error! need 1 args: self");
		assert(std::get<0>(t));
		std::get<0>(t)->Release();
		return 0;
	});

	Lua_NewFunc(L, "Send", [](lua_State* L)
	{
		var t = Lua_ToTuple<xx::UvTcpClient_w, xx::BBuffer*>(L, "Send error! need 2 args: self, BBuffer");
		assert(std::get<0>(t));
		var r = std::get<0>(t)->Send(*std::get<1>(t));
		lua_pushinteger(L, r);
		return 1;
	});

	Lua_NewFunc(L, "SendRequest", [](lua_State* L)
	{
		var t = Lua_ToTuple<xx::UvTcpClient_w, xx::BBuffer*, Lua_Func>(L, "SendRequest error! need 3 ~ 4 args: self, BBuffer, callback, timeoutSec = 0");
		assert(std::get<0>(t));
		if (!std::get<2>(t)) return luaL_error(L, "SendRequest error! callback can't be null.");
		float timeoutSec = 0;
		if (lua_gettop(L) > 3)
		{
			Lua_Get(timeoutSec, L, 4);
		}
		var r = std::get<0>(t)->SendRequest(*std::get<1>(t), [self = std::get<0>(t), f = std::move(std::get<2>(t))](uint32_t ser, xx::BBuffer* bb)
		{
			if (!self || !gLua) return;
			if (self->rpcSerials)
				self->rpcSerials->Remove(ser);

			var L = gLua;
			assert(!lua_gettop(L));
			Lua_Push(L, f);												// func
			if (!bb) Lua_PCall(L, 0);
			else
			{
				Lua_BBuffer::Create(L);									// func, bb
				var Lbb = *(xx::BBuffer**)lua_touserdata(L, -1);
				Lbb->WriteBuf(bb->buf + bb->offset, bb->readLengthLimit - bb->offset);

				Lua_PCall(L, 1);
			}
			lua_settop(L, 0);

		}, timeoutSec);

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
		var t = Lua_ToTuple<xx::UvTcpClient_w, int, xx::BBuffer*>(L, "SendResponse error! need 3 args: self, serial, BBuffer");
		assert(std::get<0>(t));
		var r = std::get<0>(t)->SendResponse(std::get<1>(t), *std::get<2>(t));
		lua_pushinteger(L, r);
		return 1;
	});

	Lua_NewFunc(L, "SetAddress", [](lua_State* L)
	{
		var t = Lua_ToTuple<xx::UvTcpClient_w, const char*, int>(L, "SetAddress error! need 3 args: self, ipv4, port");
		assert(std::get<0>(t));
		var r = std::get<0>(t)->SetAddress(std::get<1>(t), std::get<2>(t));
		lua_pushinteger(L, r);
		return 1;
	});

	Lua_NewFunc(L, "SetAddress6", [](lua_State* L)
	{
		var t = Lua_ToTuple<xx::UvTcpClient_w, const char*, int>(L, "SetAddress6 error! need 3 args: self, ipv6, port");
		assert(std::get<0>(t));
		var r = std::get<0>(t)->SetAddress6(std::get<1>(t), std::get<2>(t));
		lua_pushinteger(L, r);
		return 1;
	});

	Lua_NewFunc(L, "Connect", [](lua_State* L)
	{
		var t = Lua_ToTuple<xx::UvTcpClient_w>(L, "Connect error! need 1 ~ 2 args: self, timeoutSec = 0");
		assert(std::get<0>(t));
		float timeoutSec = 0;
		if (lua_gettop(L) > 3)
		{
			Lua_Get(timeoutSec, L, 4);
		}
		var r = std::get<0>(t)->Connect(timeoutSec * 1000);
		lua_pushinteger(L, r);
		return 1;
	});

	Lua_NewFunc(L, "Disconnect", [](lua_State* L)
	{
		var t = Lua_ToTuple<xx::UvTcpClient_w, bool>(L, "Disconnect error! need 2 args: self, runCallback:bool");
		assert(std::get<0>(t));
		std::get<0>(t)->Disconnect(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "GetState", [](lua_State* L)
	{
		var t = Lua_ToTuple<xx::UvTcpClient_w>(L, "GetState error! need 1 args: self");
		assert(std::get<0>(t));
		lua_pushinteger(L, (int)std::get<0>(t)->state);
		return 1;
	});

	Lua_NewFunc(L, "OnConnect", [](lua_State* L)
	{
		var t = Lua_ToTuple<xx::UvTcpClient_w, Lua_Func>(L, "OnConnect error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->OnConnect = [self = std::get<0>(t), f = std::move(std::get<1>(t))](int status)
			{
				if (!self) return;
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, status);
				lua_settop(gLua, 0);
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
		var t = Lua_ToTuple<xx::UvTcpClient_w, Lua_Func>(L, "OnDisconnect error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->OnDisconnect = [self = std::get<0>(t), f = std::move(std::get<1>(t))]()
			{
				if (!self) return;
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f);
				lua_settop(gLua, 0);
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
		var t = Lua_ToTuple<xx::UvTcpClient_w, Lua_Func>(L, "OnReceivePackage error! need 2 args: self, func(bb)/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->OnReceivePackage = [self = std::get<0>(t), f = std::move(std::get<1>(t))](xx::BBuffer& bb)
			{
				if (!self || !gLua) return;
				assert(!lua_gettop(gLua));
				var L = gLua;
				Lua_Push(L, f);												// func

				Lua_BBuffer::Create(L);										// func, bb
				var Lbb = *(xx::BBuffer**)lua_touserdata(L, -1);
				Lbb->WriteBuf(bb.buf + bb.offset, bb.readLengthLimit - bb.offset);

				Lua_PCall(L, 1);
				lua_settop(gLua, 0);
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
		var t = Lua_ToTuple<xx::UvTcpClient_w, Lua_Func>(L, "OnReceiveRequest error! need 2 args: self, func(serial, bb)/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->OnReceiveRequest = [self = std::get<0>(t), f = std::move(std::get<1>(t))](uint32_t serial, xx::BBuffer& bb)
			{
				if (!self || !gLua) return;
				assert(!lua_gettop(gLua));
				var L = gLua;
				Lua_Pushs(L, f, serial);									// func, serial

				Lua_BBuffer::Create(L);										// func, serial, bb
				var Lbb = *(xx::BBuffer**)lua_touserdata(L, -1);
				Lbb->WriteBuf(bb.buf + bb.offset, bb.readLengthLimit - bb.offset);

				Lua_PCall(L, 2);											//
				lua_settop(gLua, 0);
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
