why not use the entire hash key rather than hashkey % arr size-
because 1st it wont be scalabale, the array will forever be stuck to 64 eles, which on big number of queries will give no meaningful output ie shitty collisions, and second, even if that wasnt an issue, this would increase the lookup and insertion time almost 64 folds, and we will more work than an hashmap, just to give a probabilistic answer 

will try to use a flat vector rather than a n*m size vector for this, because apparently cpu works better with flat ones

hashing is same as used in bloom filter ie double hashing