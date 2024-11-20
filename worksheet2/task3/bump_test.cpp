#include "bump_up.hpp"
#include "simpletest/simpletest.h"
#include <string>

char const * groups[] = {"Bump"};

// will test basic allocation
DEFINE_TEST_G(BasicAllocation, Bump) {
    bump_allocator alloc(100);
    int* x = alloc.alloc<int>();
    TEST_MESSAGE(x != nullptr, "Doesn't allocate single int");
}

// will test multiple allocations
DEFINE_TEST_G(MultipleAllocations, Bump) {
    bump_allocator alloc(100);
    int* x = alloc.alloc<int>(5);
    double* y = alloc.alloc<double>(3);
    TEST_MESSAGE(x != nullptr, "Fails to allocate ints");
    TEST_MESSAGE(y != nullptr, "Fails to allocate doubles");
}

// will test allocation failure
DEFINE_TEST_G(AllocationFailure, Bump) {
    bump_allocator alloc(10);
    int* x = alloc.alloc<int>(5);  
    TEST_MESSAGE(x == nullptr, "Should've failed to allocate");
}

// will test alignment
DEFINE_TEST_G(AlignmentTest, Bump) {
    bump_allocator alloc(100);
    char* c = alloc.alloc<char>();
    double* d = alloc.alloc<double>();
    TEST_MESSAGE(d != nullptr, "Fails to allocate aligned double");
    TEST_MESSAGE((reinterpret_cast<std::uintptr_t>(d) % alignof(double)) == 0, 
                 "Double has not been properly aligned");
}

// will test deallocation reset
DEFINE_TEST_G(DeallocationReset, Bump) {
    bump_allocator alloc(100);
    int* first = alloc.alloc<int>();
    alloc.dealloc();
    int* second = alloc.alloc<int>();
    TEST_MESSAGE(first == second, "Memory should have reset to same address after deallocation");
}

// should test exact boundary allocation
DEFINE_TEST_G(BoundaryAllocation, Bump) {
    bump_allocator alloc(sizeof(int));
    int* x = alloc.alloc<int>();
    TEST_MESSAGE(x != nullptr, "Should succeed with exact size");
    int* y = alloc.alloc<int>();
    TEST_MESSAGE(y == nullptr, "Should fail when full");
}

// see if multiple deallocations reset
DEFINE_TEST_G(MultipleDeallocations, Bump) {
    bump_allocator alloc(100);
    int* x = alloc.alloc<int>();
    double* y = alloc.alloc<double>();
    alloc.dealloc();  // Decrement count
    alloc.dealloc();  // Should reset
    int* z = alloc.alloc<int>();
    TEST_MESSAGE(x == z, "Should return to initial position after full reset");
}

int main() {
    bool pass = true;
    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }
    return pass ? 0 : 1;
}
