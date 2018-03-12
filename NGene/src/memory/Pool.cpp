#include "Pool.h"
#include <cstdlib>
#include <iostream>
#include "Debug.h"
#include <gsl\gsl>

Pool::Pool(byte* origin, const size_t& blockSize, const size_t& blockAmount) {
    m_origin = origin;
    m_last = origin + (blockAmount * blockSize);

    m_blockSize = blockSize;
    m_blockAmount = blockAmount;

    /*
    We insert the positions (blocks of size blockSize) in reverse order in the
    stack so we retrieve them in forward order. We could also change it so we
    are using a queue instead of a stack.
    */
    for (auto i = gsl::narrow_cast<int>(blockAmount) - 1; i >= 0; --i) {
        m_freePositions.push(origin + (i * blockSize));
    }
}

Pool::~Pool() {
    if (!allFreed()) {
        std::cerr << "Destroying pool with blocksize " << m_blockSize << " with "
            << usedBlocks() << " blocks currently in use" << std::endl;
    }
}

bool Pool::allFreed() const { return (usedBlocks() == 0); }

size_t Pool::usedBlocks() const { return m_blockAmount - m_freePositions.size(); }

size_t Pool::freeBlocks() const { return m_freePositions.size(); }


bool Pool::couldBeInPool(byte* elem) const {
    return (uintptr_t)(elem) >= (uintptr_t)(m_origin) &&
        (uintptr_t)(elem) < (uintptr_t)(m_last);
}

byte* Pool::getOrigin() const {
    return m_origin;
}

byte* Pool::getEnd() const {
    return m_last - 1;
}

size_t Pool::getMaxElements() const {
    return m_blockAmount;
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