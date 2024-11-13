#include "bump.hpp"
#include "/home/mohammad2.shakeel/asp/asp_worksheet/worksheet2/simpletest/simpletest.h"

char const * groups[] = {
    "BumpAllocator",
}; 

DEFINE_TEST_G(testBasicAllocation, BumpAllocator) {
    bump_allocator alloc(100);
    double* d = alloc.alloc<double>(5);
    TEST_MESSAGE(d != nullptr, "Basic allocation failed");
}

DEFINE_TEST_G(testMultipleTypeAllocation, BumpAllocator) {
    bump_allocator alloc(200);
    
    float* f = alloc.alloc<float>(4);
    TEST_MESSAGE(f != nullptr, "Float allocation failed");
    
    long* l = alloc.alloc<long>(3); 
    TEST_MESSAGE(l != nullptr, "Long allocation failed");
    
    bool* b = alloc.alloc<bool>(1);
    TEST_MESSAGE(b != nullptr, "Bool allocation failed");
}

DEFINE_TEST_G(testOutOfMemory, BumpAllocator) {
    bump_allocator alloc(10);
    double* d = alloc.alloc<double>(5);
    TEST_MESSAGE(d == nullptr, "Should fail due to insufficient memory");
}

DEFINE_TEST_G(testAlignment, BumpAllocator) {
    bump_allocator alloc(50);
    
    char* c = alloc.alloc<char>(1);
    double* d = alloc.alloc<double>(1);
    
    TEST_MESSAGE((reinterpret_cast<uintptr_t>(d) % alignof(double)) == 0, 
                 "Double not properly aligned");
}

DEFINE_TEST_G(testResetOnDealloc, BumpAllocator) {
    bump_allocator alloc(50);
    
    float* f1 = alloc.alloc<float>(3);
    float* addr = f1;
    alloc.dealloc();
    
    float* f2 = alloc.alloc<float>(3);
    TEST_MESSAGE(f2 == addr, "Memory not reset to beginning after dealloc");
}

int main() {
    bool pass = true;
    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }
    return pass ? 0 : 1;
}