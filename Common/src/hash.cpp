#include "../include/hash.h"

#include <cstdint>

uint64_t SplitMix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;

    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);

    return x;
}