#pragma once

inline void Lua_Register_UvDialerPeer(lua_State* const& L)
{
	using Peer = xx::UvSerialBBufferSimulatePeer;
	using Peer_s = std::shared_ptr<Peer>;
	using Dialer = xx::UvToGatewayDialer<xx::UvSerialBBufferSimulatePeer>;
	using Dialer_s = std::shared_ptr<Dialer>;
	Lua_NewMT(L, TypeNames<Dialer_s*>::value, TypeNames<xx::UvItem_s*>::value);					// xx.UvDialerPeer ： UvItem



	Lua_NewFunc(L, "PeerAlive", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<Dialer_s*>(L, "PeerAlive error! need 1 args: self");
		assert((*std::get<0>(t)));
		return Lua_Pushs(L, (*std::get<0>(t))->PeerAlive());
	});

	Lua_NewFunc(L, "PeerDispose", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<Dialer_s*>(L, "PeerDispose error! need 1 args: self");
		assert((*std::get<0>(t)));
		(*std::get<0>(t))->PeerDispose();
		return 0;
	});

	Lua_NewFunc(L, "PeerSetTimeoutMS", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<Dialer_s*, int>(L, "PeerSetTimeoutMS error! need 2 args: self, timeoutMS(int)");
		assert((*std::get<0>(t)));
		(*std::get<0>(t))->peerTimeoutMS = std::get<1>(t);
		return 0;
	});

	Lua_NewFunc(L, "PeerSetCheckIntervalMS", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<Dialer_s*, int>(L, "PeerSetCheckIntervalMS error! need 2 args: self, timeoutMS(int)");
		assert((*std::get<0>(t)));
		(*std::get<0>(t))->peerCheckIntervalMS = std::get<1>(t);
		return 0;
	});

	Lua_NewFunc(L, "PeerIsKcp", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<Dialer_s*>(L, "PeerIsKcp error! need 1 args: self");
		assert((*std::get<0>(t)));
		if ((*std::get<0>(t))->peer) {
			auto&& r = (*std::get<0>(t))->peer->IsKcp();
			return Lua_Pushs(L, r);
		}
		else return 0;
	});

	Lua_NewFunc(L, "Cleanup", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<Dialer_s*>(L, "Cleanup error! need 1 args: self");
		assert((*std::get<0>(t)));
		(*std::get<0>(t))->Cancel();
		(*std::get<0>(t))->peer.reset();
		return 0;
	});



	Lua_NewFunc(L, "Create", [](lua_State* L)
	{
		int tcpKcpOpt = 2;
		if (lua_gettop(L) >= 1) {
			Lua_Get(tcpKcpOpt, L, 1);
		}
		auto&& o = xx::TryMake<Dialer>(*uv, tcpKcpOpt);
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

	Lua_NewFunc(L, "Busy", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<Dialer_s*>(L, "Busy error! need 1 args: self");
		assert((*std::get<0>(t)));
		return Lua_Pushs(L, (*std::get<0>(t))->Busy());
	});

	Lua_NewFunc(L, "Cancel", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<Dialer_s*>(L, "Cancel error! need 1 args: self");
		assert((*std::get<0>(t)));
		(*std::get<0>(t))->Cancel();
		return 0;
	});

	Lua_NewFunc(L, "Dial", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<Dialer_s*, std::vector<std::string>, int>(L, "Dial error! need 3/4 args: self, {strings} ipList, int port, int timeoutMS = 0");
		assert((*std::get<0>(t)));
		int timeoutMS = 0;
		if (lua_gettop(L) > 3)
		{
			Lua_Get(timeoutMS, L, 4);
		}
		auto&& rtv = (*std::get<0>(t))->Dial(std::get<1>(t), std::get<2>(t), timeoutMS);
		return Lua_Pushs(L, rtv);
	});

	Lua_NewFunc(L, "OnAccept", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<Dialer_s*, Lua_Func>(L, "OnAccept error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			(*std::get<0>(t))->onAcceptSimulatePeer = [f = std::move(std::get<1>(t))](std::shared_ptr<xx::UvSerialBBufferSimulatePeer> peer)
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
			(*std::get<0>(t))->onAcceptSimulatePeer = nullptr;
		}
		return 0;
	});

	lua_pop(L, 1);																									// xx
}
