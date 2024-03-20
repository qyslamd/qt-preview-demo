/**
 *
 *
 * */

#pragma once

#include <mutex>
#include <queue>
namespace demo_thread_pool {
template <typename T> class TaskQueue {
public:
  TaskQueue() = default;
  TaskQueue(TaskQueue &&other) {}
  ~TaskQueue() {}

  ///
  /// \brief empty
  /// \return
  ///
  bool empty() {
    std::unique_lock<std::mutex> lock(mutex); // 加锁，防止被修改
    return queue.empty();
  }

  ///
  /// \brief size
  /// \return
  ///
  size_t size() {
    std::unique_lock<std::mutex> lock(mutex); // 加锁，防止被修改
    return queue.size();
  }

  ///
  /// \brief enqueue
  /// \param t
  ///
  void enqueue(T &t) {
    std::unique_lock<std::mutex> lock(mutex); // 加锁，防止被修改
    queue.emplace(t);
  }

  bool dequeue(T &t) {
    std::unique_lock<std::mutex> lock(mutex); // 加锁，防止被修改
    if (queue.empty()) {
      return false;
    }
    t = std::move(queue.front()); // 取出队首元素，并进行右值引用

    queue.pop(); // 弹出队中第一个元素
    return true;
  }

private:
  std::queue<T> queue;
  std::mutex mutex;
};
} // namespace demo_thread_pool
