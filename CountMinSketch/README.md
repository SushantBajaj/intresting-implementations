# Count-Min Sketch

A C++ implementation of a **Count-Min Sketch (CMS)** — a probabilistic data structure used for estimating frequencies in large streams while using significantly less memory than storing exact counts.

Count-Min Sketch trades accuracy for memory efficiency. The estimated count of an element is never lower than its actual count, but can be higher due to hash collisions.

This implementation was mainly built as a learning project focused on:
- probabilistic data structures
- cache-friendly layouts
- manual memory management
- Rule of Five
- atomics
- thread-safe design

## Overview

Exact frequency counting generally requires storing every unique key and its count.

Count-Min Sketch instead keeps a fixed-size counter table.

Insertion:
- hash the item using multiple hash functions
- increment one counter per row

Lookup:
- check all corresponding counters
- return the minimum value

Since collisions only increase counters:

`estimated frequency >= actual frequency`

## Why Not Use The Entire Hash Space?

Using the complete hash value directly as an index is impractical.

A 64-bit hash space would require an enormous counter array and would remove the memory advantage of using a probabilistic data structure.

Even if memory was not a problem, operating over such a large structure would make the approach slower than necessary compared to exact counting methods.

Instead:

`index = hash % width`

This keeps memory usage fixed while accepting controlled collisions.

## Implementation Details

### Flat Memory Layout

Instead of:

`vector<vector<uint64_t>>`

the counters are stored as a flat array:

`std::atomic_uint64_t* counters_`

Index calculation:

`index = row * width + column`

Benefits:
- contiguous memory
- fewer allocations
- better cache locality

## Hashing

Uses double hashing inspired by:

"Less Hashing, Same Performance: Building a Better Bloom Filter"

Instead of generating many independent hashes, two hashes are used to derive multiple counter positions.

This reduces hashing overhead while maintaining good distribution.

## Manual Memory Management

The counter array intentionally uses raw allocation instead of containers:

`std::atomic_uint64_t*`

This was done to practice ownership handling.

Implemented:
- destructor
- copy constructor
- copy assignment
- move constructor
- move assignment

## Atomic Counters

Counters use:

`std::atomic_uint64_t`

This allows concurrent increments safely.

A normal copy cannot be performed on atomic objects, so `memcpy` cannot be used.

Instead values are copied manually:

`destination.store(source.load())`

## Thread Safety

The implementation combines:
- atomic counters
- shared mutex locking

Increment operations:
- acquire shared access
- multiple threads can increment together
- counter updates themselves are protected by atomics

Structural operations:
- clear
- merge
- assignment
- move operations

require exclusive access.

This allows concurrency while protecting object lifetime and ownership changes.

## Avoiding Multi-Object Deadlocks

Operations like:

`cms1.merge(cms2)`

need locks on two objects.

A possible issue:

Thread 1:
- locks cms1
- waits for cms2

Thread 2:
- locks cms2
- waits for cms1

This creates a circular deadlock.

The implementation solves this using:

`std::scoped_lock`

with:

`std::defer_lock`

The locks are created without immediately locking, then acquired together safely.

This was one of the most interesting parts explored in this implementation.

## Supported Operations

### Increment

`incrementHash(hash)`

Complexity:

O(number of hash functions)

### Estimate

`estimateUpperBoundOfHash(hash)`

Returns the upper bound estimate.

Complexity:

O(number of hash functions)

### Merge

Combines two sketches with the same dimensions.

### Clear

Resets all counters.

## Tests

Implemented:
- constructor tests
- invalid input handling
- overflow checks
- insertion/query tests
- overestimation behavior tests
- clear tests
- merge tests
- copy constructor
- copy assignment
- move constructor
- move assignment
- self-assignment cases

Memory correctness tested using Address Sanitizer.

## Key Learnings

- approximate algorithms
- hashing tradeoffs
- cache-aware storage
- manual resource management
- Rule of Five
- move semantics
- atomic operations
- shared/exclusive locks
- preventing deadlocks

## TODO / Future Improvements

- Write dedicated thread-safety tests
  - concurrent increments
  - concurrent reads
  - synchronization edge cases

- Benchmark against STL exact frequency counting structures
  - insertion speed
  - query speed
  - memory usage

## AI Usage

The implementation, debugging, testing were performed manually.

LLMs were used as a discussion aid during development and for drafting this README from development notes.

All source code, design decisions, tests, and benchmarks were written and implemented manually without AI-generated code.