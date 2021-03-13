#include <gtest/gtest.h>
#include "hash.h"
#include <random>

const int p = 31;

const int64_t small_module = 99971;
const int64_t middle_module = 1'000'000'007;
const int64_t big_module = 1'000'000'000'000;

TEST(HashCollisionOneThread, SimpleTest) {
  std::string answer = FindCollision("aaaaaa", 31, small_module, 1);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("aaaaaa", 31, small_module));
  ASSERT_NE(answer, "aaaaaa");
  answer = FindCollision("slkfsdeo", 31, small_module, 1);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("slkfsdeo", 31, small_module));
  ASSERT_NE(answer, "slkfsdeo");
  answer = FindCollision("sc", 31, small_module, 1);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("sc", 31, small_module));
  ASSERT_NE(answer, "sc");
  answer = FindCollision("s", 31, small_module, 1);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("s", 31, small_module));
  ASSERT_NE(answer, "s");
  answer = FindCollision("asd", 31, small_module, 1);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("asd", 31, small_module));
  ASSERT_NE(answer, "asd");
  answer = FindCollision("qwfs", 31, small_module, 1);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("qwfs", 31, small_module));
  ASSERT_NE(answer, "qwfs");
  answer = FindCollision("pokgd", 31, small_module, 1);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("pokgd", 31, small_module));
  ASSERT_NE(answer, "pokgd");
  answer = FindCollision("kgdgmpq", 31, small_module, 1);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("kgdgmpq", 31, small_module));
  ASSERT_NE(answer, "kgdgmpq");
}

TEST(HashCollisionTwoThreads, SimpleTest) {
  std::string answer = FindCollision("aaaaaa", 31, small_module, 2);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("aaaaaa", 31, small_module));
  ASSERT_NE(answer, "aaaaaa");
  answer = FindCollision("slkfsdeo", 31, small_module, 2);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("slkfsdeo", 31, small_module));
  ASSERT_NE(answer, "slkfsdeo");
  answer = FindCollision("sc", 31, small_module, 2);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("sc", 31, small_module));
  ASSERT_NE(answer, "sc");
  answer = FindCollision("s", 31, small_module, 2);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("s", 31, small_module));
  ASSERT_NE(answer, "s");
  answer = FindCollision("asd", 31, small_module, 2);
  ASSERT_EQ(Hash(answer, 31, small_module), Hash("asd", 31, small_module));
  ASSERT_NE(answer, "asd");
}

TEST(HashCollisionTwoThreads, LongStringMiddleModule) {
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_int_distribution<int64_t> rndm(0, 25);
  std::string s;
  s.resize(1e5);
  for (int test = 0; test <= 10; test++) {
    for (int i = 0; i < 1e5; i++) {
      s[i] = 'a' + rndm(gen);
    }
    std::string answer = FindCollision(s, 31, middle_module, 2);
    ASSERT_EQ(Hash(answer, 31, middle_module), Hash(s, 31, middle_module));
    ASSERT_NE(answer, s);
  }
}

TEST(HashCollisionManyThreads, LongStringMiddleModule) {
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_int_distribution<int64_t> rndm(0, 25);
  std::string s;
  s.resize(1e5);
  for (int concurrency = 1; concurrency <= 8; concurrency++) {
    for (int i = 0; i < 1e5; i++) {
      s[i] = 'a' + rndm(gen);
    }
    std::string answer = FindCollision(s, 31, middle_module, concurrency);
    ASSERT_EQ(Hash(answer, 31, middle_module), Hash(s, 31, middle_module));
    ASSERT_NE(answer, s);
  }
}

TEST(HashCollisionManyThreads, LongStringBigModule) {
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_int_distribution<int64_t> rndm(0, 25);
  std::string s;
  s.resize(1e5);
  for (int concurrency = 1; concurrency <= 8; concurrency++) {
    for (int i = 0; i < 1e5; i++) {
      s[i] = 'a' + rndm(gen);
    }
    std::string answer = FindCollision(s, 31, big_module, concurrency);
    ASSERT_EQ(Hash(answer, 31, big_module), Hash(s, 31, big_module));
    ASSERT_NE(answer, s);
  }
}

TEST(HashCollision, ExtraBigThreads) {
  std::random_device device;
  std::mt19937 gen(device());
  std::uniform_int_distribution<int64_t> rndm(0, 25);
  std::string s;
  s.resize(1e5);
  for (int i = 0; i < 1e5; i++) {
    s[i] = 'a' + rndm(gen);
  }
  std::string answer = FindCollision(s, 31, big_module, 250);
  ASSERT_EQ(Hash(answer, 31, big_module), Hash(s, 31, big_module));
  ASSERT_NE(answer, s);
}
