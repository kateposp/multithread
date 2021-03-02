#include <gtest/gtest.h>
#include "promise.h"

int IntFunction() {
  return 3;
}

std::string StringFunction() {
  return "Hello";
}

double DoubleFunction() {
  return 94.142;
}

void VoidFunction() {
  int a = 1;
  a++;
}

TEST(PromiseWait, SimpleTypes) {
  int result_int = MakePromise(IntFunction)->Wait();
  ASSERT_EQ(result_int, IntFunction());

  std::string result_string = MakePromise(StringFunction)->Wait();
  ASSERT_EQ(result_string, StringFunction());

  double real_result = MakePromise(DoubleFunction)->Wait();
  ASSERT_EQ(real_result, DoubleFunction());

  MakePromise(VoidFunction)->Wait();
}

TEST(PromiseWait, PromiseFromPromise) {
  Promise<Promise<int>> promise1([]() {
    return Promise<int>(IntFunction);
  });
  ASSERT_EQ(promise1.Wait().Wait(), IntFunction());

  auto promise2 = MakePromise([]() {
    return MakePromise([]() {
      return MakePromise([]() {
        return "Correct function";
      });
    });
  });
  ASSERT_STREQ(promise2->Wait()->Wait()->Wait(), "Correct function");
}

double DoubleExceptionFunction() {
  throw std::runtime_error("Double exception");
}

double VoidExceptionFunction() {
  std::string s;
  throw std::out_of_range("Out of range");
}

TEST(PromiseException, NoExceptionTest) {
  ASSERT_NO_THROW(MakePromise(IntFunction)->Wait());
}

TEST(PromiseException, ExceptionTest) {
  ASSERT_THROW(MakePromise(DoubleExceptionFunction)->Wait(),
               std::runtime_error);
  auto promise = MakePromise(VoidExceptionFunction);
  ASSERT_THROW(promise->Wait(), std::out_of_range);
}

int IntFunctionParam(int a) {
  return a * a;
}

int DoubleFunctionParam(double a) {
  return a * 2;
}

std::string StringFunctionParam(int a) {
  return "Success " + std::to_string(a);
}

void VoidFunctionParam(const std::string& s) {
  std::string a = s;
}

int ParseInt(const std::string& s) {
  int result = 0;
  for (char ch : s) {
    if (ch < '0' || ch > '9') {
      throw std::runtime_error("Couldn't parse int: " + s);
    }
    result = result * 10 + (ch - '0');
  }
  return result;
}

int64_t ExpensiveComputation(int32_t x) {
  return x * (x - 1) * (x - 2);
}

TEST(PromiseThen, SimpleTest) {
  auto promise = MakePromise(IntFunction)
      ->Then(IntFunctionParam);
  ASSERT_EQ(promise->Wait(), 9);

  auto promise2 = MakePromise(IntFunction)
      ->Then(StringFunctionParam);
  ASSERT_STREQ(promise2->Wait().c_str(), "Success 3");

  auto result3 = MakePromise(VoidFunction)->Then(DoubleFunction);
  ASSERT_EQ(result3->Wait(), 94.142);
}

TEST(PromiseThen, ChainTest) {
  auto promise = MakePromise([] {
    return "10";
  })->Then(ParseInt)
      ->Then(ExpensiveComputation);
  ASSERT_EQ(promise->Wait(), 720);

  auto result = MakePromise(IntFunction)
      ->Then(IntFunctionParam)
      ->Then(StringFunctionParam);
  ASSERT_EQ(result->Wait(), "Success 9");

  auto result2 = MakePromise(IntFunction)
      ->Then(IntFunctionParam)
      ->Then(StringFunctionParam)
      ->Then(VoidFunctionParam)
      ->Then(IntFunction);
  ASSERT_EQ(result2->Wait(), 3);
}

TEST(PromiseCatch, SimpleTest) {
  std::atomic<bool> have_exception = false;
  auto promise = MakePromise(IntFunction)
      ->Catch([&have_exception](const std::exception& e) {
        have_exception.store(true);
        return 7;
      });
  ASSERT_EQ(promise->Wait(), 3);
  ASSERT_FALSE(have_exception.load());

  have_exception.store(false);
  auto promise1 = MakePromise(VoidExceptionFunction)
      ->Catch([&have_exception](const std::exception& e) {
        have_exception.store(true);
        return 10.0;
      });
  ASSERT_EQ(promise1->Wait(), 10);
  ASSERT_TRUE(have_exception.load());
}

TEST(PromiseCatch, ThenCatchTest) {
  std::atomic<bool> have_exception = false;
  auto result = MakePromise(DoubleExceptionFunction)
      ->Catch([&have_exception](const std::exception& error) {
        have_exception.store(true);
        return 3.0;
      });
  ASSERT_EQ(result->Wait(), 3);
  ASSERT_TRUE(have_exception.load());

  have_exception.store(false);
  auto promise1 = MakePromise([] {
    return "I'm printed later\n";
  })
      ->Then(ParseInt)
      ->Then(ExpensiveComputation)
      ->Then([](int64_t result) {
        result++;
      })
      ->Catch([&have_exception](const std::exception& e) {
        have_exception.store(true);
        ASSERT_STREQ(e.what(), "Couldn't parse int: I'm printed later\n");
      });
  promise1->Wait();
  ASSERT_TRUE(have_exception.load());
}

TEST(PromiseCatch, NoExceptionTest) {
  std::atomic<bool> have_exception = false;
  auto result2 = MakePromise(IntFunction)
      ->Then(IntFunctionParam)
      ->Then(StringFunctionParam)
      ->Then(VoidFunctionParam)
      ->Then(IntFunction)
      ->Catch([&have_exception](const std::exception e) {
        have_exception.store(true);
        return 5;
      });
  ASSERT_EQ(result2->Wait(), 3);
  ASSERT_FALSE(have_exception.load());
}

TEST(PromiseCatch, ExceptionInTheMiddleTest) {
  std::atomic<bool> have_exception = false;
  auto result4 = MakePromise(VoidFunction)
      ->Then(VoidExceptionFunction)
      ->Then(DoubleFunctionParam)
      ->Catch([&have_exception](const std::exception& e) {
        have_exception.store(true);
        return 10;
      });
  ASSERT_EQ(result4->Wait(), 10);
  ASSERT_TRUE(have_exception.load());
}