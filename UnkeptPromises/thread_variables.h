#ifndef THREAD_VARIABLES_H_
#define THREAD_VARIABLES_H_

#include <functional>
#include <condition_variable>
#include <thread>
#include <memory>

template<typename ResultType>
struct ThreadVariables {
  explicit ThreadVariables(const std::function<ResultType()>& function);

  ~ThreadVariables();

  std::condition_variable cv;
  std::mutex mutex;
  std::exception_ptr exception_ptr;
  bool is_ready = false;
  std::shared_ptr<ResultType> result;
  std::thread function_thread;
};

template<>
struct ThreadVariables<void> {
  explicit ThreadVariables(const std::function<void()>& f);

  ~ThreadVariables();

  std::condition_variable cv;
  std::mutex mutex;
  std::exception_ptr exception_ptr;
  bool is_ready = false;
  std::thread function_thread;
};

template<typename ResultType>
ThreadVariables<ResultType>::
ThreadVariables(const std::function<ResultType()>& function)
    : function_thread([this, function]() {
  std::unique_lock<std::mutex> unique_lock(mutex);

  try {
    result = std::make_shared<ResultType>(function());
  } catch (const std::exception& e) {
    exception_ptr = std::current_exception();
  }

  is_ready = true;
  unique_lock.unlock();
  cv.notify_one();
}) {}

ThreadVariables<void>::
ThreadVariables(const std::function<void()>& function)
    : function_thread([this, function]() {
  std::unique_lock<std::mutex> unique_lock(mutex);

  try {
    function();
  } catch (const std::exception& e) {
    exception_ptr = std::current_exception();
  }

  is_ready = true;
  unique_lock.unlock();
  cv.notify_one();
}) {}

template<typename ResultType>
ThreadVariables<ResultType>::~ThreadVariables() {
  function_thread.join();
}

ThreadVariables<void>::~ThreadVariables() {
  function_thread.join();
}

#endif  // THREAD_VARIABLES_H_
