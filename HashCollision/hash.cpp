#include "hash.h"

int64_t Hash(const std::string& s, int64_t p, int64_t m) {
  int64_t result = 0;
  for (char ch : s) {
    result = (__int128_t(result) * p + (ch - 'a' + 1)) % m;
  }
  return result;
}

std::string FindCollision(const std::string& a,
                          int64_t p,
                          int64_t m,
                          uint8_t concurrency) {
  std::string answer;
  int64_t hash = Hash(a, p, m);
  for (int length = 3; length <= 5; length++) {
    HashMap suffixes;
    std::vector<std::thread> threads;
    for (int j = 0; j < concurrency; j++) {
      threads.emplace_back(GenerateSuffixes,
                           length, j,
                           concurrency,
                           p, m,
                           &suffixes);
    }
    for (int j = 0; j < concurrency; j++) {
      threads[j].join();
    }
    threads.clear();
    std::mutex answer_mutex;
    for (int j = 0; j < concurrency; j++) {
      threads.emplace_back(FindPrefix,
                           length, j, concurrency, p, m, hash,
                           a, &answer, &answer_mutex,
                           &suffixes);
    }
    for (int j = 0; j < concurrency; j++) {
      threads[j].join();
    }
    if (!answer.empty()) {
      return answer;
    }
  }
  return answer;
}

void GenerateSuffixes(int length,
                      int thread_number,
                      int shift,
                      int64_t p,
                      int64_t module,
                      HashMap* suffixes) {
  if (thread_number > 25) {
    return;
  }
  std::string suffix(length, 'a');
  suffix[0] += thread_number;
  while (suffix[0] <= 'z') {
    int64_t hash = Hash(suffix, p, module);
    suffixes->Add(hash, suffix);
    for (int i = length - 1; i >= 0; i--) {
      if (suffix[i] < 'z') {
        if (i == 0) {
          if ('z' - suffix[i] < shift) {
            return;
          }
          suffix[i] += shift;
        } else {
          suffix[i] += 1;
        }
        break;
      } else {
        suffix[i] = 'a';
      }
      if (i == 0) {
        return;
      }
    }
  }
}

void FindPrefix(int length,
                int thread_number,
                int shift,
                int64_t p,
                int64_t module,
                int64_t full_string_hash,
                const std::string& main_string,
                std::string* answer,
                std::mutex* answer_mutex,
                HashMap* suffixes) {
  if (thread_number > 25) {
    return;
  }
  int64_t p_in_power = 1;
  for (int i = 0; i < length; i++) {
    p_in_power *= p;
  }
  std::string prefix(length, 'a');
  prefix[0] += thread_number;
  while (prefix[0] <= 'z') {
    if (!answer->empty()) {
      return;
    }

    int64_t hash = Hash(prefix, p, module);
    int64_t prefix_hash = static_cast<__int128_t>(hash) * p_in_power % module;
    int64_t suffix_hash = (full_string_hash - prefix_hash) % module;

    std::string suffix = suffixes->Find(suffix_hash);
    if (!suffix.empty()) {
      std::string collision = prefix + suffix;
      if (collision != main_string) {
        answer_mutex->lock();
        *answer = collision;
        answer_mutex->unlock();
        return;
      }
    }

    for (int i = length - 1; i >= 0; i--) {
      if (prefix[i] < 'z') {
        if (i == 0) {
          if ('z' - prefix[i] < shift) {
            return;
          }
          prefix[i] += shift;
        } else {
          prefix[i] += 1;
        }
        break;
      } else {
        prefix[i] = 'a';
      }
      if (i == 0) {
        return;
      }
    }
  }
}
