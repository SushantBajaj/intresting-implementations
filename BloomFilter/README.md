using splitmix64 is better for hashing ints than inbuilt hash since that usually just return x%n
deletes dont work in this bloomfilter, for that we will have to implement bloomfilter with count in each element, which will increase the storage *32 or *64
since we need multiple hashing for the same value in bloom filter impl, the naive way is to make n diffrent hash functions, or use some a single hash function but use some sort of input combined with current hash iteration number to get unique hashes, but in the paper Less Hashing, Same Performance: Building a Better Bloom Filter, we see that using 
gi(x) = h1(x) + i * h2(x)
gives the same performance as uniform hashing
but rather than using 2 diffrent hashing functions, for simplicity purpose, we will use the same hashing function twice, with the second one using the output of the first one as input, this would essentially simulate double hashing
gi (x) = h1(x) + i* h1(h1(x))