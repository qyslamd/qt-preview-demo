/**
 *
 *
 * */

#pragma once

#include <mutex>
#include <queue>
namespace demo_thread_pool {
template <typename T>
class TaskQueue {
 public:
  TaskQueue() = default;
  TaskQueue(TaskQueue&&) {}
  ~TaskQueue() {}

  ///
  /// \brief empty
  /// \return
  ///
  bool empty() {
    std::unique_lock<std::mutex> lock(mutex);
    return queue.empty();
  }

  ///
  /// \brief size
  /// \return
  ///
  size_t size() {
    std::unique_lock<std::mutex> lock(mutex);
    return queue.size();
  }

  ///
  /// \brief enqueue
  /// \param t
  ///
  void enqueue(T& t) {
    std::unique_lock<std::mutex> lock(mutex);
    queue.emplace(t);
  }

  bool dequeue(T& t) {
    std::unique_lock<std::mutex> lock(mutex);
    if (queue.empty()) {
      return false;
    }
    t = std::move(queue.front());

    queue.pop();
    return true;
  }

 private:
  std::queue<T> queue;
  std::mutex mutex;
};
}  // namespace demo_thread_pool
