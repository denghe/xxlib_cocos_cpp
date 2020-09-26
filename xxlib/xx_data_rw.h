#pragma once
#include "xx_data.h"
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <map>
#include <optional>
#include <cmath>

namespace xx {
	/************************************************************************************/
	// Data 序列化 / 反序列化 基础适配模板
	
	struct DataReader;
	struct DataWriter;

	template<typename T, typename ENABLED = void>
	struct DataFuncs {
		// 整数变长写( 1字节除外 ), double 看情况, float 拷贝内存, 容器先变长写长度
		static inline void Write(DataWriter& dw, T const& in) {
			assert(false);
		}
		// 返回非 0 表示操作失败
		static inline int Read(DataReader& dr, T& out) {
			assert(false);
			return 0;
		}
	};

	/************************************************************************************/
	// Data 读取器 / 反序列化器. 引用模式, 不持有 data
	
	struct DataReader {
		// 引用到内存地址
		char const *buf = nullptr;
		
		// 数据长度
		size_t len = 0;
		
		// 读指针偏移量
		size_t offset = 0;

		DataReader(char const* const& buf, size_t const& len)
			: buf(buf)
			, len(len)
		{
		}

		DataReader(Data const& d) : DataReader(d.buf, d.len) {}

		// 复制构造
		DataReader(DataReader const&) = default;
		DataReader& operator=(DataReader const&) = default;

		// 移动构造
		DataReader(DataReader&& o) {
			this->operator=(std::move(o));
		}
		inline DataReader& operator=(DataReader&& o) {
			std::swap(buf, o.buf);
			std::swap(len, o.len);
			std::swap(offset, o.offset);
			return *this;
		}

		// 重设参数
		inline void Reset(char* const& buf = nullptr, size_t const& len = 0, size_t const& offset = 0) {
			this->buf = buf;
			this->len = len;
			this->offset = offset;
		}
		inline void Reset(Data const& d) {
			Reset(d.buf, d.len);
		}

		// 读指定长度 buf 到 tar. 返回非 0 则读取失败
		int ReadBuf(char* const& tar, size_t const& siz) {
			if (offset + siz > len) return __LINE__;
			memcpy(tar, buf + offset, siz);
			offset += siz;
			return 0;
		}

		// 定长读. 返回非 0 则读取失败
		template<typename T, typename ENABLED = std::enable_if_t<IsPod_v<T>>>
		int ReadFixed(T& v) {
			return ReadBuf((char*)&v, sizeof(T));
		}
		
		// 变长读. 返回非 0 则读取失败
		template<typename T>
		inline int ReadVarInteger(T& v) {
			using UT = std::make_unsigned_t<T>;
			UT u(0);
			for (size_t shift = 0; shift < sizeof(T) * 8; shift += 7) {
				if (offset == len) return __LINE__;
				auto b = (UT)buf[offset++];
				u |= UT((b & 0x7Fu) << shift);
				if ((b & 0x80) == 0) {
					if constexpr (std::is_signed_v<T>) {
						v = ZigZagDecode(u);
					}
					else {
						v = u;
					}
					return 0;
				}
			}
			return __LINE__;
		}

		// 读出并填充到变量. 可同时填充多个. 返回非 0 则读取失败
		template<typename ...TS>
		int Read(TS&...vs) {
			return ReadCore(vs...);
		}

    protected:
        template<typename T, typename ...TS>
        int ReadCore(T& v, TS&...vs) {
            if (auto r = DataFuncs<T>::Read(*this, v)) return r;
            return ReadCore(vs...);
        }
        template<typename T>
        int ReadCore(T& v) {
            return DataFuncs<T>::Read(*this, v);
        }
	};

	// 标识内存可移动
	template<>
	struct IsPod<DataReader, void> : std::true_type {};

    /************************************************************************************/
    // Data 写入器 / 序列化器. 引用模式, 不持有 data

	struct DataWriter {
	    Data& data;
        explicit DataWriter(Data &data) : data(data) {}
        DataWriter(DataWriter const&) = delete;
        DataWriter& operator=(DataWriter const&) = delete;

        // 下面几个直接转发到 data
        inline void WriteBuf(void const* const& ptr, size_t const& siz) {
            data.WriteBuf(ptr, siz);
        }
        template<typename T>
        void WriteFixed(T const& v) {
            data.WriteFixed(v);
        }
        template<typename T>
        void WriteVarIntger(T const& v) {
            data.WriteVarIntger(v);
        }

        // 支持同时写入多个值
        template<typename ...TS>
        void Write(TS const& ...vs) {
            std::initializer_list<int> n{ (DataFuncs<TS>::Write(*this, vs), 0)... };
            (void)n;
        }
	};

    // 标识内存可移动
    template<>
    struct IsPod<DataWriter, void> : std::true_type {};


	/**********************************************************************************************************************/
	// 开始适配 DataFuncs

	// 适配 Data
	template<>
	struct DataFuncs<Data, void> {
		static inline void Write(DataWriter& dw, Data const& in) {
			dw.WriteVarIntger(in.len);
			dw.WriteBuf(in.buf, in.len);
		}
		static inline int Read(DataReader& dr, Data& out) {
            size_t siz = 0;
            if (int r = dr.ReadVarInteger(siz)) return r;
            if (dr.offset + siz > dr.len) return __LINE__;
            out.Clear();
            out.WriteBuf(dr.buf + dr.offset, siz);
            dr.offset += siz;
            return 0;
		}
	};

	// 适配 1 字节长度的 数值 或 float( 这些类型直接 memcpy )
	template<typename T>
	struct DataFuncs<T, std::enable_if_t< (std::is_arithmetic_v<T> && sizeof(T) == 1) || (std::is_floating_point_v<T> && sizeof(T) == 4) >> {
		static inline void Write(DataWriter& dw, T const& in) {
		    dw.WriteFixed(in);
		}
		static inline int Read(DataReader& dr, T& out) {
		    return dr.ReadFixed(out);
		}
	};

	// 适配 2+ 字节整数( 变长读写 )
	template<typename T>
	struct DataFuncs<T, std::enable_if_t<std::is_integral_v<T> && sizeof(T) >= 2>> {
		static inline void Write(DataWriter& dw, T const& in) {
			dw.WriteVarIntger(in);
		}
		static inline int Read(DataReader& dr, T& out) {
			return dr.ReadVarInteger(out);
		}
	};

	// 适配 enum( 根据原始数据类型调上面的适配 )
	template<typename T>
	struct DataFuncs<T, std::enable_if_t<std::is_enum_v<T>>> {
		typedef std::underlying_type_t<T> UT;
		static inline void Write(DataWriter& dw, T const& in) {
			dw.Write((UT const&)in);
		}
		static inline int Read(DataReader& dr, T& out) {
			return dr.Read((UT&)out);
		}
	};

	// 适配 double
	template<>
	struct DataFuncs<double, void> {
		static inline void Write(DataWriter& dw, double const& in) {
		    auto&& d = dw.data;
			d.Reserve(d.len + sizeof(double) + 1);
			if (in == 0) {
				d.buf[d.len++] = 0;
			}
			else if (std::isnan(in)) {
				d.buf[d.len++] = 1;
			}
			else if (in == -std::numeric_limits<double>::infinity()) {	// negative infinity
				d.buf[d.len++] = 2;
			}
			else if (in == std::numeric_limits<double>::infinity()) {	// positive infinity
				d.buf[d.len++] = 3;
			}
			else {
				auto i = (int32_t)in;
				if (in == (double)i) {
					d.buf[d.len++] = 4;
					d.WriteVarIntger<int32_t, false>(i);
				}
				else {
					d.buf[d.len] = 5;
					memcpy(d.buf + d.len + 1, &in, sizeof(double));
					d.len += sizeof(double) + 1;
				}
			}
		}
		static inline int Read(DataReader& dr, double& out) {
			if (dr.offset >= dr.len) return __LINE__;	// 确保还有 1 字节可读
			switch (dr.buf[dr.offset++]) {			// 跳过 1 字节
			case 0:
				out = 0;
				return 0;
			case 1:
				out = std::numeric_limits<double>::quiet_NaN();
				return 0;
			case 2:
				out = -std::numeric_limits<double>::infinity();
				return 0;
			case 3:
				out = std::numeric_limits<double>::infinity();
				return 0;
			case 4: {
				int32_t i = 0;
				if (auto rtv = DataFuncs<int32_t>::Read(dr, i)) return rtv;
				out = i;
				return 0;
			}
			case 5: {
				if (dr.len < dr.offset + sizeof(double)) return __LINE__;
				memcpy(&out, dr.buf + dr.offset, sizeof(double));
                dr.offset += sizeof(double);
				return 0;
			}
			default:
				return __LINE__;							// failed
			}
		}
	};

	// 适配 literal char[len] string  ( 写入 变长长度-1 + 内容. 不写入末尾 0 )
	template<size_t len>
	struct DataFuncs<char[len], void> {
		static inline void Write(DataWriter& dw, char const(&in)[len]) {
			dw.WriteVarIntger((size_t)(len - 1));
			dw.WriteBuf((char*)in, len - 1);
		}
		static inline int Read(DataReader& dr, char(&out)[len]) {
			size_t readLen = 0;
			if (auto r = dr.Read(readLen)) return r;
			if (dr.offset + readLen > dr.len) return __LINE__;
			if (readLen >= len) return __LINE__;
			memcpy(out, dr.buf + dr.offset, readLen);
			out[readLen] = 0;
            dr.offset += readLen;
			return 0;
		}
	};

	// 适配 std::string ( 写入 变长长度 + 内容 )
	template<>
	struct DataFuncs<std::string, void> {
		static inline void Write(DataWriter& dw, std::string const& in) {
			dw.WriteVarIntger(in.size());
			dw.WriteBuf((char*)in.data(), in.size());
		}
		static inline int Read(DataReader& dr, std::string& out) {
            size_t siz = 0;
            if (auto r = dr.ReadVarInteger(siz)) return r;
            if (dr.offset + siz > dr.len) return __LINE__;
            out.assign((char*)dr.buf + dr.offset, siz);
            dr.offset += siz;
            return 0;
		}
	};

	// 适配 std::string_view ( 写入 变长长度 + 内容 )
	template<>
	struct DataFuncs<std::string_view, void> {
		static inline void Write(DataWriter& dw, std::string_view const& in) {
			dw.WriteVarIntger(in.size());
			dw.WriteBuf((char*)in.data(), in.size());
		}
	};

	// 适配 std::optional<T>
	template<typename T>
	struct DataFuncs<std::optional<T>, void> {
		static inline void Write(DataWriter& dw, std::optional<T> const& in) {
			if (in.has_value()) {
                dw.Write((char)1, in.value());
			}
			else {
                dw.Write((char)0);
			}
		}
		static inline int Read(DataReader& dr, std::optional<T>& out) {
			char hasValue = 0;
			if (int r = dr.Read(hasValue)) return r;
			if (!hasValue) return 0;
			if (!out.has_value()) {
				out.emplace();
			}
			return dr.Read(out.value());
		}
	};

	// 适配 std::vector<T>
	template<typename T>
	struct DataFuncs<std::vector<T>, void> {
		static inline void Write(DataWriter& dw, std::vector<T> const& in) {
			auto buf = in.data();
			auto len = in.size();
            auto&& d = dw.data;
			d.Reserve(d.len + 5 + len * sizeof(T));
			d.WriteVarIntger(len);
			if (!len) return;
			if constexpr (sizeof(T) == 1 || std::is_same_v<float, T>) {
				::memcpy(d.buf + d.len, buf, len * sizeof(T));
				d.len += len * sizeof(T);
			}
			else {
				for (size_t i = 0; i < len; ++i) {
                    dw.Write(buf[i]);
				}
			}
		}
		static inline int Read(DataReader& dr, std::vector<T>& out) {
			size_t siz = 0;
			if (auto rtv = dr.Read(siz)) return rtv;
			if (dr.offset + siz > dr.len) return __LINE__;
			out.resize(siz);
			if (siz == 0) return 0;
			auto buf = out.data();
			if constexpr (sizeof(T) == 1 || std::is_same_v<float, T>) {
				::memcpy(buf, dr.buf + dr.offset, siz * sizeof(T));
                dr.offset += siz * sizeof(T);
			}
			else {
				for (size_t i = 0; i < siz; ++i) {
					if (int r = dr.Read(buf[i])) return r;
				}
			}
			return 0;
		}
	};

    // 适配 std::map<K, V>
    template<typename K, typename V>
    struct DataFuncs<std::map<K, V>, void> {
        static inline void Write(DataWriter& dw, std::map<K, V> const& in) {
            auto len = in.size();
            auto&& d = dw.data;
            d.Reserve(d.len + 5 + len * (sizeof(K) + sizeof(V)));
            d.WriteVarIntger(len);
            if (!len) return;
            for(auto&& kv : in) {
                dw.Write(kv.first, kv.second);
            }
        }
        static inline int Read(DataReader& dr, std::map<K, V>& out) {
            size_t siz = 0;
            if (auto rtv = dr.Read(siz)) return rtv;
            if (siz == 0) return 0;
            if (dr.offset + siz * 2 > dr.len) return __LINE__;
            for (size_t i = 0; i < siz; ++i) {
                std::pair<K, V> kv;
                if (int r = dr.Read(kv.first, kv.second)) return r;
                out.insert(std::move(kv));
            }
            return 0;
        }
    };

    // 适配 std::unordered_map<K, V>
    template<typename K, typename V>
    struct DataFuncs<std::unordered_map<K, V>, void> {
        static inline void Write(DataWriter& dw, std::unordered_map<K, V> const& in) {
            auto len = in.size();
            auto&& d = dw.data;
            d.Reserve(d.len + 5 + len * (sizeof(K) + sizeof(V)));
            d.WriteVarIntger(len);
            if (!len) return;
            for(auto&& kv : in) {
                dw.Write(kv.first, kv.second);
            }
        }
        static inline int Read(DataReader& dr, std::unordered_map<K, V>& out) {
            size_t siz = 0;
            if (auto rtv = dr.Read(siz)) return rtv;
            if (siz == 0) return 0;
            if (dr.offset + siz * 2 > dr.len) return __LINE__;
            for (size_t i = 0; i < siz; ++i) {
                std::pair<K, V> kv;
                if (int r = dr.Read(kv.first, kv.second)) return r;
                out.insert(std::move(kv));
            }
            return 0;
        }
    };


    // 适配 std::pair<K, V>
    template<typename K, typename V>
    struct DataFuncs<std::pair<K, V>, void> {
        static inline void Write(DataWriter& dw, std::pair<K, V> const& in) {
            dw.Write(in.first, in.second);
        }
        static inline int Read(DataReader& dr, std::pair<K, V>& out) {
            return dr.Read(out.first, out.second);
        }
    };

    // 适配 std::tuple<T...>
    template<typename...T>
    struct DataFuncs<std::tuple<T...>, void> {
        static inline void Write(DataWriter& dw, std::tuple<T...> const& in) {
            std::apply([&](auto const &... args) {
                dw.Write(args...);
            }, in);
        }
        static inline int Read(DataReader& dr, std::tuple<T...>& out) {
            int rtv = 0;
            std::apply([&](auto&... args) {
                rtv = dr.Read(args...);
            }, out);
            return rtv;
        }
    };
}
