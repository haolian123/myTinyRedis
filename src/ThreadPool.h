#ifndef THREADPOOL
#define THREADPOOL

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <cassert>
#include <functional>

// 线程池结构体，用于管理任务队列和线程状态
struct Pool {
    std::mutex mutex;                    // 互斥锁，用于保护任务队列
    std::condition_variable condition;    // 条件变量，用于线程等待任务
    bool isClose;                        // 线程池是否关闭
    std::queue<std::function<void()>> tasks; // 任务队列，存放需要执行的任务
};

class ThreadPool {
private:
    std::shared_ptr<Pool> pool; // 线程池结构体的共享指针

public:
     // 构造函数，创建线程池并启动线程
    explicit ThreadPool(size_t threadNumber = 8)
        : pool(std::make_shared<Pool>()) {
        assert(threadNumber > 0);

        // 创建指定数量的线程，并将它们绑定到任务执行函数
        for (size_t i = 0; i < threadNumber; i++) {
            std::thread(
                [&]() { 
                    std::unique_lock<std::mutex> locker(pool->mutex);
                    while (true) {
                        if (!pool->tasks.empty()) {
                            auto task = std::move(pool->tasks.front());
                            pool->tasks.pop();
                            locker.unlock();
                            task();
                            locker.lock();
                        } else if (pool->isClose) {
                            break;
                        } else {
                            pool->condition.wait(locker);
                        }
                    }
                }
            ).detach();
        }
    }

    // 移动构造函数
    ThreadPool(ThreadPool&&) = default;

    // 析构函数，关闭线程池
    ~ThreadPool() {
        if (pool != nullptr) {
            std::lock_guard<std::mutex> locker(pool->mutex);
            pool->isClose = true;
        }
        pool->condition.notify_all();
    }

    // 添加任务到任务队列
    template <class F>
    void addTask(F&& task) {
        {
            std::lock_guard<std::mutex> locker(pool->mutex);
            pool->tasks.emplace(std::forward<F>(task));
        }
        pool->condition.notify_one();
    }
};

#endif
