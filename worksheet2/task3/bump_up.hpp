#ifndef BUMP_ALLOCATOR_H
#define BUMP_ALLOCATOR_H

#include <cstddef>
#include <iostream>

class bump_allocator {
    private:
        char* memory;
        char* next;
        unsigned int allocation_count = 0;
        size_t total_size = 0;

    public:
    //  initialise memory block with given capacity, choose 12 as default but doesnt really matter
        bump_allocator() : bump_allocator(12) {}

    // if the capacity is greater than zero, memory is allocated to a new block of memory (new char[capacity]). 
    // On the other hand, if the capacity is not greater than zero (for instance, if it’s zero), 
    // memory is set to nullptr, indicating that no memory was allocated.
        bump_allocator(std::size_t capacity) {
            memory = (capacity > 0) ? new char[capacity] : nullptr;
            total_size = capacity;
            next = memory;
        }   

        // destructor 
        ~bump_allocator() {
            delete[] memory;
        }

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

        // just decrements allocation count and resets if zero
        void dealloc() {
            if (allocation_count > 0) {
                allocation_count--;
            }
            if (allocation_count == 0) {
                next = memory; // reset pointer to start of memory block
                std::cout << "Allocator has reset." << std::endl;
            }
        }
};

#endif
