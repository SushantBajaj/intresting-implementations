# Bloom Filter

A from-scratch C++ implementation of a Bloom Filter, a probabilistic data structure used for fast membership testing with fixed memory.

## Why Bloom Filters?

Benchmark Highlight:

| Metric               | Bloom Filter | `std::unordered_set` |
| -------------------- | ------------ | -------------------- |
| Elements Inserted    | 10⁷          | 10⁷                  |
| Memory               | 11.9 MB      | ≥ 76.3 MB            |
| False Positive Rate  | 0.94%        | 0%                   |
| Negative Lookup Time | 0.59 s       | 2.67 s               |

> With a false positive rate below **1%**, the Bloom Filter used **6.4x less memory** and performed negative lookups **4.5x faster** than `std::unordered_set`.

---

## What Problem Does a Bloom Filter Solve?

Suppose we want to answer:

```text
Have we seen this value before?
```

The exact solution is:

```cpp
std::unordered_set<uint64_t> values;
```

which stores every element.

This guarantees correct answers, but memory grows linearly with the number of stored values.

A Bloom Filter trades perfect accuracy for significantly lower memory usage.

The tradeoff is:

```text
No false negatives
Possible false positives
```

Meaning:

```text
Not Present  -> Always Correct
Present      -> Probably Correct
```

---

## Core Idea

A Bloom Filter consists of:

```text
Bit Array
+
Multiple Hash Functions
```

Insertion:

1. Hash the value multiple times.
2. Map each hash into the bit array.
3. Set all corresponding bits.

Lookup:

1. Recompute the same hashes.
2. Check the corresponding bits.

If any bit is zero:

```text
Definitely not present
```

If all bits are one:

```text
Probably present
```

---

## Hashing Strategy

A Bloom Filter requires multiple hash functions per element.

The naive approach is to implement:

```text
k independent hash functions
```

Instead, this implementation uses **double hashing**, inspired by:

[Less Hashing, Same Performance: Building a Better Bloom Filter](https://www.eecs.harvard.edu/~michaelm/postscripts/rsa2008.pdf)

The paper shows that:

```text
gᵢ(x) = h₁(x) + i · h₂(x)
```

performs similarly to fully independent hashing.

For simplicity, this implementation derives the second hash from the first:

```text
gᵢ(x) = h₁(x) + i · h₁(h₁(x))
```

using SplitMix64 as the underlying hash function.

---

## Why SplitMix64?

For integer keys, `std::hash<uint64_t>` is often close to:

```cpp
return x;
```

which provides little mixing.

SplitMix64 provides strong bit diffusion while remaining extremely fast, making it well suited for Bloom Filter indexing.

---

## Complexity

| Operation | Complexity |
| --------- | ---------- |
| Insert    | O(k)       |
| Lookup    | O(k)       |
| Memory    | O(m)       |

where:

```text
k = number of hash functions
m = number of bits
```

---

## Benchmark Results

### Bit Array Sensitivity

| Bits | Memory  | False Positive Rate |
| ---- | ------- | ------------------- |
| 10⁶  | 122 KB  | 96.68%              |
| 10⁷  | 1.19 MB | 0.94%               |
| 10⁸  | 11.9 MB | ~0%                 |

Increasing the bit array dramatically reduces false positives.

---

### Hash Function Sensitivity

| Hash Functions | False Positive Rate |
| -------------- | ------------------- |
| 1              | 9.49%               |
| 3              | 1.75%               |
| 5              | 0.95%               |
| 7              | 0.82%               |
| 10             | 1.01%               |

The optimal value depends on the ratio:

```text
bits / inserted elements
```

Adding more hash functions eventually hurts performance while providing little improvement in accuracy.

---

### Cardinality Scaling

| Insertions | Bloom Memory | HashSet Memory* | False Positive Rate |
| ---------- | ------------ | --------------- | ------------------- |
| 10⁶        | 1.19 MB      | 7.63 MB         | 0.93%               |
| 10⁷        | 11.9 MB      | 76.3 MB         | 0.94%               |
| 10⁸        | 119 MB       | 762.9 MB        | 0.94%               |

* HashSet memory is a lower bound assuming unique `uint64_t` values and does not include buckets, allocator metadata, node overhead, or load-factor overhead.

---

## Tests

Implemented tests:

* Constructor validation
* True positive lookups
* Non-member lookup
* No false negatives

---

## Interesting Findings

* Bloom Filters are typically only **1–2x faster** than `std::unordered_set` for insertions and positive lookups.
* The largest gains occur during negative lookups, where speedups of **3–5x** were consistently observed.
* More hash functions do not always improve performance. Past the optimal point, lookup and insertion costs increase while false positive rates stop improving.
* At very large scales, Bloom Filters become memory-bandwidth limited and may become slower than hash tables despite using significantly less memory.
* The primary benefit of Bloom Filters is usually memory efficiency rather than raw CPU speed.

Most real-world systems use Bloom Filters as a filtering layer before expensive operations such as:

```text
Disk Reads
Database Queries
Network Requests
```

In those scenarios, avoiding unnecessary work often matters more than the Bloom Filter lookup itself.

---

## Building

```bash
mkdir build
cd build

cmake ..
make
```

Run tests:

```bash
./test
```

Run benchmarks:

```bash
./benchmark
```

---

## Future Improvements

* Counting Bloom Filter (supports deletions)
* Scalable Bloom Filter (automatic growth)
* Benchmarking Bloom Filter + HashSet hybrid architectures

---

## AI Usage

The implementation, debugging, testing, and benchmarking were performed manually.

LLMs were used for discussion during development and for drafting this README, but all source code was written manually.
