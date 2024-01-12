#ifndef SIMPLESTOCKEXCHANGE_UTILITY_THREADSAFEQ_HPP
#define SIMPLESTOCKEXCHANGE_UTILITY_THREADSAFEQ_HPP

#include <boost/lockfree/queue.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <queue>

namespace s21 {
template <typename T>
class ThreadSafeQ : boost::noncopyable {
  using scoped_lock = boost::recursive_mutex::scoped_lock;

 public:
  ThreadSafeQ() = default;
  ThreadSafeQ(ThreadSafeQ&&) = default;
  ThreadSafeQ& operator=(ThreadSafeQ&&) = default;
  ~ThreadSafeQ() { Clear(); }
  /// returns the first element in the queue
  const T& Front() const noexcept {
    scoped_lock lock(q_mutex_);
    return message_que_.front();
  }

  /// returns the first element in the queue removing it from the queue
  T PopFront() {
    scoped_lock lock(q_mutex_);
    auto t = std::move(message_que_.front());
    message_que_.pop_front();
    return t;
  }

  /// adds an element to the back of the queue
  template <typename... Args>
  void EmplaceBack(Args&&... args) {
    scoped_lock lock(q_mutex_);
    message_que_.emplace_back(std::forward<Args>(args)...);
  }
  /// remove target value
  void Erase(T& value) {
    scoped_lock lock(q_mutex_);
    message_que_.erase(
        std::remove(message_que_.begin(), message_que_.end(), value),
        message_que_.end());
  }

  bool Empty() {
    scoped_lock lock(q_mutex_);
    return message_que_.empty();
  }

  void Clear() {
    scoped_lock lock(q_mutex_);
    message_que_.clear();
  }

 private:
  std::deque<T> message_que_;
  mutable boost::recursive_mutex q_mutex_;
};
}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_UTILITY_THREADSAFEQ_HPP
