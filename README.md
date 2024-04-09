# Set ADT
Implemented a set which is an Abstract Data Type (ADT) maintaining an unordered collection of (key, item) pairs. Each key can only occur once. It starts empty and grows with new (key, item) pairs.

# Hash Table ADT
Implemented a hash table which is a set of (key, item) pairs, acting similarly to the set but with increased efficiency for large collections.

# Least Recently Used (LRU) Cache
Implemented an LRU Cache which is an ADT with limited capacity, discarding the least recently used item when storage space is needed.

**Implemented Using:** Set ADT

## Compliation
You can compile with:

```gcc -Wall -Wextra set.c hashtable.c lru.c test_LRU.c -o test_LRU```
