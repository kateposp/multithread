#include <benchmark/benchmark.h>
#include "find_if.h"

bool SimplePredicate(int64_t a) {
  return a % 3 == 0;
}

bool ComplexPredicate(int64_t a) {
  for (int64_t i = 2; i * i <= a; i++) {
    if (a % i == 0) {
      return false;
    }
  }
  return true;
}

int64_t lower_bound = 0;
int64_t upper_bound = 1e6;

static void BM_FindIfSimplePredicate(benchmark::State& state) {
  for (auto _ : state)
    FindIf(lower_bound, upper_bound, SimplePredicate, state.range(0));
}

BENCHMARK(BM_FindIfSimplePredicate)->Unit(benchmark::kMillisecond)
    ->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8);


static void BM_FindIfComplexPredicate(benchmark::State& state) {
    for (auto _ : state) {
    FindIf(lower_bound, upper_bound, ComplexPredicate, state.range(0));
  }
}

BENCHMARK(BM_FindIfComplexPredicate)->Unit(benchmark::kMillisecond)
    ->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8);


BENCHMARK_MAIN();