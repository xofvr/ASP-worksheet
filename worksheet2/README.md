# Bump Allocator Implementation

"clang++ -std=c++17 simpletest/simpletest.cpp bump_test.cpp -o bump_test"

"Check if the bump up implementation is right"
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

```cpp
constexpr std::size_t align = alignof(T);
std::size_t padding = (reinterpret_cast<std::uintptr_t>(next) % align);
if (padding > 0) {
    padding = align - padding;
}
```

This will calculate the padding required for type T. It also determines how far; the current pointer is from the next aligned address. 

2. Size 

```cpp
std::size_t required_size = padding + (num * sizeof(T));

if (memory == nullptr || (next + required_size > memory + total_size)) {
    return nullptr; // Not enough memory
}
```
Calculates teh size and if there's not enough memory left, returns a `nullptr` if/ allocation would exceed its capacity. 

3. Memory Alloc

```cpp
next += padding;
T* result = reinterpret_cast<T*>(next);
next += num * sizeof(T);
allocation_count++;
```
This applies the padding by advancing the pointer and makes the aligned address to the type needed. Then increments the allocation counter. 

```cpp
    template <typename T>
    T* alloc(std::size_t num = 1) {
        if (num == 0) return nullptr;
        
        // Ensure proper alignment
        std::uintptr_t current = reinterpret_cast<std::uintptr_t>(next);
        std::uintptr_t aligned = (current + alignof(T) - 1) & ~(alignof(T) - 1);
        std::size_t padding = aligned - current;
        
        // Check for overflow
        if (num > (std::numeric_limits<std::size_t>::max() - padding) / sizeof(T)) {
            return nullptr;
        }
        
        std::size_t required_size = padding + (num * sizeof(T));
        
        // Check if we have enough space
        if (memory == nullptr || 
            next + required_size > memory + total_size || 
            next + required_size < next) { // overflow check
            return nullptr;
        }
        
        next += padding;
        T* result = reinterpret_cast<T*>(next);
        next += num * sizeof(T);
        allocation_count++;
        
        return result;
    }
```

### Dealloc()

```cpp
        void dealloc() {
            if (allocation_count > 0) {
                allocation_count--;
            }
            if (allocation_count == 0) {
                next = memory; // reset pointer to start of memory block
                std::cout << "Allocator has reset." << std::endl;
            }
        }
```

Simple implementation, it decrements the allocation counter by 1 if its less than 0. 

When it the allocation acounter reaches 0, it resets the next pointer back to the start of/ memory block. 

# Task 2

The test implementation uses the simpletest framework with several test cases that verify the bump allocator functionality:

### Basic Allocation Test
Tests if the allocator can handle a simple single integer allocation:

```cpp
DEFINE_TEST_G(BasicAllocation, Bump) {
    bump_allocator alloc(100);
    int* x = alloc.alloc<int>();
    TEST_MESSAGE(x != nullptr, "Doesn't allocate single int");
}
```

* Tests simple allocation of a single integer
* Creates allocator with 100 bytes capacity
* Verifies the allocation succeeds (returns non-null pointer)

### Multiple Allocations Test
Verifies that multiple allocations of different types work correctly:

```cpp
DEFINE_TEST_G(MultipleAllocations, Bump) {
    bump_allocator alloc(100);
    int* x = alloc.alloc<int>(5);
    double* y = alloc.alloc<double>(3);
    TEST_MESSAGE(x != nullptr, "Fails to allocate ints");
    TEST_MESSAGE(y != nullptr, "Fails to allocate doubles");
}
```

* Tests allocating different data types sequentially
* Allocates array of 5 ints followed by array of 3 doubles
* Verifies both allocations succeed

### Allocation Failure Test
Ensures the allocator returns nullptr when there isn't enough memory:

```cpp
DEFINE_TEST_G(AllocationFailure, Bump) {
    bump_allocator alloc(10);
    int* x = alloc.alloc<int>(5);
    TEST_MESSAGE(x == nullptr, "Should've failed to allocate");
}
```

* Tests behavior when memory is insufficient
* Creates small allocator (10 bytes)
* Attempts large allocation that should fail
* Verifies null pointer is returned

### Memory Alignment Test
Verifies proper memory alignment for different data types:

```cpp
DEFINE_TEST_G(AlignmentTest, Bump) {
    bump_allocator alloc(100);
    char* c; = alloc.alloc<char>();
    double* d = alloc.alloc<double>();
    TEST_MESSAGE((reinterpret_cast<std::uintptr_t>(d) % alignof(double)) == 0, 
                 "Double has not been properly aligned");
}
```
* Tests proper memory alignment for different data types
* Allocates char followed by double
* Verifies double pointer is properly aligned (address divisible by alignof(double))

### Deallocation Reset Test
Tests if memory resets properly after deallocation:

```cpp
DEFINE_TEST_G(DeallocationReset, Bump) {
    bump_allocator alloc(100);
    int* first = alloc.alloc<int>();
    alloc.dealloc();
    int* second = alloc.alloc<int>();
    TEST_MESSAGE(first == second, "Memory should reset to same address");
}
```
* Tests if memory resets after deallocation
* Allocates memory, deallocates it
* Verifies next allocation returns same address


### Boundary Allocation Test
Checks allocation behavior at exact memory boundaries:

```cpp
DEFINE_TEST_G(BoundaryAllocation, Bump) {
    bump_allocator alloc(sizeof(int));
    int* x = alloc.alloc<int>();
    TEST_MESSAGE(x != nullptr, "Should succeed with exact size");
    int* y = alloc.alloc<int>();
    TEST_MESSAGE(y == nullptr, "Should fail when full");
}
```

* Tests allocations at exact capacity limits
* Creates allocator with exactly enough space for one int
* Verifies first allocation succeeds
* Verifies second allocation fails (no space left)

### Multiple Deallocations Test
Verifies correct behavior with multiple deallocation operations:

```cpp
DEFINE_TEST_G(MultipleDeallocations, Bump) {
    bump_allocator alloc(100);
    int* x = alloc.alloc<int>();
    double* y = alloc.alloc<double>();
    alloc.dealloc();  // Decrement count
    alloc.dealloc();  // Should reset
    int* z = alloc.alloc<int>();
    TEST_MESSAGE(x == z, "Should return to initial position after reset");
}
```

* Tests behavior with multiple deallocations
* Makes two allocations
* Deallocates both (should reset to start)
* Verifies new allocation returns to initial position


![Output Task 1](/img/W2Task1.png)

# Task 3

## Benchmark System

In my implementation of the benchmark suite, I have used Variadic templates. 

```cpp
template <typename Func, typename... Args>
static void measure(const std::string& name, Func func, Args&&... args) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto end = high_resolution_clock::now();
    duration<double, std::milli> ms_double = end - start;
    std::cout << name << ": " << ms_double.count() << "ms\n";
}
```

It does multiple runs:

```cpp
BenchmarkResult run_benchmark(const std::string& name, 
                            std::function<void(bump_allocator&)> func, 
                            bump_allocator& allocator,
                            size_t num_runs = 10) {
    std::vector<double> durations;
    durations.reserve(num_runs);
    // ... timing code
}
```

* Each test is executed num_runs times (default 10).
* Duration of each run is recorded in milliseconds.
* Allocator is reset between runs using dealloc().

For each set of runs, Mean duration and Standard Deviation is calculated. 

```cpp
double mean = sum / num_runs;

double sq_sum = 0.0;
for (double d : durations) {
    sq_sum += (d - mean) * (d - mean);
}
double std_dev = std::sqrt(sq_sum / (num_runs - 1));
```
```cpp
void print_results(const std::string& name, const BenchmarkResult& result) {
    std::cout << "\n=== " << name << " ===" << std::endl;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Runs: " << result.num_runs << std::endl;
    std::cout << "Mean time: " << result.mean_duration << " ms" << std::endl;
    std::cout << "Std Dev: ± " << result.std_deviation << " ms" << std::endl;
}
```

It does 3 allocation patterns:

1. Small allocations:

```cpp
void small_allocations(bump_allocator& allocator) {
    for (int i = 0; i < 1000; ++i) {
        allocator.alloc<int>(1);
    }
}
```

2. Large Allocations:

```cpp
void large_allocations(bump_allocator& allocator) {
    for (int i = 0; i < 100; ++i) {
        allocator.alloc<int>(100);
    }
}
```

3. Mixed Allocations

```cpp
void mixed_allocations(bump_allocator& allocator) {
    for (int i = 0; i < 500; ++i) {
        allocator.alloc<int>(i % 10 + 1);
    }
}
```

## Bump down allocator implementation:

The bump down allocator was changed from bumping up to bumping down.

It allocates memory by decrementing the pointer downward from the end of the memory block.

```cpp
        template <typename T>
        T* alloc(std::size_t num = 1) {
            constexpr std::size_t align = alignof(T);
            
            // Calculate required padding to align properly
            std::size_t padding = (reinterpret_cast<std::uintptr_t>(next) % align);
            if (padding > 0) {
                padding = align - padding;
            }
            
            // Calculate total required size including padding
            std::size_t required_size = padding + (num * sizeof(T));

            // Check if there's enough capacity left
            if (memory == nullptr || (next - required_size < memory)) {
                return nullptr; // Not enough memory
            }
            
            // Apply padding by moving down
            next -= padding;
            
            // Move pointer down by required size
            next -= (num * sizeof(T));
            
            // Increment allocation counter
            allocation_count++;  // Add this line
            
            // Return aligned pointer cast to requested type
            return reinterpret_cast<T*>(next);
        }

```

## Benchmark results

I have found bump down allocator to be a tad bit more efficient. 

When allocating downward, checking for overflow is simpler - we just need to verify if we've gone below the base memory address (next - required_size < memory). This eliminates the need for the double overflow check present in the bump-up version:

```cpp
// Bump-up overflow check (more complex):
next + required_size > memory + total_size || next + required_size < next

// Bump-down overflow check (simpler):
next - required_size < memory
```
This makes bump down implementation more efficient. 

Benchmark results for bump up:
![Benchmark Results up](/img/bumpup.png)

Benchmark results for bump down: 
![Benchmark Results down](/img/BumpDown.png)

