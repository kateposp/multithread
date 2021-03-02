#include "hash_map.h"

HashMap::HashMap() : mutexes(50), blocks(5000000) {}

void HashMap::Add(int64_t hash, const std::string& s) {
  int64_t block_number = hash % static_cast<int64_t>(blocks.size());
  int64_t mutex_block_number = block_number % mutexes.size();
  mutexes[mutex_block_number].lock();
  blocks[block_number].emplace_back(hash, s);
  mutexes[mutex_block_number].unlock();
}

std::string HashMap::Find(int64_t hash) const {
  if (hash < 0) {
    return std::string();
  }
  int64_t block_number = hash % static_cast<int64_t>(blocks.size());
  for (const auto& hash_string : blocks[block_number]) {
    if (hash_string.first == hash) {
      return hash_string.second;
    }
  }
  return std::string();
}
