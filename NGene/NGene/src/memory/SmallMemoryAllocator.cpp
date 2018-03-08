#include "SmallMemoryAllocator.h"

SmallMemoryAllocator::SmallMemoryAllocator() {
    m_pGeneralPool =
        static_cast<byte*>(malloc(config::sma_pool_size * m_poolArray.size()));
    auto currOrigin = m_pGeneralPool;
    for (auto i = 0; i < m_poolArray.size();
        ++i, currOrigin += config::sma_pool_size) {
        m_poolArray[i] = std::make_unique<Pool>(currOrigin, std::pow(2, i),
            config::sma_pool_size >> i);
    }
}

SmallMemoryAllocator::~SmallMemoryAllocator() { free(m_pGeneralPool); }

Pool* SmallMemoryAllocator::getPoolForSize(size_t size) {
    auto index = getPoolIndex(size);
    if (index >= m_poolArray.size()) {
        return nullptr;
    }
    return m_poolArray[index].get();
}

bool SmallMemoryAllocator::fits(size_t size) const {
    return (getPoolIndex(size) >= m_poolArray.size());
}

void* SmallMemoryAllocator::alloc(size_t size) {
    //
    //  So you wanted 0 bytes?
    //  Here they are :D
    //
    if (size == 0) return nullptr;

    auto index = getPoolIndex(size);
    if (index >= m_poolArray.size()) {
        //
        //  It doesn't fit in any of our pools
        //
        return malloc(size);
    }

    auto ptr = m_poolArray[index].get()->alloc();
    if (ptr) {
        //
        //  The element is in one of our pools
        //
        return ptr;
    }
    else {
        //
        //  The pool couldn't allocate the element (was full)
        //
        return malloc(size);
    }
}

void SmallMemoryAllocator::dealloc(void* elem) {
    for (auto& pool : m_poolArray) {
        if (pool.get()->couldBeInPool((byte*)elem)) {
            //
            //  The element should be in this pool
            //
            pool.get()->free((byte*)elem);
            return;
        }
    }

    //
    //  The element wasn't in any pool so it should be freed normally
    //  since it was allocated with "malloc".
    //
    free(elem);
}

void SmallMemoryAllocator::printStatus(void* elem) const {
    for (auto& pool : m_poolArray) {
        if (pool.get()->couldBeInPool((byte*)elem)) {
            std::cout << "Address " << elem
                << " would fit in pool with a block size of "
                << pool.get()->blockSize() << " which is currently ";
            if (pool.get()->isBlockUsed((byte*)elem)) {
                std::cout << "IN USE";
            }
            else {
                std::cout << "NOT IN USE";
            }
            std::cout << std::endl;
            return;
        }
    }
    std::cout << "Address " << elem
        << " can't be in any of our pools so it might be managed with "
        "normal malloc/free dynamic allocation"
        << std::endl;
}
