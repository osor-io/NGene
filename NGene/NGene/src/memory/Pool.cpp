#include "Pool.h"
#include <cstdlib>
#include <iostream>

Pool::Pool(byte* origin, const size_t& blockSize, const size_t& blockAmount) {
    m_origin = origin;
    m_last = origin + (blockAmount * blockSize);

    /*
    We may be able to remove these two members.
    */
    m_blockSize = blockSize;
    m_blockAmount = blockAmount;

    for (size_t i = 0; i < blockAmount; ++i) {
        m_freePositions.push(origin + (i * blockSize));
    }
}

Pool::~Pool() {
    if (!allFreed()) {
        std::cout << "Destroying pool with blocksize " << m_blockSize << " with "
            << usedBlocks() << " blocks currently in use" << std::endl;
    }
}

bool Pool::allFreed() const { return (usedBlocks() == 0); }

size_t Pool::usedBlocks() const { return m_blockAmount - m_freePositions.size(); }

bool Pool::couldBeInPool(byte* elem) const {
    return (uintptr_t)(elem) >= (uintptr_t)(m_origin) &&
        (uintptr_t)(elem) < (uintptr_t)(m_last);
}

size_t Pool::blockSize() const { return m_blockSize; }

byte* Pool::alloc() {
    if (m_freePositions.size() == 0) return nullptr;
    return m_freePositions.pop();
}

/**
@NOTE: Expensive function, only for debugging purposes
*/
bool Pool::isBlockUsed(byte* elem) const {
    if (!couldBeInPool(elem)) {
        return false;
    }
    return !(m_freePositions.hasElement(elem));
}

void Pool::free(byte* elem) {
    if (couldBeInPool(elem)) {
        m_freePositions.push(elem);
    }
    else {
        std::cerr << "Trying to free an invalid memory allocation for this pool"
            << std::endl;
        std::terminate();
    }
}

void Pool::printNextPossibleElements(size_t count) const {
    this->m_freePositions.printTop(count);
}

void* Pool::operator new(std::size_t count) { return ::malloc(count); }

void Pool::operator delete(void* ptr) { ::free(ptr); }