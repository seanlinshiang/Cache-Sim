# Cache Simulation
Simulate cache behavior with different cache size, block size, associativity and replace policy.
## Usage
```
cache <cache size> <block size> <associativity> <replace policy> <file name>
```
  1. cache size : 8, 16, 32, ..., 256 (KB)
  2. block size : 4, 8, 16, ..., 128 (B)
  3. associativity : 1 (direct-mapped), 2, 4, 8, f (fully associative)
  4. replace policy : FIFO, LRU
  5. file name : spice.din, gcc.din
 ## Output
1. Input file
2. Demand fetch
3. Cache hit
4. Cache miss
5. Miss rate
6. Read data
7. Write data
8. Bytes from Memory
9. Byte to memory
