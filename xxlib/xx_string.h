#pragma once

#include <cassert>
#include <string>
#include <type_traits>
#include <optional>
#include <vector>
#include <tuple>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <sstream>
#include <iostream>
#include <ctime>  // std::tm
#include <iomanip>  // std::put_time
#include "xx_data.h"

namespace xx {
    // 各种 std::string 辅助函数

    /************************************************************************************/
    // StringFuncs 适配模板 for easy to append value to std::string
    /************************************************************************************/

    template<typename T, typename ENABLED = void>
    struct StringFuncs {
        static inline void Append(std::string &s, T const &in) {
            assert(false);
        }

        static inline void AppendCore(std::string &s, T const &in) {
            assert(false);
        }
    };



    /************************************************************************************/
    // Append / ToString
    /************************************************************************************/

    namespace Core {
        template<typename T>
        void Append(std::string &s, T const &v) {
            ::xx::StringFuncs<T>::Append(s, v);
        }
    }

    template<typename ...Args>
    void Append(std::string &s, Args const &... args) {
        std::initializer_list<int> n{((::xx::Core::Append(s, args)), 0)...};
        (void) (n);
    }

    template<typename ...Args>
    std::string ToString(Args const &... args) {
        std::string s;
        Append(s, args...);
        return s;
    }

    /************************************************************************************/
    // StringFuncs 继续适配各种常见数据类型
    /************************************************************************************/

    // 适配 char const* \0 结尾 字串
    template<>
    struct StringFuncs<char const *, void> {
        static inline void Append(std::string &s, char const *const &in) {
            s.append(in ? in : "null");
        }
    };

    // 适配 char* \0 结尾 字串
    template<>
    struct StringFuncs<char *, void> {
        static inline void Append(std::string &s, char *const &in) {
            s.append(in ? in : "null");
        }
    };

    // 适配 literal char[len] string
    template<size_t len>
    struct StringFuncs<char[len], void> {
        static inline void Append(std::string &s, char const(&in)[len]) {
            s.append(in);
        }
    };

    // 适配 std::string( 前后加引号 )
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_base_of_v<std::string, T>>> {
        static inline void Append(std::string &s, T const &in) {
            s.push_back('\"');
            s.append(in);
            s.push_back('\"');
        }
    };

    // 适配 std::string_view( 前后加引号 )
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_base_of_v<std::string_view, T>>> {
        static inline void Append(std::string &s, T const &in) {
            s.push_back('\"');
            s.append(in);
            s.push_back('\"');
        }
    };

    // 适配 type_info     typeid(T)
    template<>
    struct StringFuncs<std::type_info, void> {
        static inline void Append(std::string &s, std::type_info const &in) {
            s.push_back('\"');
            s.append(in.name());
            s.push_back('\"');
        }
    };

    // 适配所有数字
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
        static inline void Append(std::string &s, T const &in) {
            if constexpr (std::is_same_v<bool, T>) {
                s.append(in ? "true" : "false");
            } else if constexpr (std::is_same_v<char, T>) {
                s.push_back(in);
            } else if constexpr (std::is_floating_point_v<T>) {
                char buf[32];
                sprintf(buf, "%g", (double) in);
                s.append(buf);
            } else {
                s.append(std::to_string(in));
            }
        }
    };

    // 适配 enum( 根据原始数据类型调上面的适配 )
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_enum_v<T>>> {
        static inline void Append(std::string &s, T const &in) {
            s.append(std::to_string((std::underlying_type_t<T>) in));
        }
    };

    // 适配 TimePoint
    template<typename C, typename D>
    struct StringFuncs<std::chrono::time_point<C, D>, void> {
        static inline void Append(std::string &s, std::chrono::time_point<C, D> const &in) {
            auto &&t = std::chrono::system_clock::to_time_t(in);
            std::stringstream ss;
            std::tm tm{};
#ifdef _WIN32
            localtime_s(&tm, &t);
#else
            localtime_r(&t, &tm);
#endif
            ss << std::put_time(&tm, "%F %T");
            s.append(ss.str());
        }
    };

    // 适配 std::optional<T>
    template<typename T>
    struct StringFuncs<std::optional<T>, void> {
        static inline void Append(std::string &s, std::optional<T> const &in) {
            if (in.has_value()) {
                ::xx::Append(s, in.value());
            } else {
                s.append("null");
            }
        }
    };

    // 适配 std::vector<T>
    template<typename T>
    struct StringFuncs<std::vector<T>, void> {
        static inline void Append(std::string &s, std::vector<T> const &in) {
            s.push_back('[');
            if (auto inLen = in.size()) {
                for (size_t i = 0; i < inLen; ++i) {
                    ::xx::Append(s, in[i]);
                    s.push_back(',');
                }
                s[s.size() - 1] = ']';
            } else {
                s.push_back(']');
            }
        }
    };

    // 适配 std::unordered_set<T>
    template<typename T>
    struct StringFuncs<std::unordered_set<T>, void> {
        static inline void Append(std::string &s, std::unordered_set<T> const &in) {
            s.push_back('[');
            if (!in.empty()) {
                for (auto &&o : in) {
                    ::xx::Append(s, o);
                    s.push_back(',');
                }
                s[s.size() - 1] = ']';
            } else {
                s.push_back(']');
            }
        }
    };

    // 适配 std::unordered_map<K, V>
    template<typename K, typename V>
    struct StringFuncs<std::unordered_map<K, V>, void> {
        static inline void Append(std::string &s, std::unordered_map<K, V> const &in) {
            s.push_back('[');
            if (!in.empty()) {
                for (auto &kv : in) {
                    ::xx::Append(s, kv.first);
                    s.push_back(',');
                    ::xx::Append(s, kv.second);
                    s.push_back(',');
                }
                s[s.size() - 1] = ']';
            } else {
                s.push_back(']');
            }
        }
    };

    // 适配 std::map<K, V>
    template<typename K, typename V>
    struct StringFuncs<std::map<K, V>, void> {
        static inline void Append(std::string &s, std::map<K, V> const &in) {
            s.push_back('[');
            if (!in.empty()) {
                for (auto &kv : in) {
                    ::xx::Append(s, kv.first);
                    s.push_back(',');
                    ::xx::Append(s, kv.second);
                    s.push_back(',');
                }
                s[s.size() - 1] = ']';
            } else {
                s.push_back(']');
            }
        }
    };

    // 适配 std::pair<K, V>
    template<typename K, typename V>
    struct StringFuncs<std::pair<K, V>, void> {
        static inline void Append(std::string &s, std::pair<K, V> const &in) {
            s.push_back('[');
            ::xx::Append(s, in.first);
            s.push_back(',');
            ::xx::Append(s, in.second);
            s.push_back(']');
        }
    };

    // 适配 std::tuple<......>
    template<typename...T>
    struct StringFuncs<std::tuple<T...>, void> {
        static inline void Append(std::string &s, std::tuple<T...> const &in) {
            s.push_back('[');
            std::apply([&](auto const &... args) {
                std::initializer_list<int> n{((::xx::Append(s, args, ',')), 0)...};
                (void(n));
                if constexpr(sizeof...(args)) {
                    s.resize(s.size() - 1);
                }
            }, in);
            s.push_back(']');
        }
    };

    // 适配 Data
    template<>
    struct StringFuncs<Data, void> {
        static inline void Append(std::string &s, Data const &in) {
            s.push_back('[');
            if (auto inLen = in.len) {
                for (size_t i = 0; i < inLen; ++i) {
                    ::xx::Append(s, (uint8_t) in[i]);
                    s.push_back(',');
                }
                s[s.size() - 1] = ']';
            } else {
                s.push_back(']');
            }
        }
    };

    // 适配 DataView
    template<>
    struct StringFuncs<DataView, void> {
        static inline void Append(std::string &s, DataView const &in) {
            s.push_back('[');
            if (auto inLen = in.len) {
                for (size_t i = 0; i < inLen; ++i) {
                    ::xx::Append(s, (uint8_t) in.buf[i]);
                    s.push_back(',');
                }
                s[s.size() - 1] = ']';
            } else {
                s.push_back(']');
            }
        }
    };


    /************************************************************************************/
    // string 处理相关
    /************************************************************************************/

    // 转换 s 数据类型 为 T 填充 dst
    template<typename T>
    inline void Convert(char const *const &s, T &dst) {
        if (!s) {
            dst = T();
        } else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T> && sizeof(T) <= 4) {
            dst = (T) strtoul(s, nullptr, 0);
        } else if constexpr (std::is_integral_v<T> && !std::is_unsigned_v<T> && sizeof(T) <= 4) {
            dst = (T) atoi(s);
        } else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T> && sizeof(T) == 8) {
            dst = strtoull(s, nullptr, 0);
        } else if constexpr (std::is_integral_v<T> && !std::is_unsigned_v<T> && sizeof(T) == 8) {
            dst = atoll(s);
        } else if constexpr (std::is_floating_point_v<T> && sizeof(T) == 4) {
            dst = strtof(s, nullptr);
        } else if constexpr (std::is_floating_point_v<T> && sizeof(T) == 8) {
            dst = atof(s);
        } else if constexpr (std::is_same_v<T, bool>) {
            dst = s[0] == '1' || s[0] == 't' || s[0] == 'T' || s[0] == 'y' || s[0] == 'Y';
        } else if constexpr (std::is_same_v<T, std::string>) {
            dst = s;
        }
        // todo: more
    }

    inline int FromHex(uint8_t const &c) {
        if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
        else if (c >= 'a' && c <= 'z') return c - 'a' + 10;
        else if (c >= '0' && c <= '9') return c - '0';
        else return 0;
    }

    inline void UrlDecode(std::string const &src, std::string &dst) {
        auto &&length = src.size();
        dst.clear();
        dst.reserve(length);
        for (size_t i = 0; i < length; i++) {
            if (src[i] == '+') {
                dst += ' ';
            } else if (src[i] == '%') {
                if (i + 2 >= length) return;
                auto high = FromHex(src[i + 1]);
                auto low = FromHex(src[i + 2]);
                i += 2;
                dst += ((char) (uint8_t) (high * 16 + low));
            } else dst += src[i];
        }
    }

    inline std::string UrlDecode(std::string const &src) {
        std::string rtv;
        ::xx::UrlDecode(src, rtv);
        return rtv;
    }

    inline void ToHex(uint8_t const &c, uint8_t &h1, uint8_t &h2) {
        auto a = c / 16;
        auto b = c % 16;
        h1 = (uint8_t) (a + ((a <= 9) ? '0' : ('a' - 10)));
        h2 = (uint8_t) (b + ((b <= 9) ? '0' : ('a' - 10)));
    }

    inline void UrlEncode(std::string const &src, std::string &dst) {
        auto &&str = src.c_str();
        auto &&siz = src.size();
        dst.clear();
        dst.reserve(siz * 2);
        for (size_t i = 0; i < siz; ++i) {
            char c = str[i];
            if ((c >= '0' && c <= '9') ||
                (c >= 'a' && c <= 'z') ||
                (c >= 'A' && c <= 'Z') ||
                c == '-' || c == '_' || c == '.' || c == '!' || c == '~' ||
                c == '*' || c == '\'' || c == '(' || c == ')') {
                dst += c;
            } else if (c == ' ') {
                dst += '+';
            } else {
                dst += '%';
                uint8_t h1, h2;
                ToHex(c, h1, h2);
                dst += h1;
                dst += h2;
            }
        }
    }

    inline std::string UrlEncode(std::string const &src) {
        std::string rtv;
        ::xx::UrlEncode(src, rtv);
        return rtv;
    }


    inline void HtmlEncode(std::string const &src, std::string &dst) {
        auto &&str = src.c_str();
        auto &&siz = src.size();
        dst.clear();
        dst.reserve(siz * 2);    // 估算. * 6 感觉有点浪费
        for (size_t i = 0; i < siz; ++i) {
            auto c = str[i];
            switch (c) {
                case '&':
                    dst.append("&amp;");
                    break;
                case '\"':
                    dst.append("&quot;");
                    break;
                case '\'':
                    dst.append("&apos;");
                    break;
                case '<':
                    dst.append("&lt;");
                    break;
                case '>':
                    dst.append("&gt;");
                    break;
                default:
                    dst += c;
                    break;
            }
        }
    }

    inline std::string HtmlEncode(std::string const &src) {
        std::string rtv;
        ::xx::HtmlEncode(src, rtv);
        return rtv;
    }


    /************************************************************************************/
    // 各种 Cout
    /************************************************************************************/

    // 替代 std::cout. 支持实现了 StringFuncs 模板适配的类型
    template<typename...Args>
    inline void Cout(Args const &...args) {
        std::string s;
        Append(s, args...);
        for (auto &&c : s) {
            if (!c) c = '^';
        }
        std::cout << s;
    }

    // 在 Cout 基础上添加了换行
    template<typename...Args>
    inline void CoutN(Args const &...args) {
        Cout(args...);
        std::cout << std::endl;
    }

    // 在 CoutN 基础上于头部添加了时间
    template<typename...Args>
    inline void CoutTN(Args const &...args) {
        CoutN("[", std::chrono::system_clock::now(), "] ", args...);
    }

    // 立刻输出
    inline void CoutFlush() {
        std::cout.flush();
    }


    /************************************************************************************/
    // 各种工具函数
    /************************************************************************************/

    // 获取 1, 2 级文件扩展名
    inline std::pair<std::string_view, std::string_view> GetFileNameExts(std::string const& fn) {
        std::pair<std::string_view, std::string_view> rtv;
        auto dotPos = fn.rfind('.');
        auto extLen = fn.size() - dotPos;
        rtv.first = std::string_view(fn.data() + dotPos, extLen);
        if (dotPos) {
            dotPos = fn.rfind('.', dotPos - 1);
            if(dotPos != std::string::npos) {
                extLen = fn.size() - dotPos - extLen;
                rtv.second = std::string_view(fn.data() + dotPos, extLen);
            }
        }
        return rtv;
    }
}
