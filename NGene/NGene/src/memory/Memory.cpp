#include "Memory.h"
#include "./SmallMemoryAllocator.h"

void* operator new(std::size_t count) {
    auto ptr = SmallMemoryAllocator::get().alloc(count);
    return ptr;
}

void operator delete(void* ptr) {
    SmallMemoryAllocator::get().dealloc(ptr);
}