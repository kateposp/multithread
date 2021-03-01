#include <gtest/gtest.h>
#include "find_if.h"
#include <random>

bool SimplePredicate(int64_t a) {
  return a % 7 == 0;
}

bool ComplexPredicate(int64_t a) {
  int64_t abs_a = std::abs(a);
  for (int64_t i = 2; i * i <= abs_a; i++) {
    if (abs_a % i == 0) {
      return false;
    }
  }
  return true;
}

std::vector<int64_t> GetAns(int64_t lower_bound, int64_t upper_bound,
                            const std::function<bool(int64_t)>& predicate) {
  std::vector<int64_t> answer;
  for (int64_t number = lower_bound; number <= upper_bound; number++) {
    if (predicate(number)) {
      answer.push_back(number);
    }
  }
  return answer;
}

TEST(FindIfOneThread, SmallNumbers) {
  ASSERT_EQ(FindIf(1, 7, SimplePredicate, 1), std::vector<int64_t>{7});
  ASSERT_EQ(FindIf(1, 5, SimplePredicate, 1), std::vector<int64_t>{});
  ASSERT_EQ(FindIf(3, -5, SimplePredicate, 1), std::vector<int64_t>{});
  ASSERT_EQ(FindIf(-10, 10, SimplePredicate, 1),
            (std::vector<int64_t>{-7, 0, 7}));
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_int_distribution<int64_t> rndm(-1e4, 1e4);
  for (int i = -100; i <= 100; i++) {
    int64_t a = rndm(gen);
    int64_t b = rndm(gen);
    std::vector<int64_t> real_result =
        FindIf(std::min(a, b), std::max(a, b), SimplePredicate, 1);
    std::sort(real_result.begin(), real_result.end());
    ASSERT_EQ(real_result,
              GetAns(std::min(a, b), std::max(a, b), SimplePredicate));
  }
}

TEST(FindIfOneThread, BigNumbers) {
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_int_distribution<int64_t> rndm1(INT64_MIN, INT64_MIN + 1e4);
  std::uniform_int_distribution<int64_t> rndm2(INT64_MAX - 1e4, INT64_MAX);
  for (int i = -100; i <= 100; i++) {
    int64_t a = rndm1(gen);
    int64_t b = rndm1(gen);
    std::vector<int64_t> real_result =
        FindIf(std::min(a, b), std::max(a, b), SimplePredicate, 1);
    std::sort(real_result.begin(), real_result.end());
    ASSERT_EQ(real_result,
              GetAns(std::min(a, b), std::max(a, b), SimplePredicate));
  }
  for (int i = -100; i <= 100; i++) {
    int64_t a = rndm2(gen);
    int64_t b = rndm2(gen);
    std::vector<int64_t> real_result =
        FindIf(std::min(a, b), std::max(a, b), SimplePredicate, 1);
    std::sort(real_result.begin(), real_result.end());
    ASSERT_EQ(real_result,
              GetAns(std::min(a, b), std::max(a, b), SimplePredicate));
  }
}

TEST(FindIfTwoThreads, SmallNumbers) {
  ASSERT_EQ(FindIf(1, 7, SimplePredicate, 2), std::vector<int64_t>{7});
  ASSERT_EQ(FindIf(1, 5, SimplePredicate, 2), std::vector<int64_t>{});
  ASSERT_EQ(FindIf(3, -5, SimplePredicate, 2), std::vector<int64_t>{});
  ASSERT_EQ(FindIf(-10, 10, SimplePredicate, 2),
            (std::vector<int64_t>{-7, 0, 7}));
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_int_distribution<int64_t> rndm(-1e4, 1e4);
  for (int i = -100; i <= 100; i++) {
    int64_t a = rndm(gen);
    int64_t b = rndm(gen);
    std::vector<int64_t> real_result =
        FindIf(std::min(a, b), std::max(a, b), SimplePredicate, 2);
    std::sort(real_result.begin(), real_result.end());
    ASSERT_EQ(real_result,
              GetAns(std::min(a, b), std::max(a, b), SimplePredicate));
  }
}

TEST(FindIfTwoThreads, BigNumbers) {
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_int_distribution<int64_t> rndm1(INT64_MIN, INT64_MIN + 1e4);
  std::uniform_int_distribution<int64_t> rndm2(INT64_MAX - 1e4, INT64_MAX);
  for (int i = -100; i <= 100; i++) {
    int64_t a = rndm1(gen);
    int64_t b = rndm1(gen);
    std::vector<int64_t> real_result =
        FindIf(std::min(a, b), std::max(a, b), SimplePredicate, 2);
    std::sort(real_result.begin(), real_result.end());
    ASSERT_EQ(real_result,
              GetAns(std::min(a, b), std::max(a, b), SimplePredicate));
  }
  for (int i = -100; i <= 100; i++) {
    int64_t a = rndm2(gen);
    int64_t b = rndm2(gen);
    std::vector<int64_t> real_result =
        FindIf(std::min(a, b), std::max(a, b), SimplePredicate, 2);
    std::sort(real_result.begin(), real_result.end());
    ASSERT_EQ(real_result,
              GetAns(std::min(a, b), std::max(a, b), SimplePredicate));
  }
}

TEST(FindIf, ManyThreads) {
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_int_distribution<int64_t> rndm2(INT64_MAX - 1e4, INT64_MAX);
  std::vector<uint8_t> streams = {3, 4, 5, 6, 7, 8, 10, 12, 15};
  for (int i = -100; i <= 100; i++) {
    int64_t a = rndm2(gen);
    int64_t b = rndm2(gen);
    for (int j = 0; j < streams.size(); j++) {
      std::vector<int64_t> real_result =
          FindIf(std::min(a, b), std::max(a, b), SimplePredicate, streams[j]);
      std::sort(real_result.begin(), real_result.end());
      ASSERT_EQ(real_result,
                GetAns(std::min(a, b), std::max(a, b), SimplePredicate));
    }
  }
}

TEST(FindIfOneThread, ComplexPredicate) {
  ASSERT_EQ(FindIf(1, 7, ComplexPredicate, 1),
            (std::vector<int64_t>{1, 2, 3, 5, 7}));
  ASSERT_EQ(FindIf(8, 10, ComplexPredicate, 1), std::vector<int64_t>{});
  ASSERT_EQ(FindIf(3, -5, ComplexPredicate, 1), std::vector<int64_t>{});
  ASSERT_EQ(FindIf(-10, 10, ComplexPredicate, 1),
            (std::vector<int64_t>{-7, -5, -3, -2, -1, 0, 1, 2, 3, 5, 7}));
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_int_distribution<int64_t> rndm(-1e4, 1e4);
  for (int i = 0; i <= 50; i++) {
    int64_t a = rndm(gen);
    int64_t b = rndm(gen);
    std::vector<int64_t> real_result =
        FindIf(std::min(a, b), std::max(a, b), ComplexPredicate, 1);
    std::sort(real_result.begin(), real_result.end());
    ASSERT_EQ(real_result,
              GetAns(std::min(a, b), std::max(a, b), ComplexPredicate));
  }
}

TEST(FindIf, ManyThreadsComplexPredicate) {
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_int_distribution<int64_t> rndm2(0, 1e5);
  std::vector<uint8_t> streams = {3, 4, 5, 6, 7, 8, 10, 12, 15};
  for (int i = 0; i < 5; i++) {
    int64_t a = rndm2(gen);
    int64_t b = rndm2(gen);
    for (int j = 0; j < streams.size(); j++) {
      std::vector<int64_t> real_result =
          FindIf(std::min(a, b), std::max(a, b), ComplexPredicate, streams[j]);
      std::sort(real_result.begin(), real_result.end());
      ASSERT_EQ(real_result,
                GetAns(std::min(a, b), std::max(a, b), ComplexPredicate));
    }
  }
}

TEST(FindIf, ThreadsMoreThanNumbers) {
  std::vector<int64_t> real_result =
      FindIf(10, 15, ComplexPredicate, 100);
  std::sort(real_result.begin(), real_result.end());
  ASSERT_EQ(real_result, GetAns(10, 15, ComplexPredicate));

  real_result = FindIf(-1025, -1000, ComplexPredicate, 27);
  std::sort(real_result.begin(), real_result.end());
  ASSERT_EQ(real_result, GetAns(-1025, -1000, ComplexPredicate));

  real_result = FindIf(-100000025, -100000000, ComplexPredicate, 250);
  std::sort(real_result.begin(), real_result.end());
  ASSERT_EQ(real_result, GetAns(-100000025, -100000000, ComplexPredicate));

  real_result = FindIf(-970, -950, ComplexPredicate, 21);
  std::sort(real_result.begin(), real_result.end());
  ASSERT_EQ(real_result, GetAns(-970, -950, ComplexPredicate));
}
