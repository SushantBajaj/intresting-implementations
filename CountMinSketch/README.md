why not use the entire hash key rather than hashkey % arr size-
because 1st it wont be scalabale, the array will forever be stuck to 64 eles, which on big number of queries will give no meaningful output ie shitty collisions, and second, even if that wasnt an issue, this would increase the lookup and insertion time almost 64 folds, and we will more work than an hashmap, just to give a probabilistic answer 

will try to use a flat vector rather than a n*m size vector for this, because apparently cpu works better with flat ones

hashing is same as used in bloom filter ie double hashing, from the paper Less Hashing, Same Performance: Building a Better Bloom Filter

trying out uint64_t[] rather than vector, not for performance gains, idts there will be that, but to try out rule of 3/5

trying to make this thread safe

learnt about shared_mutex, now what im able to do is, the object will have a shared_mutex lock ans increment method would wait, if any other thread is executing any other method that puts a lock, but it wont have to wait, if any other thread is just using the increment method, basically this lets multiple threads increment at once, but as soon as any thread calls some critical method, no thread can use increment unless the critical method ends

also this wont hurt increment, as even tho shared_lock is usually used for read operations, here it wont be a problem as we are using atomics for increment, so by itself the increment method is already thread safe without any gaurds

had to drop memcpy and use a for loop to copy, as atomic_uint64_t doesnt give a copy construtor or operator, ie they are not meant to be copied

faced an issue, circular deadlock, some methods needs to acquire lock of both the current object and an other object, now this can cause issues like if thread 1 tries to cms1.merge(cms2),while the other thread tries to do cms2.merge(cms1),both acquire their own locks,and hence none will be able to acquire the lock of the other,and be stuck forever

learnt about std::lock() and std::scoped_lock() for this, basically they try to lock all the lockable objects given to them, if they fail to lock any one, they unlock all the ones they locked earlier, and retry,i assume in a while loop

fits my purpose perfectly, but now theres the other issue that, acquireLock returns a unique_lock which has already acquired the lock on the mutex,hence the std::scoped_lock wont be able to acquire lock of its own, now to tackle this, learnt about locking strats, basically there are 3 options, that with the default, makes it total 4, here we used defer_lock, which basically just tells the unique_lock object to not to try to acquire lock, this way we were able to return a unique_lock without acquring the lock, and finally solved the problem of circular deadlock
multi-object sync was def one of the most intresting things i got to learn from this impl