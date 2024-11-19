# Bump Allocator Implementation

"add the variable ptr and finish the readme"

### Task 1

According to the specification, alloc() and dealloc() was supposed to be implemented.

```cpp
private:
    char* memory;      // memory pointer 
    char* next;        // current position or bump
    unsigned int allocation_count = 0;  // no. of allocations
    size_t total_size = 0;  // memory needed
```

`next` acts as a `bump` .


1. Initialisation
```cpp
    bump_allocator(std::size_t capacity) {
    memory = (capacity > 0) ? new char[capacity] : nullptr;
    total_size = capacity;
    next = memory;
}
```

Allocates a block of memory, starts with 12 bytes. Just a default. 


## Alloc() 
1. Alignment

```
constexpr std::size_t align = alignof(T);
std::size_t padding = (reinterpret_cast<std::uintptr_t>(next) % align);
if (padding > 0) {
    padding = align - padding;
}
```

This will calculate the padding required for type T. It also determines how far the current pointer is from the next aligned address. 

2. Size 

```
std::size_t required_size = padding + (num * sizeof(T));

if (memory == nullptr || (next + required_size > memory + total_size)) {
    return nullptr; // Not enough memory
}
```
Calculates teh size and if there's not enough memory left, returns a `nullptr` if allocation would exceed its capacity. 

3. Memory Alloc

```
next += padding;
T* result = reinterpret_cast<T*>(next);
next += num * sizeof(T);
allocation_count++;
```
This applies the padding by advancing the pointer and makes the aligned address to the type needed. Then increments the allocation counter. 

### Dealloc()

```cpp
        void dealloc() {
            if (allocation_count > 0) {
                allocation_count--;
            }
            if (allocation_count == 0) {
                next = memory; // Reset pointer to start of memory block
                std::cout << "Allocator has reset." << std::endl;
            }
        }

```

Simple implementation, it decrements the allocation counter by 1 if its less than 0. 

When it the allocation acounter reaches 0, it resets the next pointer back to the start of memory block. 

