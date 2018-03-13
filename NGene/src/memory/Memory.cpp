#include "Memory.h"
#include "./SmallMemoryAllocator.h"
#include <Debug.h>


/*
void* operator new(std::size_t count) {
    return SmallMemoryAllocator::get().alloc(count);
}

void operator delete(void* ptr) {
    SmallMemoryAllocator::get().dealloc(ptr);
}
*/

