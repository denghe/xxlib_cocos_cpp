#pragma once

#include "xx_typehelpers.h"
#include <cassert>
#include <cstring>

namespace xx {

    // 可视作成员内存平铺版 std::shared_ptr<std::vector>. 复制规则变为复制指针增加引用计数
    // 注意：任何 添加 操作都可能导致 buf 新建，从而和之前引用到的 buf 脱钩( 如果有的话 )
    template<typename T>
    struct SharedList {
        struct Header {
            int len;
            int cap;
            int useCount;
            int unused;
            T buf[0];
        };
        // == Header.buf
        T *buf = nullptr;

        inline Header& header() const {
            return *((Header*)buf - 1);
        }

        inline int len() const {
            if (!buf) return 0;
            return header().len;
        }

        inline int cap() const {
            if (!buf) return 0;
            return header().cap;
        }

        inline int useCount() const {
            if (!buf) return 0;
            return header().useCount;
        }

        inline int unused() const {
            if (!buf) return 0;
            return header().unused;
        }

        explicit SharedList(int const &cap_ = 0) {
            if (cap_) {
                Reserve(cap_);
            }
        }

        SharedList(SharedList const &o) {
            buf = o.buf;
            ++header().useCount;
        }

        SharedList(SharedList &&o) noexcept
            : buf(o.buf) {
            o.buf = nullptr;
        }

        SharedList &operator=(SharedList const &o) {
            if (this != &o && buf != o.buf) {
                Free();
                buf = o.buf;
            }
            return *this;
        }

        SharedList &operator=(SharedList &&o) noexcept {
            std::swap(buf, o.buf);
            return *this;
        }

        ~SharedList() {
            Free();
        }


        // 申请空间. 原buf 的内容会移动到新 buf. 原buf 长度清0, 减持 甚至 释放
        inline void Reserve(int cap_) {
            if (!cap_) {
                Clear();
                return;
            }
            if (!buf) {
                buf = NewBuf(cap_);
                return;
            }
            if (cap_ <= cap()) return;

            if (cap_ < header().cap * 2) {
                cap_ = header().cap * 2;
            }
            auto newBuf = NewBuf(cap_, header().len);
            if constexpr (IsPod_v<T>) {
                memcpy((void *) newBuf, (void *) buf, header().len * sizeof(T));
            } else {
                for (int i = 0; i < header().len; ++i) {
                    new(&newBuf[i]) T((T &&) buf[i]);
                }
            }
            header().len = 0;
            Free();
            buf = newBuf;
        }

        // 长度清 0
        inline void Clear() {
            if (!buf) return;
            if constexpr (!std::is_pod_v<T>) {
                auto siz = header().len;
                for (int i = 0; i < siz; ++i) {
                    buf[i].~T();
                }
            }
            header().len = 0;
        }

        // 减持 甚至 释放 buf
        inline void Free() {
            if (!buf) return;
            if (--header().useCount == 0) {
                Clear();
                free((void *) ((char *) buf - sizeof(Header)));
            }
            buf = nullptr;
        }

        // 修改长度. 可能触发 申请空间及其副作用
        int Resize(int const& siz)  {
            auto rtv = len();
            if (siz && !buf) {
                buf = NewBuf(siz);
            }
            if (siz == rtv) return rtv;
            else if (siz < rtv) {
                if constexpr (!std::is_pod_v<T>) {
                    for (int i = siz; i < rtv; ++i) {
                        buf[i].~T();
                    }
                }
            }
            else {
                Reserve(siz);
                if constexpr (!std::is_pod_v<T>) {
                    for (int i = rtv; i < siz; ++i) {
                        new (buf + i) T();
                    }
                }
            }
            header().len = siz;
            return rtv;
        }

        // 查找并移除一个元素
        void Remove(T const& v)  {
            if (!buf) return;
            auto siz = len();
            for (int i = 0; i < siz; ++i) {
                if (v == buf[i]) {
                    RemoveAt(i);
                    return;
                }
            }
        }

        // 移除指定下标的元素. 后面的元素依次移动
        void RemoveAt(int const& idx)  {
            assert(idx < len());
            --header().len;
            if constexpr (IsPod_v<T>) {
                buf[idx].~T();
                memmove(buf + idx, buf + idx + 1, (header().len - idx) * sizeof(T));
            }
            else {
                for (int i = idx; i < header().len; ++i) {
                    buf[i] = (T&&)buf[i + 1];
                }
                if constexpr (!std::is_pod_v<T>) {
                    buf[header().len].~T();
                }
            }
        }

        // 和最后一个元素做交换删除.
        // 通常环境为随机访问或 倒循环扫描 if (list.len) { for (auto i = list.len - 1; i >= 0; --i) { ...
        // 通常上一句为 list[list.len - 1]->idx = o->idx;
        void SwapRemoveAt(int const& idx)  {
            assert(idx < len());
            if (idx + 1 < header().len) {
                std::swap(buf[idx], buf[header().len - 1]);
            }
            --header().len;
            if constexpr (!std::is_pod_v<T>) {
                buf[len()].~T();
            }
        }

        // 用 T 的一到多个构造函数的参数来追加构造一个 item
        template<typename...Args>
        T& Emplace(Args&&...args)  {
            Reserve(len() + 1);
            return *new (&buf[header().len++]) T(std::forward<Args>(args)...);
        }

        // 用 T 的一到多个构造函数的参数来构造一个 item 插入到指定下标. 下标越界等同于追加
        template<typename...Args>
        T& EmplaceAt(int idx, Args&&...args)  {
            Reserve(len() + 1);
            if (idx < header().len) {
                if constexpr (IsPod_v<T>) {
                    memmove(buf + idx + 1, buf + idx, (header().len - idx) * sizeof(T));
                }
                else {
                    new (buf + header().len) T((T&&)buf[header().len - 1]);
                    for (int i = header().len - 1; i > idx; --i) {
                        buf[i] = (T&&)buf[i - 1];
                    }
                    if constexpr (!std::is_pod_v<T>) {
                        buf[idx].~T();
                    }
                }
            }
            else idx = header().len;
            ++header().len;
            new (buf + idx) T(std::forward<Args>(args)...);
            return buf[idx];
        }

        // 与 Emplace 不同的是, 这个仅支持1个参数的构造函数, 可一次追加多个
        template<typename ...TS>
        void Add(TS&&...vs)  {
            std::initializer_list<int> n{ (Emplace(std::forward<TS>(vs)), 0)... };
            (void)(n);
        }

        // 批量添加
        void AddRange(T const* const& items, int const& count) {
            Reserve(len() + count);
            if constexpr (IsPod_v<T>) {
                memcpy(buf + header().len, items, count * sizeof(T));
            }
            else {
                for (int i = 0; i < count; ++i) {
                    new (&buf[header().len + i]) T((T&&)items[i]);
                }
            }
            header().len += count;
        }

        void AddRange(SharedList<T> const& list) {
            return AddRange(list.buf, list.len());
        }

        // 如果找到就返回索引. 找不到将返回 -1
        int Find(T const& v) const {
            if (!buf) return -1;
            auto siz = header().len;
            for (int i = 0; i < siz; ++i) {
                if (v == buf[i]) return i;
            }
            return -1;
        }


        T const &operator[](int const &index) const {
            assert(index < len());
            return buf[index];
        }

        T &operator[](int const &index) {
            assert(index < len());
            return buf[index];
        }

        T const& At(int const& index) const {
            assert(index < len());
            return buf[index];
        }
        T& At(int const& index) {
            assert(index < len());
            return buf[index];
        }

        T& Top() {
            assert(len());
            return buf[header().len - 1];
        }
        void Pop() {
            assert(len());
            --header().len;
            buf[header().len].~T();
        }
        T const& Top() const {
            assert(len());
            return buf[header().len - 1];
        }
        bool TryPop(T& output) {
            if (!buf || !len()) return false;
            output = (T&&)buf[--header().len];
            buf[header().len].~T();
            return true;
        }

        // support for( auto&& c : list )
        struct Iter {
            T *ptr;

            bool operator!=(Iter const &other) { return ptr != other.ptr; }

            Iter &operator++() {
                ++ptr;
                return *this;
            }

            T &operator*() { return *ptr; }
        };

        Iter begin() { return Iter{buf}; }

        Iter end() { return Iter{buf + len()}; }

        Iter begin() const { return Iter{buf}; }

        Iter end() const { return Iter{buf + len()}; }

    protected:
        inline T *NewBuf(int const &cap, int const& len = 0) {
            auto&& c = *(Header*) malloc(sizeof(Header) + sizeof(T) * cap);
            c.len = len;
            c.cap = cap;
            c.useCount = 1;
            c.unused = 0;
            return c.buf;
        }
    };
}
