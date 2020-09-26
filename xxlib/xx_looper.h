#pragma once
#include "xx_chrono.h"
#include <memory>
#include <exception>
#include <functional>

namespace xx::Looper {

    // 总上下文。需确保它活得最久。换句话说，当它析构时，确保所有成员全部清掉
    struct Context;

    /*************************************************************************/
    // Timer
    /*************************************************************************/

    // Looper Context 成员基类
    struct Timer {
    protected:
        friend Context;

        // 位于时间轮的下标
        int timeoutIndex = -1;
        // 指向上一个对象（在时间轮每一格形成一个双链表）
        Timer *timeoutPrev = nullptr;
        // 指向下一个对象（在时间轮每一格形成一个双链表）
        Timer *timeoutNext = nullptr;

        // 指向循环器
        Context* ctx;

    public:
        explicit Timer(Context* const &ctx);

        // SetTimeout(0)
        virtual ~Timer();

        // 设置超时( 单位：帧 )
        void SetTimeout(int const &interval);

        // 设置超时( 单位：秒 )
        void SetTimeoutSeconds(double const &seconds);

        // Close
        virtual void Timeout() = 0;
    };

    /*************************************************************************/
    // TimerEx
    /*************************************************************************/

    // 可作为值类型成员使用
    // 小心：lambda 可能导致某些 shared_ptr 生命周期延长 或者令某些对象中途析构
    struct TimerEx : Timer {
        using Timer::Timer;
        std::function<void(TimerEx* const& self)> onTimeout;
        inline virtual void Timeout() { onTimeout(this); }
    };

    /*************************************************************************/
    // Item
    /*************************************************************************/

    // 业务逻辑常见基类
    // 注意：只能用 shared_ptr 包裹使用
    struct Item : Timer, std::enable_shared_from_this<Item> {
        // Timer(ctx)
        explicit Item(Context* const &ctx);

        // Close
        void Timeout() override;

        // 将当前实例的智能指针放入 ctx->holdItems( 不能在构造函数或析构中执行 )
        void Hold();

        // 将当前实例的指针放入 ctx->deadItems( 不能在析构中执行 ) 稍后会从 ctx->holdItems 移除以触发析构
        void DelayUnhold();

        // 需要派生类覆盖的函数
        virtual bool Close(int const &reason, char const *const &desc) = 0;

        // 需要派生类覆盖的函数( Close(0, ... ) )
        //~Item() override;
    };


    /*************************************************************************/
    // Context
    /*************************************************************************/

    struct Context {
        /************************************************/
        // public

        // 执行标志位。如果要退出，修改它
        bool running = true;

        // 公共只读: 每帧开始时更新一下
        int64_t nowMS = 0;

        // Run 时填充, 以便于局部获取并转换时间单位
        double frameRate = 10;

        // 帧时间间隔
        double ticksPerFrame = 10000000.0 / frameRate;

        /************************************************/
        // protected

        // item 的智能指针的保持容器
        std::unordered_map<Item *, std::shared_ptr<Item>> holdItems;

        // 要删除一个 peer 就把它的 指针 压到这个队列. 会在 稍后 从 items 删除
        std::vector<Item *> deadItems;

        // 时间轮. 只存指针引用, 不管理内存
        std::vector<Timer *> wheel;

        // 指向时间轮的游标
        int cursor = 0;

        // 参数：时间轮长度( 要求为 2^n. 如果帧率很高 300? 500+? 该值可能需要改大 )
        explicit Context(size_t const &wheelLen = (1u << 12u));

        Context(Context const &) = delete;

        Context &operator=(Context const &) = delete;

        Context(Context &&o) noexcept;

        Context &operator=(Context &&o) noexcept;

        virtual ~Context() = default;

        // 帧逻辑可以覆盖这个函数. 返回非 0 将令 Run 退出
        inline virtual int FrameUpdate() { return 0; }

        // 将秒转为帧数
        inline int SecondsToFrames(double const &sec) const { return (int) (frameRate * sec); }

        // 初始化 Run 帧率
        virtual int SetFrameRate(double const &frameRate);

        // 每帧调用一次 以驱动 timer
        void UpdateTimeoutWheel();

        // 开始稳帧循环
        int Run();

        // 提供帧间睡眠功能( 可覆盖放别的, 例如 epoll_wait ). 返回非 0 会导致 Run 退出
        virtual int Wait(int const& ms);
    };


    /*************************************************************************/
    // Timer impls
    /*************************************************************************/

    inline Timer::Timer(Context* const &ctx)
            : ctx(ctx) {
    }

    inline void Timer::SetTimeout(int const &interval) {
        // 试着从 wheel 链表中移除
        if (timeoutIndex != -1) {
            if (timeoutNext != nullptr) {
                timeoutNext->timeoutPrev = timeoutPrev;
            }
            if (timeoutPrev != nullptr) {
                timeoutPrev->timeoutNext = timeoutNext;
            } else {
                ctx->wheel[timeoutIndex] = timeoutNext;
            }
        }

        // 检查是否传入间隔时间
        if (interval) {
            // 如果设置了新的超时时间, 则放入相应的链表
            // 安全检查
            if (interval < 0 || interval > (int) ctx->wheel.size())
                throw std::logic_error(
                        __LINESTR__
                        " Timer SetTimeout if (interval < 0 || interval > (int) ctx->wheel.size())");

            // 环形定位到 wheel 元素目标链表下标
            timeoutIndex = (interval + ctx->cursor) & ((int) ctx->wheel.size() - 1);

            // 成为链表头
            timeoutPrev = nullptr;
            timeoutNext = ctx->wheel[timeoutIndex];
            ctx->wheel[timeoutIndex] = this;

            // 和之前的链表头连起来( 如果有的话 )
            if (timeoutNext) {
                timeoutNext->timeoutPrev = this;
            }
        } else {
            // 重置到初始状态
            timeoutIndex = -1;
            timeoutPrev = nullptr;
            timeoutNext = nullptr;
        }
    }

    inline void Timer::SetTimeoutSeconds(double const &seconds) {
        SetTimeout(ctx->SecondsToFrames(seconds));
    }

    inline Timer::~Timer() {
        SetTimeout(0);
    }

    /*************************************************************************/
    // Item impls
    /*************************************************************************/

    inline Item::Item(Context* const &ctx) : Timer(ctx) {}

    inline void Item::Timeout() {
        Close(-1, __LINESTR__ " Timer Timeout");
    }

    inline void Item::Hold() {
        ctx->holdItems[this] = shared_from_this();
    }

    inline void Item::DelayUnhold() {
        ctx->deadItems.emplace_back(this);
    }

    /*************************************************************************/
    // Context impls
    /*************************************************************************/

    inline Context::Context(size_t const &wheelLen) {
        // 初始化时间伦
        wheel.resize(wheelLen);
    }

    inline Context::Context(Context &&o) noexcept {
        this->operator=(std::move(o));
    }

    inline Context& Context::operator=(Context &&o) noexcept {
        std::swap(running, o.running);
        std::swap(nowMS, o.nowMS);
        std::swap(frameRate, o.frameRate);
        std::swap(ticksPerFrame, o.ticksPerFrame);
        std::swap(holdItems, o.holdItems);
        std::swap(deadItems, o.deadItems);
        std::swap(wheel, o.wheel);
        std::swap(cursor, o.cursor);
        return *this;
    }

    inline int Context::SetFrameRate(double const &frameRate_) {
        // 参数检查
        if (frameRate_ <= 0) return -1;
        // 保存帧率
        frameRate = frameRate_;
        // 计算帧时间间隔
        ticksPerFrame = 10000000.0 / frameRate_;
        return 0;
    }

    inline void Context::UpdateTimeoutWheel() {
        cursor = (cursor + 1) & ((int) wheel.size() - 1);
        auto p = wheel[cursor];
        wheel[cursor] = nullptr;
        while (p) {
            auto np = p->timeoutNext;

            p->timeoutIndex = -1;
            p->timeoutPrev = nullptr;
            p->timeoutNext = nullptr;

            p->Timeout();
            p = np;
        };
    }

    inline int Context::Run() {
        // 稳定帧回调用的时间池
        double ticksPool = 0;
        // 本次要 Wait 的超时时长
        int waitMS = 0;
        // 取当前时间
        auto lastTicks = xx::NowEpoch10m();
        // 更新一下逻辑可能用到的时间戳
        nowMS = xx::NowSteadyEpochMS();
        // 开始循环
        while (running) {
            // 计算上个循环到现在经历的时长, 并累加到 pool
            auto currTicks = xx::NowEpoch10m();
            auto elapsedTicks = (double) (currTicks - lastTicks);
            ticksPool += elapsedTicks;
            lastTicks = currTicks;

            // 如果累计时长跨度大于一帧的时长 则 Update
            if (ticksPool > ticksPerFrame) {
                // 消耗累计时长
                ticksPool -= ticksPerFrame;
                // 本次 Wait 不等待.
                waitMS = 0;
                // 更新一下逻辑可能用到的时间戳
                nowMS = xx::NowSteadyEpochMS();
                // 驱动 timer
                UpdateTimeoutWheel();
                // 帧逻辑调用一次
                if (int r = FrameUpdate()) return r;
            } else {
                // 计算等待时长
                waitMS = (int) ((ticksPerFrame - elapsedTicks) / 10000.0);
            }
            // 调用一次 wait.
            if (int r = Wait(waitMS)) return r;
            // 清除延迟杀死的 items
            if (!deadItems.empty()) {
                for (auto &&item : deadItems) {
                    holdItems.erase(item);
                }
                deadItems.clear();
            }
        }
        return 0;
    }

    inline int Context::Wait(int const& ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        return 0;
    }
}
