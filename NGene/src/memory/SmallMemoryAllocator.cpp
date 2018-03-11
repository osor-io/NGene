#include "SmallMemoryAllocator.h"
#include <cassert>
#include "../utils/OtherUtils.h"
#include <sstream>
#include <File.h>


SmallMemoryAllocator::SmallMemoryAllocator() {
    m_pGeneralPool =
        static_cast<byte*>(malloc(config::sma_pool_size * m_poolArray.size()));
    auto currOrigin = m_pGeneralPool;
    for (auto i = 0; i < m_poolArray.size();
        ++i, currOrigin += config::sma_pool_size) {
        m_poolArray[i] = std::make_unique<Pool>(
            currOrigin,
            gsl::narrow<size_t>(std::pow(2, i)),
            config::sma_pool_size >> i);
    }
}


SmallMemoryAllocator::~SmallMemoryAllocator() {
    {
        auto ss = std::stringstream{};
        printStatus(ss);
        writeToFile("memory.log", ss.str().c_str());
    }
    free(m_pGeneralPool);
}

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
        ++m_extraRequestedElements[config::sma_pool_amount];
        return malloc(size);
    }

    auto ptr = m_poolArray[index].get()->alloc();
    if (ptr) {
        //
        //  The element is in one of our pools
        //

        if (m_poolArray[index].get()->usedBlocks() > m_maxAllocatedElements[index]) {
            m_maxAllocatedElements[index] = m_poolArray[index].get()->usedBlocks();
        }
        return ptr;
    }
    else {
        ++m_extraRequestedElements[index];
        //
        //  The pool couldn't allocate the element (was full)
        //
        return malloc(size);
    }
}

void SmallMemoryAllocator::dealloc(void* elem) {
    for (auto i = 0; i < m_poolArray.size(); ++i) {
        auto& pool = m_poolArray[i];
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

void SmallMemoryAllocator::printStatus(std::ostream& stream) {

    for (auto i = 0; i < m_poolArray.size(); ++i) {
        const auto& pool = m_poolArray[i].get();
        stream << "Pool with chunk size [" << pool->blockSize() <<
            "] ranging from [" << static_cast<void*>(pool->getOrigin()) <<
            "] to [" << static_cast<void*>(pool->getEnd()) << "]" << '\n';

        stream << "\t" << "It can hold up to [" <<
            pool->getMaxElements() << "] and [" <<
            m_extraRequestedElements[i] + m_maxAllocatedElements[i] <<
            "] have been required, meaning that we needed [" <<
            (m_extraRequestedElements[i] > 0 ? m_extraRequestedElements[i] : 0) << "] extra elements" << '\n';

        stream << '\n';
    }

    if (m_extraRequestedElements[config::sma_pool_amount] > 0) {
        stream << "We had to allocate with the default malloc [" << m_extraRequestedElements[config::sma_pool_amount] <<
            "] times because they were larger than our biggest pool (" << m_poolArray[m_poolArray.size() - 1]->getMaxElements() << ")." << '\n';
        stream << "We know that at least 4 of these big allocations are to hold the stream and string to write this exact file so don't worry about those" << '\n';
    }
    else {
        stream << "We didn't have to allocate anything with the default malloc" << '\n';
    }

    stream << std::endl;

}

void SmallMemoryAllocator::printStatus(void* elem, std::ostream& stream) const {
    for (auto& pool : m_poolArray) {
        if (pool.get()->couldBeInPool((byte*)elem)) {
            stream << "Address " << elem
                << " would fit in pool with a block size of "
                << pool.get()->blockSize() << " which is currently ";
            if (pool.get()->isBlockUsed((byte*)elem)) {
                stream << "IN USE";
            }
            else {
                stream << "NOT IN USE";
            }
            stream << '\n';
            return;
        }
    }
    stream << "Address " << elem
        << " can't be in any of our pools so it might be managed with "
        "normal malloc/free dynamic allocation"
        << std::endl;
}

size_t SmallMemoryAllocator::getPoolIndex(size_t size) const {
    return gsl::narrow_cast<size_t>(std::log2(static_cast<double>(nextPowerOf2(static_cast<unsigned int>(size)))));
}
