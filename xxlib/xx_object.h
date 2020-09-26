#pragma once

#include "xx_data_rw.h"
#include "xx_string.h"
#include <unordered_map>
#include <array>

namespace xx {
    /************************************************************************************/
    // Data 序列化 / 反序列化 基础适配模板 for Object, std::shared_ptr, std::weak_ptr

    struct DataReaderEx;
    struct DataWriterEx;
    struct ObjectHelper;
    struct Object;

    // 默认转发到 DataFunc 模板
    template<typename T, typename ENABLED = void>
    struct DataFuncsEx {
        static inline void Write(DataWriterEx &dw, T const &in) {
            DataFuncs<T>::Write(dw, in);
        }

        static inline int Read(DataReaderEx &dr, T &out) {
            return DataFuncs<T>::Read(dr, out);
        }
    };

    // 默认转发到 StringFuncs 模板
    template<typename T, typename ENABLED = void>
    struct StringFuncsEx {
        static void Append(ObjectHelper &oh, T const &in);
    };

    // 这是新的适配模板 for clone 功能
    template<typename T, typename ENABLED = void>
    struct CloneFuncs {
        static inline void Clone1(ObjectHelper &oh, T const &in, T &out) {
            out = in;
        }

        static inline void Clone2(ObjectHelper &oh, T const &in, T &out) {
            // do nothing
        }
    };

    /************************************************************************************/
    // 为 Object 序列化 克隆 值相等判定 转字符串 功能 提供上下文支撑( 主要是类创建，字典防递归 )

    struct ObjectHelper {
        ObjectHelper() = default;

        ObjectHelper(ObjectHelper const &) = default;

        ObjectHelper &operator=(ObjectHelper const &) = default;

        // 反序列化时的创建函数 类型
        typedef std::shared_ptr<Object>(*CreateFunc)();

        // 创建函数数组 类型
        typedef std::array<CreateFunc, 1u << (sizeof(uint16_t) * 8)> CreateFuncs;

        // 创建函数容器
        CreateFuncs createFuncs{};

        // 创建相应 typeId 的创建函数( 通常调用该函数的函数体位于序列化生成物 )
        template<typename T, typename ENABLED = std::enable_if_t<std::is_base_of_v<Object, T>>>
        void Register(uint16_t const &typeId = TypeId_v<T>) {
            createFuncs[typeId] = []() -> std::shared_ptr<Object> {
                try {
                    return std::make_shared<T>();
                }
                catch (...) {
                    return std::shared_ptr<T>();
                }
            };
        }

        // 根据 typeId 来创建对象
        inline std::shared_ptr<Object> CreateByTypeId(uint16_t const &typeId) {
            if (!createFuncs[typeId]) return nullptr;
            return createFuncs[typeId]();
        }

        // 通过 offset 定位 obj
        std::unordered_map<size_t, std::shared_ptr<Object>> offsetObjs;

        // 通过 obj 定位 offset
        std::unordered_map<void *, size_t> objOffsets;

        // 克隆过程中用于映射 ptr 关系. 便于 weak 调整指向
        std::unordered_map<void *, std::shared_ptr<Object>> oldNewObjs;

        // ToString 填充用 $
        std::string s;
        std::string s1;
        std::string s2;

        // len / offset backup
        int bak = 0;

        // 对于一些本来返回 int 的函数但是改变了返回值，那就将那个 int 存放到此备查
        int lastRtv = 0;

        // 将一个东西写入 data
        template<typename T>
        void WriteTo(Data &d, T const &v);

        // 从 data 读出一个东西
        template<typename T>
        int ReadFrom(Data &d, T &v);

        std::shared_ptr<Object> ReadFrom(Data &d);

        // s1 = a.ToString(), s2 = b.ToString(), return memcmp(s1, s2)
        template<typename T>
        int Compare(T const &a, T const &b);

        // 简单输出 s1, s2 的不同之处
        void CoutCompareResult();

        // 调用适配模板实现 2 pass clone
        // step 1: clone data + shared_ptrs( create old new map )
        // step 2: clone weak_ptrs( from old new map )
        template<typename T>
        void Clone(T const &in, T &out);

        template<typename T>
        T Clone(T const &in);

        template<typename...Args>
        std::string const &ToString(Args const &...args) {
            s.clear();
            objOffsets.clear();
            AppendEx(*this, args...);
            return s;
        }

        // 替代 std::cout. 支持实现了 StringFuncsEx 模板适配的类型
        template<typename...Args>
        inline void Cout(Args const &...args) {
            ToString(args...);
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

        // 向 s 追加 string
        template<typename ...Args>
        void Append(Args const &... args);
    };

    template<typename T, typename ENABLED>
    inline void StringFuncsEx<T, ENABLED>::Append(ObjectHelper &oh, T const &in) {
        ::xx::StringFuncs<T>::Append(oh.s, in);
    };

    /************************************************************************************/
    // Append / ToString
    /************************************************************************************/

    namespace Core {
        template<typename T>
        void AppendEx(ObjectHelper &oh, T const &v) {
            ::xx::StringFuncsEx<T>::Append(oh, v);
        }
    }

    template<typename ...Args>
    void AppendEx(ObjectHelper &oh, Args const &... args) {
        std::initializer_list<int> n{((::xx::Core::AppendEx(oh, args)), 0)...};
        (void) (n);
    }

    template<typename ...Args>
    std::string const &ToStringEx(ObjectHelper &oh, Args const &... args) {
        AppendEx(oh, args...);
        return oh.s;
    }

    template<typename ...Args>
    void ObjectHelper::Append(Args const &... args) {
        AppendEx(*this, args...);
    }


    /************************************************************************************/
    // Object 主要用于满足 无脑智能指针堆业务逻辑 的建模与序列化需求

    struct Object : std::enable_shared_from_this<Object> {
        Object() = default;

        virtual ~Object() = default;

        // 序列化时获取 typeId( 如果 Object 以值类型方式使用则无需获取 typeId )
        inline virtual uint16_t GetTypeId() const { return 0; };

        // 序列化正文
        inline virtual void Serialize(DataWriterEx &dw) const {};

        // 反序列化( 填充正文 )
        inline virtual int Deserialize(DataReaderEx &dr) { return 0; };

        // 输出 json 长相时用于输出外包围 {  } 部分
        inline virtual void ToString(ObjectHelper &oh) const {};

        // 输出 json 长相时用于输出花括号内部的成员拼接
        inline virtual void ToStringCore(ObjectHelper &oh) const {};

        // 克隆步骤1: 拷贝普通数据，遇到 shared_ptr 就同型新建, 并保存映射关系
        inline virtual void Clone1(ObjectHelper &oh, std::shared_ptr<Object> const &tar) const {};

        // 克隆步骤2: 只处理成员中的 weak_ptr 类型。根据步骤 1 建立的映射关系来填充
        inline virtual void Clone2(ObjectHelper &oh, std::shared_ptr<Object> const &tar) const {};
    };

    /************************************************************************************/
    // 序列化器
    struct DataWriterEx : DataWriter {
        // 引用到 Object 功能辅助器
        ObjectHelper &oh;

        DataWriterEx(Data &data, ObjectHelper &oh)
                : DataWriter(data), oh(oh) {
        }

        // 支持同时写入多个值. 覆盖基类的实现，以确保走 DataFuncsEx 适配模板
        template<typename ...TS>
        void Write(TS const &...vs) {
            std::initializer_list<int> n{(DataFuncsEx<TS>::Write(*this, vs), 0)...};
            (void) n;
        }

        // 一次完整的写入。会备份长度（方便计算相对偏移量）和初始化指针字典
        template<typename T>
        void WriteOnce(T const &v) {
            oh.bak = data.len;
            oh.objOffsets.clear();
            Write(v);
        }

        // 写入 typeId + offset + data
        template<typename T>
        void WritePtr(std::shared_ptr<T> const &v) {
            // 写入 typeId. 空值 typeId 为 0
            auto &&typeId = v ? v->GetTypeId() : (uint16_t) 0;
            Write(typeId);
            if (!typeId) return;

            // 计算 相对偏移量. 第一次出现则 计算相对偏移量. 否则用 从字典找到的
            auto &&iter = oh.objOffsets.find((void *) &*v);
            size_t offs;
            if (iter == oh.objOffsets.end()) {
                offs = data.len - oh.bak;
                oh.objOffsets[(void *) &*v] = offs;
            } else {
                offs = iter->second;
            }

            // 写入 相对偏移量
            Write(offs);

            // 第一次出现则 继续写入 内容序列化数据
            if (iter == oh.objOffsets.end()) {
                v->Serialize(*this);
            }
        }
    };

    /************************************************************************************/
    // 反序列化器
    struct DataReaderEx : DataReader {
        // 引用到 Object 功能辅助器
        ObjectHelper &oh;

        DataReaderEx(char const *const &buf, size_t const &len, ObjectHelper &oh)
                : DataReader(buf, len), oh(oh) {
        }

        DataReaderEx(Data const &d, ObjectHelper &oh)
                : DataReader(d.buf, d.len), oh(oh) {
        }

        // 读出并填充到变量. 可同时填充多个. 返回非 0 则读取失败
        template<typename ...TS>
        int Read(TS &...vs) {
            return ReadCore(vs...);
        }

    protected:
        template<typename T, typename ...TS>
        int ReadCore(T &v, TS &...vs) {
            if (auto r = DataFuncsEx<T>::Read(*this, v)) return r;
            return ReadCore(vs...);
        }

        template<typename T>
        int ReadCore(T &v) {
            return DataFuncsEx<T>::Read(*this, v);
        }

    public:

        template<typename T>
        int ReadOnce(T &v) {
            oh.bak = offset;
            oh.offsetObjs.clear();
            return Read(v);
        }

        template<typename T>
        int ReadPtr(std::shared_ptr<T> &v) {
            // 防御性清理数据内容
            v.reset();

            // 先读 typeId. 如果为 0 则 清空 入参 并退出
            uint16_t typeId;
            if (auto &&r = Read(typeId)) return r;
            if (!typeId) {
                return 0;
            }

            // 计算 当前相对偏移量
            auto currOffs = offset - oh.bak;

            // 读出 相对偏移量
            size_t offs;
            if (auto r = Read(offs)) return r;

            // 如果等于 当前相对偏移量，则表明是第一次写入，后面会跟随 内容序列化数据
            if (offs == currOffs) {

                // 用创建函数创建出目标实例. 如果创建失败则退出( 未注册? 创建函数bug? 创建时内存不足? 构造失败? )
                auto &&o = oh.CreateByTypeId(typeId);
                if (!o) return __LINE__;

                // 将 o 写入 v. 利用动态转换检查数据类型兼容性。如果转换失败则退出( 防止数据与类型对应不上 )
                v = std::dynamic_pointer_cast<T>(o);
                if (!v) return __LINE__;

                // 建立 相对偏移量 到 智能指针 的映射, 以便下次遇到这个 相对偏移量 从而直接读出
                oh.offsetObjs[offs] = o;

                // 继续读出类实例内容。如果失败则退出
                if (auto &&r = v->Deserialize(*this)) return r;
            } else {

                // 从字典查找 读出的 相对偏移量 对应的 智能指针。找不到 或 类型对应不上则退出
                auto iter = oh.offsetObjs.find(offs);
                if (iter == oh.offsetObjs.end()) return __LINE__;
                if (iter->second->GetTypeId() != typeId) return __LINE__;

                // 填充 入参。如果类型转换失败则退出( 防止数据与类型对应不上 )
                v = std::dynamic_pointer_cast<T>(iter->second);
                if (!v) return __LINE__;
            }
            // 返回成功
            return 0;
        }
    };

    /************************************************************************************/
    // 各种适配

    // 适配 std::shared_ptr<T>
    template<typename T>
    struct DataFuncsEx<std::shared_ptr<T>, std::enable_if_t<std::is_base_of_v<Object, T>>> {
        static inline void Write(DataWriterEx &dw, std::shared_ptr<T> const &in) {
            dw.WritePtr(in);
        }

        static inline int Read(DataReaderEx &dr, std::shared_ptr<T> &out) {
            return dr.ReadPtr(out);
        }
    };

    template<typename T>
    struct StringFuncsEx<std::shared_ptr<T>, std::enable_if_t<std::is_base_of_v<Object, T>>> {
        static inline void Append(ObjectHelper &oh, std::shared_ptr<T> const &in) {
            if (in) {
                in->ToString(oh);
            } else {
                oh.s.append("null");
            }
        }
    };

    template<typename T>
    struct CloneFuncs<std::shared_ptr<T>, std::enable_if_t<std::is_base_of_v<Object, T>>> {
        static inline void Clone1(ObjectHelper &oh, std::shared_ptr<T> const &in, std::shared_ptr<T> &out) {
            if (!in) {
                out.reset();
            } else {
                auto inTypeId = in->GetTypeId();
                if (!out || out->GetTypeId() != inTypeId) {
                    out = xx::As<T>(oh.CreateByTypeId(inTypeId));
                }
                oh.oldNewObjs[&*in] = out;
                in->Clone1(oh, out);
            }
        }

        static inline void Clone2(ObjectHelper &oh, std::shared_ptr<T> const &in, std::shared_ptr<T> &out) {
            if (in) {
                in->Clone2(oh, out);
            }
        }
    };

    // 适配 std::weak_ptr<T>
    template<typename T>
    struct DataFuncsEx<std::weak_ptr<T>, std::enable_if_t<std::is_base_of_v<Object, T>>> {
        static inline void Write(DataWriterEx &dw, std::weak_ptr<T> const &in) {
            dw.WritePtr(in.lock());
        }

        static inline int Read(DataReaderEx &dr, std::weak_ptr<T> &out) {
            std::shared_ptr<T> ptr;
            if (int r = dr.ReadPtr(ptr)) return r;
            out = ptr;
            return 0;
        }
    };

    template<typename T>
    struct StringFuncsEx<std::weak_ptr<T>, std::enable_if_t<std::is_base_of_v<Object, T>>> {
        static inline void Append(ObjectHelper &oh, std::weak_ptr<T> const &in) {
            if (auto &&p = in.lock()) {
                p->ToString(oh);
            } else {
                oh.s.append("null");
            }
        }
    };

    template<typename T>
    struct CloneFuncs<std::weak_ptr<T>, std::enable_if_t<std::is_base_of_v<Object, T>>> {
        static inline void Clone1(ObjectHelper &oh, std::weak_ptr<T> const &in, std::weak_ptr<T> &out) {
            // do nothing
        }

        static inline void Clone2(ObjectHelper &oh, std::weak_ptr<T> const &in, std::weak_ptr<T> &out) {
            assert(!out.lock());
            auto &&sp = in.lock();
            if (!sp) return;
            auto &&iter = oh.oldNewObjs.find(&*sp);
            if (iter == oh.oldNewObjs.end()) {
                out = sp;
            } else {
                out = xx::As<T>(iter->second);
            }
        }
    };

    // 适配 std::vector
    template<typename T>
    struct DataFuncsEx<std::vector<T>, void> {
        static inline void Write(DataWriterEx &dw, std::vector<T> const &in) {
            auto buf = in.data();
            auto len = in.size();
            auto &&d = dw.data;
            d.Reserve(d.len + 5 + len * sizeof(T));
            d.WriteVarIntger(len);
            if (!len) return;
            if constexpr (sizeof(T) == 1 || std::is_same_v<float, T>) {
                ::memcpy(d.buf + d.len, buf, len * sizeof(T));
                d.len += len * sizeof(T);
            } else {
                for (size_t i = 0; i < len; ++i) {
                    dw.Write(buf[i]);
                }
            }
        }

        static inline int Read(DataReaderEx &dr, std::vector<T> &out) {
            size_t siz = 0;
            if (auto rtv = dr.Read(siz)) return rtv;
            if (dr.offset + siz > dr.len) return __LINE__;
            out.resize(siz);
            if (siz == 0) return 0;
            auto buf = out.data();
            if constexpr (sizeof(T) == 1 || std::is_same_v<float, T>) {
                ::memcpy(buf, dr.buf + dr.offset, siz * sizeof(T));
                dr.offset += siz * sizeof(T);
            } else {
                for (size_t i = 0; i < siz; ++i) {
                    if (int r = dr.Read(buf[i])) return r;
                }
            }
            return 0;
        }
    };

    template<typename T>
    struct StringFuncsEx<std::vector<T>, void> {
        static inline void Append(ObjectHelper &oh, std::vector<T> const &in) {
            oh.s.push_back('[');
            if (auto inLen = in.size()) {
                for (size_t i = 0; i < inLen; ++i) {
                    ::xx::AppendEx(oh, in[i]);
                    oh.s.push_back(',');
                }
                oh.s[oh.s.size() - 1] = ']';
            } else {
                oh.s.push_back(']');
            }
        }
    };

    template<typename T>
    struct CloneFuncs<std::vector<T>, void> {
        static inline void Clone1(ObjectHelper &oh, std::vector<T> const &in, std::vector<T> &out) {
            auto siz = in.size();
            out.resize(siz);
            for (size_t i = 0; i < siz; ++i) {
                CloneFuncs<T>::Clone1(oh, in[i], out[i]);
            }
        }

        static inline void Clone2(ObjectHelper &oh, std::vector<T> const &in, std::vector<T> &out) {
            assert(in.size() == out.size());
            auto siz = in.size();
            for (size_t i = 0; i < siz; ++i) {
                CloneFuncs<T>::Clone2(oh, in[i], out[i]);
            }
        }
    };


    // 适配 std::map
    template<typename K, typename V>
    struct DataFuncsEx<std::map<K, V>, void> {
        static inline void Write(DataWriterEx &dw, std::map<K, V> const &in) {
            auto len = in.size();
            auto &&d = dw.data;
            d.Reserve(d.len + 5 + len * (sizeof(K) + sizeof(V)));
            d.WriteVarIntger(len);
            if (!len) return;
            for (auto &&kv : in) {
                dw.Write(kv.first, kv.second);
            }
        }

        static inline int Read(DataReaderEx &dr, std::map<K, V> &out) {
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

    template<typename K, typename V>
    struct StringFuncsEx<std::map<K, V>, void> {
        static inline void Append(ObjectHelper &oh, std::map<K, V> const &in) {
            auto &&s = oh.s;
            s.push_back('[');
            if (!in.empty()) {
                for (auto &kv : in) {
                    ::xx::AppendEx(oh, kv.first);
                    s.push_back(',');
                    ::xx::AppendEx(oh, kv.second);
                    s.push_back(',');
                }
                s[s.size() - 1] = ']';
            } else {
                s.push_back(']');
            }
        }
    };

    template<typename K, typename V>
    struct CloneFuncs<std::map<K, V>, void> {
        static inline void Clone1(ObjectHelper &oh, std::map<K, V> const &in, std::map<K, V> &out) {
            out.clear();
            for (auto &&kv : in) {
                std::pair<K, V> tar;
                CloneFuncs<K>::Clone1(oh, kv.first, tar.first);
                CloneFuncs<V>::Clone1(oh, kv.second, tar.second);
                out.insert(std::move(tar));
            }
        }

        static inline void Clone2(ObjectHelper &oh, std::map<K, V> const &in, std::map<K, V> &out) {
            assert(in.size() == out.size());
            for (auto &&kv : in) {
                auto &&iter = out.find(kv.first);
                CloneFuncs<K>::Clone2(oh, kv.first, *(K *) &iter->first);
                CloneFuncs<V>::Clone2(oh, kv.second, iter->second);
            }
        }
    };

    // 适配 std::unordered_map
    template<typename K, typename V>
    struct DataFuncsEx<std::unordered_map<K, V>, void> {
        static inline void Write(DataWriterEx &dw, std::unordered_map<K, V> const &in) {
            auto len = in.size();
            auto &&d = dw.data;
            d.Reserve(d.len + 5 + len * (sizeof(K) + sizeof(V)));
            d.WriteVarIntger(len);
            if (!len) return;
            for (auto &&kv : in) {
                dw.Write(kv.first, kv.second);
            }
        }

        static inline int Read(DataReaderEx &dr, std::unordered_map<K, V> &out) {
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

    template<typename K, typename V>
    struct StringFuncsEx<std::unordered_map<K, V>, void> {
        static inline void Append(ObjectHelper &oh, std::unordered_map<K, V> const &in) {
            auto &&s = oh.s;
            s.push_back('[');
            if (!in.empty()) {
                for (auto &kv : in) {
                    ::xx::AppendEx(oh, kv.first);
                    s.push_back(',');
                    ::xx::AppendEx(oh, kv.second);
                    s.push_back(',');
                }
                s[s.size() - 1] = ']';
            } else {
                s.push_back(']');
            }
        }
    };

    template<typename K, typename V>
    struct CloneFuncs<std::unordered_map<K, V>, void> {
        static inline void Clone1(ObjectHelper &oh, std::unordered_map<K, V> const &in, std::unordered_map<K, V> &out) {
            auto siz = in.size();
            out.clear();
            for (auto &&kv : in) {
                std::pair<K, V> tar;
                CloneFuncs<K>::Clone1(oh, kv.first, tar.first);
                CloneFuncs<V>::Clone1(oh, kv.second, tar.second);
                out.insert(std::move(tar));
            }
        }

        static inline void Clone2(ObjectHelper &oh, std::unordered_map<K, V> const &in, std::unordered_map<K, V> &out) {
            assert(in.size() == out.size());
            for (auto &&kv : in) {
                auto &&iter = out.find(kv.first);
                CloneFuncs<K>::Clone2(oh, kv.first, *(K *) &iter->first);
                CloneFuncs<V>::Clone2(oh, kv.second, iter->second);
            }
        }
    };


    // 适配 std::pair
    template<typename K, typename V>
    struct DataFuncsEx<std::pair<K, V>, void> {
        static inline void Write(DataWriterEx &dw, std::pair<K, V> const &in) {
            dw.Write(in.first, in.second);
        }

        static inline int Read(DataReaderEx &dr, std::pair<K, V> &out) {
            return dr.Read(out.first, out.second);
        }
    };

    template<typename K, typename V>
    struct StringFuncsEx<std::pair<K, V>, void> {
        static inline void Append(ObjectHelper &oh, std::pair<K, V> const &in) {
            auto &&s = oh.s;
            s.push_back('[');
            ::xx::AppendEx(oh, in.first);
            s.push_back(',');
            ::xx::AppendEx(oh, in.second);
            s.push_back(']');
        }
    };

    template<typename K, typename V>
    struct CloneFuncs<std::pair<K, V>, void> {
        static inline void Clone1(ObjectHelper &oh, std::pair<K, V> const &in, std::pair<K, V> &out) {
            CloneFuncs<K>::Clone1(oh, in.first, out.first);
            CloneFuncs<V>::Clone1(oh, in.second, out.second);
        }

        static inline void Clone2(ObjectHelper &oh, std::pair<K, V> const &in, std::pair<K, V> &out) {
            CloneFuncs<K>::Clone2(oh, in.first, out.first);
            CloneFuncs<V>::Clone2(oh, in.second, out.second);
        }
    };

    // 适配 std::tuple<T...>
    template<typename...T>
    struct DataFuncsEx<std::tuple<T...>, void> {
        static inline void Write(DataWriterEx &dw, std::tuple<T...> const &in) {
            std::apply([&](auto const &... args) {
                dw.Write(args...);
            }, in);
        }

        static inline int Read(DataReaderEx &dr, std::tuple<T...> &out) {
            int rtv = 0;
            std::apply([&](auto &... args) {
                rtv = dr.Read(args...);
            }, out);
            return rtv;
        }
    };

    template<typename...T>
    struct StringFuncsEx<std::tuple<T...>, void> {
        static inline void Append(ObjectHelper &oh, std::tuple<T...> const &in) {
            auto &&s = oh.s;
            s.push_back('[');
            std::apply([&](auto const &... args) {
                std::initializer_list<int> n{((::xx::AppendEx(oh, args, ',')), 0)...};
                if constexpr(sizeof...(args)) {
                    s.resize(s.size() - 1);
                }
            }, in);
            s.push_back(']');
        }
    };

    namespace Detail {
        template<class Tuple, std::size_t N>
        struct TupleForeachClone {
            static void Clone1(Tuple const &in, Tuple &out) {
                xx::CloneFuncs<std::tuple_element_t<N - 1, Tuple>, void>::Clone1(std::get<N - 1>(in), std::get<N - 1>(out));
                TupleForeachClone<Tuple, N - 1>::Clone1(in, out);
            }

            static void Clone2(Tuple const &in, Tuple &out) {
                xx::CloneFuncs<std::tuple_element_t<N - 1, Tuple>, void>::Clone2(std::get<N - 1>(in), std::get<N - 1>(out));
                TupleForeachClone<Tuple, N - 1>::Clone2(in, out);
            }
        };

        template<class Tuple>
        struct TupleForeachClone<Tuple, 1> {
            static void Clone1(Tuple const &in, Tuple &out) {}

            static void Clone2(Tuple const &in, Tuple &out) {}
        };
    }

    template<typename...T>
    struct CloneFuncs<std::tuple<T...>, void> {
        static inline void Clone1(ObjectHelper &oh, std::tuple<T...> const &in, std::tuple<T...> &out) {
            Detail::TupleForeachClone<std::tuple<T...>, std::tuple_size_v<std::tuple<T...>>>::Clone1(in, out);
        }

        static inline void Clone2(ObjectHelper &oh, std::tuple<T...> const &in, std::tuple<T...> &out) {
            Detail::TupleForeachClone<std::tuple<T...>, std::tuple_size_v<std::tuple<T...>>>::Clone2(in, out);
        }
    };


    /************************************************************************************/
    // ObjectHelper 的各种实现

    template<typename T>
    void ObjectHelper::WriteTo(Data &d, T const &v) {
        DataWriterEx dw(d, *this);
        dw.WriteOnce(v);
    }

    template<typename T>
    int ObjectHelper::ReadFrom(Data &d, T &v) {
        DataReaderEx dr(d, *this);
        return dr.ReadOnce(v);
    }

    inline std::shared_ptr<Object> ObjectHelper::ReadFrom(Data &d) {
        DataReaderEx dr(d, *this);
        std::shared_ptr<Object> o;
        lastRtv = dr.ReadOnce(o);
        return o;
    }

    template<typename T>
    int ObjectHelper::Compare(T const &a, T const &b) {
        s1 = ToString(a);
        s2 = ToString(b);
        auto &&s1Siz = s1.size();
        auto &&s2Siz = s1.size();
        if (s1Siz != s2Siz) return -1;
        return memcmp(s1.data(), s2.data(), s1Siz);
    }

    inline void ObjectHelper::CoutCompareResult() {
        size_t len = s1.size();
        if (len > s2.size()) {
            len = s2.size();
        }
        size_t i = 0;
        for (; i < len; ++i) {
            if (s1[i] != s2[i]) break;
        }
        std::cout << s1 << std::endl;
        std::cout << std::string(i, ' ') << '|' << std::endl;
        std::cout << s2 << std::endl;
    }

    template<typename T>
    void ObjectHelper::Clone(T const &in, T &out) {
        oldNewObjs.clear();
        CloneFuncs<T>::Clone1(*this, in, out);
        CloneFuncs<T>::Clone2(*this, in, out);
    }

    template<typename T>
    T ObjectHelper::Clone(T const &in) {
        T out;
        Clone<T>(in, out);
        return out;
    }
}

/************************************************************************************/
// 各种宏

#define XX_GENCODE_OBJECT_H(T, BT) \
using BaseType = BT; \
T() = default; \
T(T const&) = default; \
T& operator=(T const&) = default; \
T(T&& o) noexcept; \
T& operator=(T&& o) noexcept; \
uint16_t GetTypeId() const override; \
void Serialize(xx::DataWriterEx &dw) const override; \
int Deserialize(xx::DataReaderEx &dr) override; \
void ToString(xx::ObjectHelper &oh) const override; \
void ToStringCore(xx::ObjectHelper &oh) const override; \
void Clone1(xx::ObjectHelper &oh, std::shared_ptr<Object> const& tar) const override; \
void Clone2(xx::ObjectHelper &oh, std::shared_ptr<Object> const& tar) const override;


#define XX_GENCODE_STRUCT_H(T) \
T() = default; \
T(T const&) = default; \
T& operator=(T const&) = default; \
T(T&& o) noexcept; \
T& operator=(T&& o) noexcept;
