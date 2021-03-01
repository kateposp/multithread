#include "find_if.h"

std::vector<int64_t> FindIf(int64_t lower_bound,
                            int64_t upper_bound,
                            const std::function<bool(int64_t)>& predicate,
                            uint8_t concurrency) {
  if (upper_bound < lower_bound) {
    return {};
  }
  std::vector<int64_t> answer;
  std::mutex v_mutex;
  std::vector<std::thread> threads;
  int64_t start = lower_bound;
  int64_t step =
      (upper_bound - lower_bound + 1) / static_cast<int64_t>(concurrency);
  if (step == 0) {
    concurrency = upper_bound - lower_bound + 1;
    step = 1;
  }
  for (int i = 0; i < concurrency; i++) {
    int64_t end = start + step - 1;
    if (i == concurrency - 1) {
      end = upper_bound;
    }
    threads.emplace_back(FindIfOnSegment,
                         start,
                         end,
                         predicate,
                         &answer,
                         &v_mutex);
    start += step;
  }
  for (int i = 0; i < concurrency; i++) {
    if (threads[i].joinable()) {
      threads[i].join();
    }
  }
  return answer;
}

void FindIfOnSegment(int64_t lower_bound,
                     int64_t upper_bound,
                     const std::function<bool(int64_t)>& predicate,
                     std::vector<int64_t>* answer,
                     std::mutex* mutex) {
  for (int64_t i = lower_bound; i <= upper_bound; i++) {
    if (predicate(i)) {
      mutex->lock();
      answer->push_back(i);
      mutex->unlock();
    }
  }
}
