#include "bloom_filter.h"

#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <random>

uint64_t Random64() {
    static std::mt19937_64 rng(std::random_device{}());
    return rng();
}

std::string Scientific(size_t x) {
    size_t exponent = 0;
    size_t temp = x;

    while (temp % 10 == 0 && temp > 1) {
        temp /= 10;
        ++exponent;
    }

    if (temp == 1) {
        return "10^" + std::to_string(exponent);
    }

    return std::to_string(x);
}

void PrintMemory(const std::string& name, double bytes) {
    if (bytes < 1024.0 * 1024.0) {
        std::cout << name << " = "
                  << bytes / 1024.0
                  << " KB\n";
    } else {
        std::cout << name << " = "
                  << bytes / (1024.0 * 1024.0)
                  << " MB\n";
    }
}

void Benchmark(size_t inserts,
               size_t bits,
               size_t hash_functions) {

    std::cout << "========================================" << std::endl;
    std::cout << "insertions = " << Scientific(inserts) << std::endl;
    std::cout << "bits = " << Scientific(bits) << std::endl;
    std::cout << "hash functions = " << hash_functions << std::endl;

    PrintMemory("bloom filter memory", bits / 8.0);
    PrintMemory("hashset memory (MINIMUM)",
                inserts * sizeof(uint64_t));

    std::cout << "========================================" << std::endl;

    std::vector<uint64_t> inserted(inserts);
    std::vector<uint64_t> queries(inserts);

    for(size_t i = 0; i < inserts; i++) {
        inserted[i] = Random64();
        queries[i] = Random64();
    }

    BloomFilter bf(bits, hash_functions);

    std::unordered_set<uint64_t> hashset;
    hashset.reserve(inserts);

    auto start =
        std::chrono::high_resolution_clock::now();

    for(auto x : inserted) {
        bf.insertHash(x);
    }

    auto end =
        std::chrono::high_resolution_clock::now();

    auto bloom_insert_time =
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                end - start
            ).count();

    start =
        std::chrono::high_resolution_clock::now();

    for(auto x : inserted) {
        hashset.insert(x);
    }

    end =
        std::chrono::high_resolution_clock::now();

    auto hashmap_insert_time =
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                end - start
            ).count();

    // ========================================
    // Positive lookups
    // ========================================

    start =
        std::chrono::high_resolution_clock::now();

    size_t bloom_positive_hits = 0;

    for(auto x : inserted) {
        bloom_positive_hits += bf.containsHash(x);
    }

    end =
        std::chrono::high_resolution_clock::now();

    auto bloom_positive_lookup_time =
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                end - start
            ).count();

    start =
        std::chrono::high_resolution_clock::now();

    size_t hashmap_positive_hits = 0;

    for(auto x : inserted) {
        hashmap_positive_hits += hashset.contains(x);
    }

    end =
        std::chrono::high_resolution_clock::now();

    auto hashmap_positive_lookup_time =
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                end - start
            ).count();

    // ========================================
    // Negative lookups / false positives
    // ========================================

    start =
        std::chrono::high_resolution_clock::now();

    size_t bloom_hits = 0;

    for(auto x : queries) {
        bloom_hits += bf.containsHash(x);
    }

    end =
        std::chrono::high_resolution_clock::now();

    auto bloom_negative_lookup_time =
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                end - start
            ).count();
    
    size_t hashmap_negative_hits = 0;

    start =
        std::chrono::high_resolution_clock::now();

    for(auto x : queries) {
        hashmap_negative_hits += hashset.contains(x);
    }

    end =
        std::chrono::high_resolution_clock::now();

    auto hashmap_negative_lookup_time =
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                end - start
            ).count();

    double false_positive_rate =
        bloom_hits * 100.0 / inserts;

    // ========================================
    // Output
    // ========================================

    std::cout << "false positives = "
              << bloom_hits
              << std::endl;

    std::cout << "false positive rate = "
              << false_positive_rate
              << "%"
              << std::endl;

    std::cout << std::endl;

    std::cout << "hashset insertion time = "
              << hashmap_insert_time / 1000000.0
              << " s"
              << std::endl;

    std::cout << "bloom insertion time = "
              << bloom_insert_time / 1000000.0
              << " s"
              << std::endl;

    std::cout << std::endl;

    std::cout << "insert speedup = "
              << (double)hashmap_insert_time
                 / bloom_insert_time
              << "x"
              << std::endl;

    std::cout << std::endl;

    std::cout << "Positive Lookup Benchmark"
              << std::endl;

    std::cout << "hashset lookup time = "
              << hashmap_positive_lookup_time / 1000000.0
              << " s"
              << std::endl;

    std::cout << "bloom lookup time = "
              << bloom_positive_lookup_time / 1000000.0
              << " s"
              << std::endl;

    std::cout << "lookup speedup = "
              << (double)hashmap_positive_lookup_time
                 / bloom_positive_lookup_time
              << "x"
              << std::endl;

    std::cout << std::endl;

    std::cout << "Negative Lookup Benchmark"
              << std::endl;

    std::cout << "hashset lookup time = "
              << hashmap_negative_lookup_time / 1000000.0
              << " s"
              << std::endl;

    std::cout << "bloom lookup time = "
              << bloom_negative_lookup_time / 1000000.0
              << " s"
              << std::endl;

    std::cout << "lookup speedup = "
              << (double)hashmap_negative_lookup_time
                 / bloom_negative_lookup_time
              << "x"
              << std::endl;

    std::cout << "========================================"
              << std::endl;

    std::cout << std::endl;
}

int main() {

    std::cout << "Bit Sensitivity Benchmark"
              << std::endl;

    Benchmark(1000000, 1000000, 5);
    Benchmark(1000000, 10000000, 5);
    Benchmark(1000000, 100000000, 5);

    std::cout << "Hash Function Sensitivity Benchmark"
              << std::endl;

    Benchmark(1000000, 10000000, 1);
    Benchmark(1000000, 10000000, 3);
    Benchmark(1000000, 10000000, 5);
    Benchmark(1000000, 10000000, 7);
    Benchmark(1000000, 10000000, 10);

    std::cout << "Cardinality Scaling Benchmark"
              << std::endl;

    Benchmark(1000000,    10000000,   5);
    Benchmark(10000000,   100000000,  5);
    Benchmark(100000000,  1000000000, 5);
}