#include "SmallMemoryAllocator.h"
#include <cassert>
#include "Debug.h"
#include <sstream>
#include <File.h>


SmallMemoryAllocator::SmallMemoryAllocator() {
    m_general_pool =
        static_cast<byte*>(malloc(config::sma_pool_size * m_pool_array.size()));
    auto currOrigin = m_general_pool;
    for (auto i = 0; i < m_pool_array.size();
        ++i, currOrigin += config::sma_pool_size) {
        m_pool_array[i] = std::make_unique<Pool>(
            currOrigin,
            gsl::narrow<size_t>(std::pow(2, i)),
            config::sma_pool_size >> i);
    }
}


SmallMemoryAllocator::~SmallMemoryAllocator() {
    {
        auto ss = std::stringstream{};
        print_status(ss);
        write_to_file("memory.log", ss.str().c_str());
    }
    free(m_general_pool);
}

Pool* SmallMemoryAllocator::get_pool_for_size(size_t size) {
    auto index = get_pool_index(size);
    if (index >= m_pool_array.size()) {
        return nullptr;
    }
    return m_pool_array[index].get();
}

bool SmallMemoryAllocator::fits(size_t size) const {
    return (get_pool_index(size) >= m_pool_array.size());
}

void* SmallMemoryAllocator::alloc(size_t size) {
    //
    //  So you wanted 0 bytes?
    //  Here they are :D
    //
    if (size == 0) return nullptr;

    auto index = get_pool_index(size);
    if (index >= m_pool_array.size()) {
        //
        //  It doesn't fit in any of our pools
        //
        ++m_extra_requested_elements[config::sma_pool_amount];
        return malloc(size);
    }

    auto ptr = m_pool_array[index].get()->alloc();
    if (ptr) {
        //
        //  The element is in one of our pools
        //

        if (m_pool_array[index].get()->used_blocks() > m_max_allocated_elements[index]) {
            m_max_allocated_elements[index] = m_pool_array[index].get()->used_blocks();
        }
        return ptr;
    }
    else {
        ++m_extra_requested_elements[index];
        //
        //  The pool couldn't allocate the element (was full)
        //
        return malloc(size);
    }
}

void SmallMemoryAllocator::dealloc(void* elem) {
    for (auto i = 0; i < m_pool_array.size(); ++i) {
        auto& pool = m_pool_array[i];
        if (pool.get()->could_be_in_pool((byte*)elem)) {
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

void SmallMemoryAllocator::print_status(std::ostream& stream) {

    for (auto i = 0; i < m_pool_array.size(); ++i) {
        const auto& pool = m_pool_array[i].get();
        stream << "Pool with chunk size [" << pool->blocksize() <<
            "] ranging from [" << static_cast<void*>(pool->get_origin()) <<
            "] to [" << static_cast<void*>(pool->get_end()) << "]" << '\n';

        stream << "\t" << "It can hold up to [" <<
            pool->get_max_elements() << "] and [" <<
            m_extra_requested_elements[i] + m_max_allocated_elements[i] <<
            "] have been required, meaning that we needed [" <<
            (m_extra_requested_elements[i] > 0 ? m_extra_requested_elements[i] : 0) << "] extra elements" << '\n';

        stream << '\n';
    }

    if (m_extra_requested_elements[config::sma_pool_amount] > 0) {
        stream << "We had to allocate with the default malloc [" << m_extra_requested_elements[config::sma_pool_amount] <<
            "] times because they were larger than our biggest pool (" << m_pool_array[m_pool_array.size() - 1]->get_max_elements() << ")." << '\n';
        stream << "We know that at least 4 of these big allocations are to hold the stream and string to write this exact file so don't worry about those" << '\n';
    }
    else {
        stream << "We didn't have to allocate anything with the default malloc" << '\n';
    }

    stream << std::endl;

}

void SmallMemoryAllocator::print_status(void* elem, std::ostream& stream) const {
    for (auto& pool : m_pool_array) {
        if (pool.get()->could_be_in_pool((byte*)elem)) {
            stream << "Address " << elem
                << " would fit in pool with a block size of "
                << pool.get()->blocksize() << " which is currently ";
            if (pool.get()->is_block_used((byte*)elem)) {
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

size_t SmallMemoryAllocator::get_pool_index(size_t size) const {
    return gsl::narrow_cast<size_t>(std::log2(static_cast<double>(next_power_of_2(static_cast<unsigned int>(size)))));
}
