#pragma once

namespace xx {
	// send & recv pure data ( do not encode/decode )
	struct UvTcpLuaPeer : UvTcpBasePeer {
		using RpcFunc = std::function<int(BBuffer* const& data)>;
		std::unordered_map<int, std::pair<RpcFunc, int64_t>> callbacks;
		int serial = 0;
		std::function<int(BBuffer& data)> OnReceivePush;
		inline virtual int ReceivePush(BBuffer& data) noexcept { return OnReceivePush ? OnReceivePush(data) : 0; };
		std::function<int(int const& serial, BBuffer& data)> OnReceiveRequest;
		inline virtual int ReceiveRequest(int const& serial, BBuffer& data) noexcept { return OnReceiveRequest ? OnReceiveRequest(serial, data) : 0; };

		UvTcpLuaPeer(Uv& uv)
			: UvTcpBasePeer(uv) {
		}
		UvTcpLuaPeer(UvTcpLuaPeer const&) = delete;
		UvTcpLuaPeer& operator=(UvTcpLuaPeer const&) = delete;
		~UvTcpLuaPeer() { this->Dispose(0); }

		inline int SendPush(BBuffer const& data) {
			return SendPackage(data);
		}
		inline int SendResponse(int32_t const& serial, BBuffer const& data) {
			return SendPackage(data, serial);
		}
		inline int SendRequest(BBuffer const& data, RpcFunc&& cb, uint64_t const& timeoutMS = 0) {
			if (!uvTcp) return -1;
			std::pair<RpcFunc, int64_t> v;
			serial = (serial + 1) & 0x7FFFFFFF;			// uint circle use
			if (timeoutMS) {
				v.second = uv.nowMS + timeoutMS;
			}
			if (int r = SendPackage(data, -serial)) return r;
			v.first = std::move(cb);
			callbacks[serial] = std::move(v);
			return 0;
		}

		inline virtual void Dispose(int const& flag = 1) noexcept override {
			if (!uvTcp) return;
			for (auto&& kv : callbacks) {
				kv.second.first(nullptr);
			}
			callbacks.clear();
			this->UvTcp::Dispose(flag);
			if (flag) {
				auto holder = shared_from_this();
				Disconnect();
				OnDisconnect = nullptr;
				OnReceivePush = nullptr;
				OnReceiveRequest = nullptr;
			}
		}

	protected:
		virtual int HandlePack(uint8_t* const& recvBuf, uint32_t const& recvLen) noexcept override {
			auto& recvBB = uv.recvBB;
			recvBB.Reset((uint8_t*)recvBuf, recvLen);

			int serial = 0;
			if (int r = recvBB.Read(serial)) return r;

			if (serial == 0) {
				return ReceivePush(recvBB);
			}
			else if (serial < 0) {
				return ReceiveRequest(-serial, recvBB);
			}
			else {
				auto&& iter = callbacks.find(serial);
				if (iter == callbacks.end()) return 0;
				auto&& a = std::move(iter->second.first);
				callbacks.erase(iter);
				return a(&recvBB);
			}
		}

		// serial == 0: push    > 0: response    < 0: request
		inline int SendPackage(BBuffer const& data, int32_t const& serial = 0) {
			if (!uvTcp) return -1;
			auto& sendBB = uv.sendBB;
			static_assert(sizeof(uv_write_t_ex) + 4 <= 1024);
			sendBB.Reserve(1024);
			sendBB.len = sizeof(uv_write_t_ex) + 4;		// skip uv_write_t_ex + header space
			sendBB.Write(serial);
			sendBB.AddRange(data.buf, data.len);

			auto buf = sendBB.buf;						// cut buf memory for send
			auto len = sendBB.len - sizeof(uv_write_t_ex) - 4;
			sendBB.buf = nullptr;
			sendBB.len = 0;
			sendBB.cap = 0;

			return SendReqAndData(buf, (uint32_t)len);
		}

		inline virtual void Update(int64_t const& nowMS) noexcept override {
			if (!uvTcp) return;
			if (this->timeoutMS && this->timeoutMS < nowMS) {
				this->Dispose();
				return;
			}
			for (auto&& iter_ = callbacks.begin(); iter_ != callbacks.end();) {
				auto&& iter = iter_++;
				if (iter->second.second < nowMS) {
					auto&& a = std::move(iter->second.first);
					callbacks.erase(iter);
					a(nullptr);
				}
			}
		}
	};
}


// 向 lua 映射 xx.Uv 表/元表
inline void Lua_Register_Uv(lua_State* const& L)
{
	// Uv

	Lua_NewMT(L, LuaKey_Uv);													// xx, Uv

	// todo: Stop ?

	lua_pop(L, 1);																// xx



	// UvItem

	Lua_NewMT(L, TypeNames<xx::UvItem_s>::value);								// xx, UvItem

	Lua_NewFunc(L, "Dispose", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvItem_s>(L, "Dispose error! need 1 args: self");
		assert(std::get<0>(t));
		std::get<0>(t)->Dispose(1);
		return 0;
	});

	Lua_NewFunc(L, "Disposed", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<xx::UvItem_s>(L, "Disposed error! need 1 args: self");
		assert(std::get<0>(t));
		return Lua_Pushs(L, std::get<0>(t)->Disposed());
	});

	Lua_NewFunc(L, "__gc", [](lua_State* L)										// todo: 如果这个不起作用，就复制到每个派生 mt
	{
		auto&& t = Lua_ToTuple<xx::UvItem_s>(L, "__gc error! need 1 args: self");
		assert(std::get<0>(t) && std::get<0>(t).use_count() == 1);
		delete &std::get<0>(t);
		return 0;
	});

	lua_pop(L, 1);																// xx
}
