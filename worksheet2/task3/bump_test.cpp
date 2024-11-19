#include "bump.hpp"
#include "simpletest/simpletest.h"
#include <string>

char const * groups[] = {"Bump"};

// Test basic allocation
DEFINE_TEST_G(BasicAllocation, Bump) {
    bump_allocator alloc(100);
    int* x = alloc.alloc<int>();
    TEST_MESSAGE(x != nullptr, "Failed to allocate single int");
}

// Test multiple allocations
DEFINE_TEST_G(MultipleAllocations, Bump) {
    bump_allocator alloc(100);
    int* x = alloc.alloc<int>(5);
    double* y = alloc.alloc<double>(3);
    TEST_MESSAGE(x != nullptr, "Failed to allocate ints");
    TEST_MESSAGE(y != nullptr, "Failed to allocate doubles");
}

// Test allocation failure
DEFINE_TEST_G(AllocationFailure, Bump) {
    bump_allocator alloc(10);
    int* x = alloc.alloc<int>(5);  // Should fail as not enough space
    TEST_MESSAGE(x == nullptr, "Should have failed to allocate");
}

// Test alignment
DEFINE_TEST_G(AlignmentTest, Bump) {
    bump_allocator alloc(100);
    char* c = alloc.alloc<char>();
    double* d = alloc.alloc<double>();
    TEST_MESSAGE(d != nullptr, "Failed to allocate aligned double");
    TEST_MESSAGE((reinterpret_cast<std::uintptr_t>(d) % alignof(double)) == 0, 
                 "Double not properly aligned");
}

// Test deallocation reset
DEFINE_TEST_G(DeallocationTest, Bump) {
    bump_allocator alloc(50);
    int* x = alloc.alloc<int>(5);
    alloc.dealloc();
    int* y = alloc.alloc<int>(5);
    TEST_MESSAGE(x != nullptr && y != nullptr, "Deallocation reset failed");
}

int main() {
    bool pass = true;
    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }
    return pass ? 0 : 1;
}
