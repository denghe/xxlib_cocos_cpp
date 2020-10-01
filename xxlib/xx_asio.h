#include <asio.hpp>
#include <iostream>
#include <unordered_map>
#include <initializer_list>
#include <algorithm>
#include <string_view>

// todo: 进一步完善。先实现一个 client 功能给 cocos 用

namespace std {
	template <>
	struct hash<asio::ip::address> {
		size_t operator()(asio::ip::address const& v) const {
			if (v.is_v4()) return v.to_v4().to_ulong();
			else if (v.is_v6()) {
				auto p = (uint64_t*)(&v.to_v6().to_bytes());
				return p[0] ^ p[1] ^ p[2] ^ p[3];
			}
			else return 0;
		}
	};
}

namespace xx::Asio {
	struct Context {
		Context() = default;
		Context(Context const&) = delete;
		Context& operator=(Context const&) = delete;

		// 可临时存点啥
		void* userData = nullptr;
		size_t userSize = 0;
		int userInt = 0;

		// asio 上下文
		asio::io_context ioc;

		// 域名 -- 地址列表 映射
		std::unordered_map<std::string, std::vector<asio::ip::address>> domainAddrs;

		// 域名 -- 解析器 映射. 如果不空, 就是有正在解析的
		std::unordered_map<std::string, asio::ip::udp::resolver> domainResolvers;

		// 域名解析
		inline int ResolveDomain(std::string const& domain) {
			auto&& r = domainResolvers.emplace(domain, asio::ip::udp::resolver(ioc));
			if (r.second) {
				r.first->second.async_resolve(domain, "", [this, domain](const asio::error_code& error, asio::ip::udp::resolver::results_type results) {
					if (error.value()) goto LabEnd;
					auto&& as = domainAddrs[domain];
					as.clear();
					for (auto&& r : results) {
						as.push_back(r.endpoint().address());
					}
				LabEnd:
					domainResolvers.erase(domain);
					});
				return 0;
			}
			// 域名正在解析中
			return -1;
		}

		// 打印解析出来的域名
		inline void DumpDomainAddrs() {
			for (auto&& kv : domainAddrs) {
				std::cout << "domain = \"" << kv.first << "\", ip list = {" << std::endl;
				for (auto&& a : kv.second) {
					std::cout << "    " << a << std::endl;
				}
				std::cout << "}" << std::endl;
			}
		}

		// cocos 每帧前后各调用一次
		inline void Update(float delta) {
			ioc.poll();
			// todo: wheel timer logic here
		}

		// 要 dial 的目标地址列表( 带端口 )
		std::unordered_map<asio::ip::address, std::vector<uint16_t>> dialAddrs;

		// 添加拨号地址. 端口合并去重
		inline void AddDialAddrs(asio::ip::address const& a, std::initializer_list<uint16_t> ports_) {
			auto&& ps = dialAddrs[a];
			ps.insert(ps.end(), ports_);
			ps.erase(std::unique(ps.begin(), ps.end()), ps.end());
		}

		// 从 domainAddrs 拿域名对应的 ip 列表, 附加多个端口后放入拨号地址集合
		inline int AddDialAddrs(std::string const& domain, std::initializer_list<uint16_t> ports_) {
			auto&& iter = domainAddrs.find(domain);
			if (iter == domainAddrs.end()) return -2;
			for (auto&& a : iter->second) {
				AddDialAddrs(a, ports_);
			}
		}


		// todo: std::unordered_map<??, Connector> connectors;

		// 根据 dialAddrs 的配置，对这些 ip:port 同时发起拨号( 带超时 ), 最后留一个最先连上的
		void Dial(float const& timeoutSeconds);
	};

	// todo: struct Connector

	inline void Context::Dial(float const& timeoutSeconds) {
		// todo: cleanup
		for (auto&& kv : dialAddrs) {
			for (auto&& port : kv.second) {
				auto ep = asio::ip::udp::endpoint(kv.first, port);
				// todo: create Connector( p )
			}
		}
	}
}


//std::shared_ptr<asio::ip::udp::socket> sock;// (io_service, udp::endpoint(udp::v4(), 0));
//// 收到 udp 数据时的目标地址( 两个策略： 1. 数据向 toAddr 发。 2. 向 fromAddr 发 )
//asio::ip::udp::endpoint fromAddr;
//// 最终选定的当前唯一通信用目标地址( 带端口 )
//asio::ip::udp::endpoint tarAddr;

//asio::ip::udp::resolver::query query(asio::ip::udp::v4(), "127.0.0.1", "12345");
//sock->send_to(asio::buffer("asdfqwer", 8), send_to_addr);

//void handle_receive_from(const asio::error_code& error, size_t bytes_recvd);

//void HelloWorld::handle_receive_from(const asio::error_code& error, size_t bytes_recvd) {
//	std::cout << "error = " << error << ", recv " << bytes_recvd << std::endl;
//	sock->async_receive_from(asio::buffer(data_, max_length), sender_endpoint, bind(&HelloWorld::handle_receive_from, this,
//		std::placeholders::_1,
//		std::placeholders::_2));
//}
