#pragma once

#include <utility>
#include <functional>

namespace xx {
    struct ScopeGuard {
    protected:
        using Func = std::function<void()>;
        Func func;
    public:
        template<typename T, class = std::enable_if_t<!std::is_same_v<T, ScopeGuard>>>
        explicit ScopeGuard(T &&v)
                : func(std::forward<T>(v)) {
        }

        ScopeGuard(ScopeGuard const &) = delete;

        ScopeGuard &operator=(ScopeGuard const &) = delete;

        ScopeGuard(ScopeGuard &&o) noexcept
                : func(std::move(o.func)) {
        }

        ScopeGuard &operator=(ScopeGuard &&o) noexcept {
            std::swap(func, o.func);
            return *this;
        }

        ~ScopeGuard() {
            Run();
        }

        inline void RunAndCancel() noexcept {
            Run();
            Cancel();
        }

        inline void Run() noexcept {
            if (func) {
                func();
            }
        }

        inline void Cancel() noexcept {
            func = nullptr;
        }

        template<typename T>
        inline void Set(T &&f) noexcept {
            func = std::forward<T>(f);
        }
    };
}
