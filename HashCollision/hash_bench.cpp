#include <benchmark/benchmark.h>
#include "hash.h"

const int64_t small_module = 99971;
const int64_t middle_module = 1e9 + 7;
const int64_t big_module = 1e12;

static void BM_HashCollisionSmallModule(benchmark::State& state) {
  for (auto _ : state) {
    FindCollision("kgmgokmvowmvpw", 31, small_module, state.range(0));
  }
}

BENCHMARK(BM_HashCollisionSmallModule)
    ->Unit(benchmark::kMillisecond)->MinTime(3)
    ->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8);

static void BM_HashCollisionMiddleModule(benchmark::State& state) {
  for (auto _ : state) {
    FindCollision("kpmpeeflewgbpdkgmgokmvowmvpw",
                  31,
                  middle_module,
                  state.range(0));
  }
}

BENCHMARK(BM_HashCollisionMiddleModule)
    ->Unit(benchmark::kMillisecond)->MinTime(3)
    ->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8);

static void BM_HashCollisionBigModule(benchmark::State& state) {
  for (auto _ : state) {
    FindCollision("bmofkbmwspekpmpeeflewgbpdkgmgokmvowmvpw",
                  31,
                  big_module,
                  state.range(0));
  }
}

BENCHMARK(BM_HashCollisionBigModule)->Unit(benchmark::kMillisecond)->MinTime(3)
    ->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8);

BENCHMARK_MAIN();
