#pragma once

#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <utility>

#include "task_queue.hpp"

namespace demo_thread_pool {
class ThreadPool {
private:
  class ThreadWorker {
  public:
    ThreadWorker(ThreadPool *pool, const int id) : pool(pool), id(id) {}
    void operator()() {
      while (!pool->shutdown_flag && pool->queue.empty()) {
        std::unique_lock<std::mutex> lock(pool->conditional_mutex);
        pool->conditional_lock.wait(lock);
        std::function<void()> func;
        if (auto dequeued = pool->queue.dequeue(func)) {
          func();
        }
      }
    }

  private:
    ThreadPool *pool;
    int id;
  };

public:
  ThreadPool(const int threadCount = 4)
      : shutdown_flag(false), threads(std::vector<std::thread>(threadCount)) {}
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &&) = delete;

  ///
  /// \brief init
  ///
  void init() {
    for (int i = 0; i < threads.size(); i++) {
      threads.at(i) = std::thread(ThreadWorker(this, i));
    }
  }

  ///
  /// \brief shutdown
  ///
  void shutdown() {
    shutdown_flag = true;
    conditional_lock.notify_all(); // 通知，唤醒所有线程
    for (decltype(threads.size()) i = 0; i < threads.size(); i++) {
      if (threads.at(i).joinable()) {
        threads.at(i).join();
      }
    }
  }

  // Submit a function to be executed asynchronously by the pool
  template <typename F, typename... Args>
  auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))> {
    // Create a function with bounded parameter ready to execute
    std::function<decltype(f(args...))()> func =
        std::bind(std::forward<F>(f), std::forward<Args>(args)...);

    // Encapsulate it into a shared pointer in order to be able to copy
    // construct
    auto task_ptr =
        std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

    // Warp packaged task into void function
    std::function<void()> warpper_func = [task_ptr]() { (*task_ptr)(); };
    queue.enqueue(warpper_func);

    //
    conditional_lock.notify_one();

    //
    return task_ptr->get_future();
  }

private:
  bool shutdown_flag; // 线程池是否关闭
  TaskQueue<std::function<void()>> queue;
  std::vector<std::thread> threads; // 工作线程队列
  std::mutex conditional_mutex;     // 线程休眠互斥锁
  std::condition_variable
      conditional_lock; // 线程环境锁，可以让线程处于休眠或者唤醒状态
};
} // namespace demo_thread_pool
