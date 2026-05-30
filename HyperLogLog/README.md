# HyperLogLog

A from-scratch C++ implementation of HyperLogLog, a probabilistic data structure used to estimate the number of distinct elements using fixed memory.

## Why HyperLogLog?

Benchmark Highlight:

| Metric | HyperLogLog | `std::unordered_set` |
|----------|----------|----------|
| Distinct Elements | 10⁸ | 10⁸ |
| Memory | 4 MB | ≥ 762 MB |
| Insert Time | 2.22 s | 50.81 s |
| Error | 0.16% | 0% |

> HyperLogLog estimated 100,000,000 distinct values using only **4 MB** of memory, achieving **0.16%** Relative error while being **22.9x** faster than exact counting with `std::unordered_set`.

---

## What Problem Does HyperLogLog Solve?

Suppose we want to answer:

> How many unique users visited a website today?

The exact solution is:

```cpp
std::unordered_set<UserId> users;
```

which stores every user ID.

This gives perfect accuracy, but memory grows linearly with the number of unique users.

HyperLogLog trades a small amount of accuracy for a massive reduction in memory usage.

---

## Core Idea

1. Hash every value.
2. Use the most significant bits to choose a register.
3. Count leading zeros in the remaining bits.
4. Store the maximum value seen in each register.
5. Use the harmonic mean of the registers to estimate cardinality.

---

## Example Layout

For:

```text
register_bits = 10
```

we get:

```text
1024 registers
```

A 64-bit hash is split into:

```text
[ register id ][ remaining bits ]
```

Example:

```text
1010110010 | 000000001001010...
^^^^^^^^^^
register
```

The remaining bits are used to compute:

```text
rho(x)
```

which is the number of leading zeros plus one.

---

## Why Harmonic Mean?

HyperLogLog uses the harmonic mean because it is resistant to large outliers.

Example:

```text
1023 registers = 0
1 register = 49
```

After conversion:

```text
1023 values = 1
1 value = 2^49
```

An arithmetic mean would be dominated by the outlier.

The harmonic mean remains close to 1, producing a much more stable estimator.

---

## Complexity

| Operation | Complexity |
|------------|------------|
| Insert | O(1) |
| Estimate Cardinality | O(m) |
| Memory | O(m) |

where:

```text
m = number of registers
```

---

## Benchmark Results

### Scaling Benchmark

| Cardinality | HLL Memory | HashMap Memory* | Error | HLL Insert | HashMap Insert | Speedup |
|------------|------------|------------|------------|------------|------------|------------|
| 10⁶ | 4 KB | 7.63 MB | 2.26% | 0.022 s | 0.216 s | 9.7x |
| 10⁷ | 128 KB | 76.29 MB | 0.50% | 0.214 s | 4.30 s | 20.1x |
| 10⁸ | 4 MB | 762.94 MB | 0.16% | 2.22 s | 50.81 s | 22.9x |

\* HashMap memory is a lower bound assuming unique `uint64_t` values and does not include bucket storage, allocator metadata, node overhead, or load factor overhead.


---

## Tests

Implemented tests:

- Constructor validation
- Leading zero counting
- Register extraction
- Basic insertion and estimation

---

## Interesting Findings

- HyperLogLog estimated 100 million distinct elements with only 0.16% error.
- 4 MB was sufficient to estimate 100 million unique values.
- HyperLogLog insertion was up to 22.9x faster than exact counting.
- Estimation time grows with the number of registers because every register must be scanned.
- The raw estimator performs poorly for very small cardinalities.

Example:

```text
1 unique value inserted repeatedly
```

can produce an estimate around:

```text
738
```

Production implementations address this using Linear Counting.

---

## Limitations

This implementation contains the raw HyperLogLog estimator.

Not implemented:

- Linear Counting
- Sparse Representation
- Bias Correction
- Large Range Correction
- Merge Support

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

- Linear Counting
- Sparse HyperLogLog
- Merge operations
- Bias correction tables
- Large range correction
- Register compression
- SuperLogLog experiments

---

## AI Usage

The implementation, debugging, testing, and benchmarking were performed manually.

LLMs were used for discussion during development and for drafting this README, but all source code was written manually.