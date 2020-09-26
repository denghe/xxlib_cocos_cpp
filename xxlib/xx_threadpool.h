#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace xx {

    /*
    T sample:
    struct Foo {
        Foo() = default;
        void operator()(std::function<void(Foo&)>& job)
        {
            try {
                job(*this);
            }
            catch (int const& e) {
                xx::CoutN("catch throw int: ", e);
            }
            catch (std::exception const& e) {
                xx::CoutN("catch throw std::exception: ", e);
            }
            catch (...) {
                xx::CoutN("catch ...");
            }
        }
    };
    */

    template<typename T>
    class ThreadPool {
        using JT = std::function<void(T &)>;

        std::vector<std::thread> threads;
        std::queue<JT> jobs;
        std::mutex mtx;
        std::condition_variable cond;
        bool stop = false;

    public:
        explicit ThreadPool(int const &numThreads = 4) {
            for (int i = 0; i < numThreads; ++i) {
                threads.emplace_back([this] {
                    // require default constructor
                    T t;
                    while (true) {
                        JT job;
                        {
                            std::unique_lock<std::mutex> lock(this->mtx);
                            this->cond.wait(lock, [this] {
                                return this->stop || !this->jobs.empty();
                            });
                            if (this->stop && this->jobs.empty()) return;
                            job = std::move(this->jobs.front());
                            this->jobs.pop();
                        }
                        // require void operator()(std::function<void(T&)>& job) { job(*this); }
                        t(job);
                    }
                });
            }
        }

        int Add(JT &&job) {
            {
                std::unique_lock<std::mutex> lock(mtx);
                if (stop) return -1;
                jobs.emplace(std::move(job));
            }
            cond.notify_one();
            return 0;
        }

        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> lock(mtx);
                stop = true;
            }
            cond.notify_all();
            for (std::thread &worker : threads) {
                worker.join();
            }
        }
    };

    // 从上面复制小改的精简版
    template<>
    class ThreadPool<void> {
        using JT = std::function<void()>;

        std::vector<std::thread> threads;
        std::queue<JT> jobs;
        std::mutex mtx;
        std::condition_variable cond;
        bool stop = false;

    public:
        explicit ThreadPool(int const &numThreads = 4) {
            for (int i = 0; i < numThreads; ++i) {
                threads.emplace_back([this] {
                    while (true) {
                        JT job;
                        {
                            std::unique_lock<std::mutex> lock(this->mtx);
                            this->cond.wait(lock, [this] {
                                return this->stop || !this->jobs.empty();
                            });
                            if (this->stop && this->jobs.empty()) return;
                            job = std::move(this->jobs.front());
                            this->jobs.pop();
                        }
                        job();
                    }
                });
            }
        }

        int Add(JT &&job) {
            {
                std::unique_lock<std::mutex> lock(mtx);
                if (stop) return -1;
                jobs.emplace(std::move(job));
            }
            cond.notify_one();
            return 0;
        }

        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> lock(mtx);
                stop = true;
            }
            cond.notify_all();
            for (std::thread &worker : threads) {
                worker.join();
            }
        }
    };
}
