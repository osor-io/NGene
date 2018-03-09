#pragma once

#include "../containers/Stack.h"
#include <cstddef>
#include "../types/def.h"

/**
Represents one of our memory pools for a particular block size.
*/
class Pool {
public:
    Pool(byte* origin, const size_t& blockSize, const size_t& blockAmount);
    ~Pool();

    bool allFreed() const;

    size_t usedBlocks() const;

    size_t freeBlocks() const;

    bool couldBeInPool(byte* elem) const;

    byte* getOrigin() const;

    byte* getEnd() const;

    size_t getMaxElements() const;

    size_t blockSize() const;

    byte* alloc();

    /**
    @NOTE: Expensive function, only for debugging purposes
    */
    bool isBlockUsed(byte* elem) const;

    void free(byte* elem);

    void printNextPossibleElements(size_t count) const;

    void* operator new(std::size_t count);

    void operator delete(void* ptr);

private:
    Stack<byte*> m_freePositions{};
    byte* m_origin{};
    byte* m_last{};
    size_t m_blockSize{};
    size_t m_blockAmount{};
};

