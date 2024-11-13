#include <iostream>
#include <cassert>
#include "bump.hpp"

struct SmallStruct {
    char a;
    int b;
};

struct BigStruct {
    double a;
    double b;
    double c;
};

int main() {
    // Test 1: Basic allocation
    {
        bump_allocator alloc(32);
        int* ptr = alloc.alloc<int>();
        assert(ptr != nullptr);
        *ptr = 42;
        assert(*ptr == 42);
        std::cout << "Test 1 passed: Basic allocation works\n";
    }

    // Test 2: Multiple allocations
    {
        bump_allocator alloc(64);
        int* int_ptr = alloc.alloc<int>();
        double* double_ptr = alloc.alloc<double>();
        char* char_ptr = alloc.alloc<char>();
        
        assert(int_ptr != nullptr);
        assert(double_ptr != nullptr);
        assert(char_ptr != nullptr);
        
        *int_ptr = 1;
        *double_ptr = 2.0;
        *char_ptr = '3';
        
        std::cout << "Test 2 passed: Multiple allocations work\n";
    }

    // Test 3: Allocation failure
    {
        bump_allocator alloc(8);  // Small allocator
        double* ptr1 = alloc.alloc<double>();
        double* ptr2 = alloc.alloc<double>();  // Should fail
        
        assert(ptr1 != nullptr);
        assert(ptr2 == nullptr);
        
        std::cout << "Test 3 passed: Allocation failure handled correctly\n";
    }

    // Test 4: Deallocation reset
    {
        bump_allocator alloc(32);
        
        int* ptr1 = alloc.alloc<int>();
        int* ptr2 = alloc.alloc<int>();
        
        assert(ptr1 != nullptr && ptr2 != nullptr);
        
        alloc.dealloc();  // First deallocation
        alloc.dealloc();  // Second deallocation - should reset
        
        int* ptr3 = alloc.alloc<int>();
        assert(ptr3 != nullptr);
        assert(ptr3 == ptr1);  // Should reuse the first position
        
        std::cout << "Test 4 passed: Deallocation reset works\n";
    }

    // Test 5: Mixed-size allocations
    {
        bump_allocator alloc(64);
        
        SmallStruct* small = alloc.alloc<SmallStruct>();
        BigStruct* big = alloc.alloc<BigStruct>();
        
        assert(small != nullptr);
        assert(big != nullptr);
        
        small->a = 'A';
        small->b = 123;
        
        big->a = 1.0;
        big->b = 2.0;
        big->c = 3.0;
        
        std::cout << "Test 5 passed: Mixed-size allocations work\n";
    }

    std::cout << "All tests passed!\n";
    return 0;
}