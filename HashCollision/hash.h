#ifndef HASH_H_
#define HASH_H_

#include<iostream>
#include<string>
#include <vector>
#include <mutex>
#include <thread>
#include <map>
#include "hash_map.h"

int64_t Hash(const std::string& s, int64_t p, int64_t m);

std::string FindCollision(const std::string& a,
                          int64_t p,
                          int64_t m,
                          uint8_t concurrency);

void GenerateSuffixes(int length,
                      int stream_number,
                      int shift,
                      int64_t p,
                      int64_t m,
                      HashMap* suffixes);

void FindPrefix(int length,
                int thread_number,
                int shift,
                int64_t p,
                int64_t module,
                int64_t full_string_hash,
                const std::string& main_string,
                std::string* answer,
                std::mutex* answer_mutex,
                HashMap* suffixes);

#endif  // HASH_H_
