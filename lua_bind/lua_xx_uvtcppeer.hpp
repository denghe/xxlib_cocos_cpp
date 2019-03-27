#pragma once

inline void Lua_Register_UvTcpLuaPeer(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<xx::UvTcpLuaPeer_s>::value, TypeNames<std::shared_ptr<xx::UvItem>>::value);					// xx.UvTcpDialer ： UvItem

	Lua_NewFunc(L, "__gc", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvItem_s>(L, "__gc error! need 1 args: self");
		assert(std::get<0>(t));
		std::get<0>(t).~shared_ptr();
		return 0;
	});

	Lua_NewFunc(L, "GetIP", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvTcpLuaPeer_s, bool>(L, "Send error! need 2 args: self, bool includePort");
		assert(std::get<0>(t));
		std::string s;
		xx::Uv::FillIP(std::get<0>(t)->uvTcp, s, std::get<1>(t));
		return Lua_Pushs(L, s);
	});

	Lua_NewFunc(L, "SendPush", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvTcpLuaPeer_s, xx::BBuffer*>(L, "Send error! need 2 args: self, BBuffer");
		assert(std::get<0>(t));
		auto&& r = std::get<0>(t)->SendPush(*std::get<1>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "SendResponse", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvTcpLuaPeer_s, int, xx::BBuffer*>(L, "SendResponse error! need 3 args: self, serial, BBuffer");
		assert(std::get<0>(t));
		auto&& r = std::get<0>(t)->SendResponse(std::get<1>(t), *std::get<2>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "SendRequest", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvTcpLuaPeer_s, xx::BBuffer*, Lua_Func>(L, "SendRequest error! need 3 ~ 4 args: self, BBuffer, callback, timeoutMS = 0");
		assert(std::get<0>(t));
		int timeoutMS = 0;
		if (lua_gettop(L) > 3)
		{
			Lua_Get(timeoutMS, L, 4);
		}
		auto&& rtv = std::get<0>(t)->SendRequest(*std::get<1>(t), [f = std::move(std::get<2>(t))](xx::BBuffer* const& data) {
			if (!gLua) return -1;
			auto&& L = gLua;
			assert(!lua_gettop(L));

			Lua_Push(L, f);												// func
			if (!data) {
				Lua_PCall(L, 0);
				lua_settop(L, 0);
				return -1;
			}
			else
			{
				Lua_BBuffer::Create(L);									// func, data
				auto&& Lbb = *(xx::BBuffer**)lua_touserdata(L, -1);
				Lbb->AddRange(data->buf + data->offset, data->len - data->offset);

				Lua_PCall(L, 1);
				if (lua_gettop(L)) {
					auto&& rt = Lua_ToTuple<int>(gLua, "SendRequest's callback function error! need 1 results: int");
					lua_settop(L, 0);
					return std::get<0>(rt);
				}
				return 0;
			}
		}, timeoutMS);
		return Lua_Pushs(L, rtv);
	});

	Lua_NewFunc(L, "OnDisconnect", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvTcpLuaPeer_s, Lua_Func>(L, "OnDisconnect error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->OnDisconnect = [f = std::move(std::get<1>(t))]()
			{
				if (!gLua) return;
				assert(!lua_gettop(gLua));
				auto&& L = gLua;

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

	Lua_NewFunc(L, "OnReceivePush", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvTcpLuaPeer_s, Lua_Func>(L, "OnReceivePackage error! need 2 args: self, func(bb)/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->OnReceivePush = [f = std::move(std::get<1>(t))](xx::BBuffer& data)
			{
				if (!gLua) return -1;
				assert(!lua_gettop(gLua));
				auto&& L = gLua;

				Lua_Push(L, f);												// func
				Lua_BBuffer::Create(L);										// func, bb
				auto&& Lbb = *(xx::BBuffer**)lua_touserdata(L, -1);
				Lbb->AddRange(data.buf + data.offset, data.readLengthLimit - data.offset);

				Lua_PCall(L, 1);											// rtv?
				if (lua_gettop(L)) {
					auto&& rt = Lua_ToTuple<int>(gLua, "SendRequest's callback function error! need 1 results: int");
					lua_settop(L, 0);
					return std::get<0>(rt);
				}
				return 0;
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
		auto&& t = Lua_ToTuple<xx::UvTcpLuaPeer_s, Lua_Func>(L, "OnReceiveRequest error! need 2 args: self, func(bb)/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->OnReceiveRequest = [f = std::move(std::get<1>(t))](int const& serial, xx::BBuffer& data)
			{
				if (!gLua) return -1;
				assert(!lua_gettop(gLua));
				auto&& L = gLua;

				Lua_Pushs(L, f, serial);									// func, serial
				Lua_BBuffer::Create(L);										// func, serial, bb
				auto&& Lbb = *(xx::BBuffer**)lua_touserdata(L, -1);
				Lbb->AddRange(data.buf + data.offset, data.readLengthLimit - data.offset);

				Lua_PCall(L, 2);											// rtv?
				if (lua_gettop(L)) {
					auto&& rt = Lua_ToTuple<int>(gLua, "SendRequest's callback function error! need 1 results: int");
					lua_settop(L, 0);
					return std::get<0>(rt);
				}
				return 0;
			};
		}
		else
		{
			std::get<0>(t)->OnDisconnect = nullptr;
		}
		return 0;
	});

	lua_pop(L, 1);																									// xx
}

