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
    // Constructor to initialize memory block with given capacity (default 30)
        bump_allocator() : bump_allocator(12) {}

    // If the capacity is greater than zero, memory is allocated to a new block of memory (new char[capacity]). On the other hand, if the capacity is not greater than zero (for instance, if it’s zero), memory is set to nullptr, indicating that no memory was allocated.
        bump_allocator(std::size_t capacity) {
            memory = (capacity > 0) ? new char[capacity] : nullptr;
            total_size = capacity;
            next = memory;
        }   

        // Destructor to release memory block
        ~bump_allocator() {
            delete[] memory;
        }


        template <typename T>
        T* alloc(std::size_t num = 1) {
            // Get alignment requirement for type T
            constexpr std::size_t align = alignof(T);
            
            // Calculate required padding to align properly
            std::size_t padding = (reinterpret_cast<std::uintptr_t>(next) % align);
            if (padding > 0) {
                padding = align - padding;
            }
            
            // Calculate total required size including padding
            std::size_t required_size = padding + (num * sizeof(T));

            // Check if there's enough capacity left
            if (memory == nullptr || (next + required_size > memory + total_size)) {
                return nullptr; // Not enough memory
            }
            // Apply padding
            next += padding;

            // Allocate memory and update the bump pointer
            T* result = reinterpret_cast<T*>(next);
            next += num * sizeof(T);
            allocation_count++;
            std::cout << "Allocated " << num << " objects of size " << sizeof(T) 
                    << " with padding " << padding << " bytes" << std::endl;
            return result;
            }

        // Dealloc function: decrements allocation count and resets if zero
        void dealloc() {
            if (allocation_count > 0) {
                allocation_count--;
            }
            if (allocation_count == 0) {
                next = memory; // Reset pointer to start of memory block
                std::cout << "Allocator has reset." << std::endl;
            }
        }
};

#endif
