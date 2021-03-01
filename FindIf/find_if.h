#ifndef FIND_IF_H_
#define FIND_IF_H_

#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>

std::vector<int64_t> FindIf(int64_t lower_bound,
                            int64_t upper_bound,
                            const std::function<bool(int64_t)>& predicate,
                            uint8_t concurrency = 1);

void FindIfOnSegment(int64_t lower_bound,
                     int64_t upper_bound,
                     const std::function<bool(int64_t)>& predicate,
                     std::vector<int64_t>* answer,
                     std::mutex* mutex);

#endif  // FIND_IF_H_
