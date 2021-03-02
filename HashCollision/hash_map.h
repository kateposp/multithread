#ifndef HASH_MAP_H_
#define HASH_MAP_H_

#include <iostream>
#include <vector>
#include <mutex>
#include <string>
#include <utility>

class HashMap {
 public:
    HashMap();
    void Add(int64_t hash, const std::string& s);
    std::string Find(int64_t hash) const;

 private:
  std::vector<std::mutex> mutexes;
  std::vector<std::vector<std::pair<int64_t, std::string>>> blocks;
};

#endif  // HASH_MAP_H_
