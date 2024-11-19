#ifndef BUMP_DOWN_ALLOCATOR_H
#define BUMP_DOWN_ALLOCATOR_H

#include <cstddef>
#include <iostream>

class bump_allocator {
    private:
        char* memory;
        char* next;
        unsigned int allocation_count = 0;
        size_t total_size = 0;

    public:
        bump_allocator() : bump_allocator(12) {}

        bump_allocator(std::size_t capacity) {
            memory = (capacity > 0) ? new char[capacity] : nullptr;
            total_size = capacity;
            next = memory + capacity; // Start at end of memory block
        }

        ~bump_allocator() {
            delete[] memory;
        }

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
            
            // Return aligned pointer cast to requested type
            return reinterpret_cast<T*>(next);
        }

        void dealloc() {
            if (allocation_count > 0) {
                allocation_count--;
            }
            if (allocation_count == 0) {
                next = memory + total_size; // Reset pointer to end of memory block
                std::cout << "Allocator has reset." << std::endl;
            }
        }
};

#endif