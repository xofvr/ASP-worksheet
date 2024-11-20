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
            // alignment of T 
            constexpr std::size_t align = alignof(T);
            
            // padding to align
            std::size_t padding = (reinterpret_cast<std::uintptr_t>(next) % align);
            if (padding > 0) {
                padding = align - padding;
            }
            
            // total required size including padding
            std::size_t required_size = padding + (num * sizeof(T));

            // to see if there's enough capacity left
            if (memory == nullptr || (next + required_size > memory + total_size)) {
                return nullptr; // returns nullptr if no capacity left
            }
            
            next += padding;

            // allocates the memory and applies the set padding
            T* result = reinterpret_cast<T*>(next);
            next += num * sizeof(T);
            allocation_count++;
            std::cout << "Allocated " << num << " objects of size " << sizeof(T) 
                    << " with padding " << padding << " bytes" << std::endl;
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
