#ifndef PROMISE_H_
#define PROMISE_H_

#define PROMISE_SUPPORTS_EXCEPTIONS
#define PROMISE_SUPPORTS_THEN

#include "thread_variables.h"
#include <functional>
#include <thread>
#include <memory>
#include <utility>

template<typename ResultType>
class Promise : public std::enable_shared_from_this<Promise<ResultType>> {
 public:
  explicit Promise(const std::function<ResultType()>& function);

  ResultType Wait();

  template<typename F>
  std::shared_ptr<Promise<std::result_of_t<F&(ResultType)>>> Then(F function);
  template<typename H>
  std::shared_ptr<Promise<ResultType>> Catch(H handler);

 private:
  std::shared_ptr<ThreadVariables<ResultType>> thread_variables_;
};

template<>
class Promise<void> : public std::enable_shared_from_this<Promise<void>> {
 public:
  explicit Promise(const std::function<void()>& function);

  template<typename F>
  std::shared_ptr<Promise<std::result_of_t<F&()>>> Then(F function);

  void Wait();

  template<typename H>
  std::shared_ptr<Promise<void>> Catch(H handler);

 private:
  std::shared_ptr<ThreadVariables<void>> thread_variables_;
};

template<typename T>
std::shared_ptr<Promise<std::result_of_t<T&()>>> MakePromise(T function) {
  return std::make_shared<Promise<std::result_of_t<T&()>>>(function);
}

template<typename ResultType>
Promise<ResultType>::Promise(const std::function<ResultType()>& function)
    : thread_variables_(std::make_shared<ThreadVariables<ResultType>>(
    function)) {}

template<typename ResultType>
ResultType Promise<ResultType>::Wait() {
  std::unique_lock<std::mutex> unique_lock(thread_variables_->mutex);
  thread_variables_->cv.wait(unique_lock,
                             [this]() {
                               return thread_variables_->is_ready == true;
                             });
  if (thread_variables_->exception_ptr != nullptr) {
    std::rethrow_exception(thread_variables_->exception_ptr);
  }
  return *(thread_variables_->result);
}

template<typename ResultType>
template<typename F>
std::shared_ptr<Promise<std::result_of_t<F&(ResultType)>>>
Promise<ResultType>::Then(F function) {
  std::shared_ptr<Promise<ResultType>>
      current_promise = this->shared_from_this();
  return MakePromise([current_promise, function]() {
    ResultType function_variable = current_promise->Wait();

    return function(function_variable);
  });
}

template<typename ResultType>
template<typename H>
std::shared_ptr<Promise<ResultType>> Promise<ResultType>::Catch(H handler) {
  std::shared_ptr<Promise<ResultType>>
      current_promise = this->shared_from_this();
  return MakePromise([current_promise, handler]() {
    try {
      return current_promise->Wait();
    } catch (const std::exception& e) {
      return handler(e);
    }
  });
}

// VoidSpecialization
Promise<void>::Promise(const std::function<void()>& function)
    : thread_variables_(std::make_shared<ThreadVariables<void>>(
    function)) {}

void Promise<void>::Wait() {
  std::unique_lock<std::mutex> unique_lock(thread_variables_->mutex);
  thread_variables_->cv.wait(unique_lock,
                             [this]() {
                               return thread_variables_->is_ready == true;
                             });
  if (thread_variables_->exception_ptr != nullptr) {
    std::rethrow_exception(thread_variables_->exception_ptr);
  }
}

template<typename F>
std::shared_ptr<Promise<std::result_of_t<F&()>>>
Promise<void>::Then(F function) {
  std::shared_ptr<Promise<void>> current_promise = this->shared_from_this();
  return MakePromise([current_promise, function]() {
    current_promise->Wait();
    return function();
  });
}

template<typename H>
std::shared_ptr<Promise<void>> Promise<void>::Catch(H handler) {
  std::shared_ptr<Promise<void>>
      current_promise = this->shared_from_this();
  return MakePromise([current_promise, handler]() {
    try {
      current_promise->Wait();
    } catch (const std::exception& e) {
      handler(e);
    }
  });
}

#endif  // PROMISE_H_
