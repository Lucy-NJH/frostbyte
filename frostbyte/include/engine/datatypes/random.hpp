#pragma once

#include "lua.h"

#include <cstdint>
#include <random>

namespace frostbyte {

struct Random {
    int64_t seed;
    std::mt19937 rng;

    Random(int64_t seed): seed(seed), rng(seed) {}
};

Random* lua_checkrandom(lua_State* L, int narg);

void open_randomlib(lua_State* L);

}; // namespace frostbyte
