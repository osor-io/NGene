#include "Pool.h"
#include <cstdlib>
#include <iostream>
#include "Debug.h"
#include <gsl\gsl>

Pool::Pool(byte* origin, const size_t& blocksize, const size_t& blockAmount) {
    m_origin = origin;
    m_last = origin + (blockAmount * blocksize);

    m_blocksize = blocksize;
    m_block_amount = blockAmount;

    /*
    We insert the positions (blocks of size blocksize) in reverse order in the
    stack so we retrieve them in forward order. We could also change it so we
    are using a queue instead of a stack.
    */
    for (auto i = gsl::narrow_cast<int>(blockAmount) - 1; i >= 0; --i) {
        m_free_positions.push(origin + (i * blocksize));
    }
}

Pool::~Pool() {
    if (!all_freed()) {
        std::cerr << "Destroying pool with blocksize " << m_blocksize << " with "
            << used_blocks() << " blocks currently in use" << std::endl;
    }
}

bool Pool::all_freed() const { return (used_blocks() == 0); }

size_t Pool::used_blocks() const { return m_block_amount - m_free_positions.size(); }

size_t Pool::free_blocks() const { return m_free_positions.size(); }


bool Pool::could_be_in_pool(byte* elem) const {
    return (uintptr_t)(elem) >= (uintptr_t)(m_origin) &&
        (uintptr_t)(elem) < (uintptr_t)(m_last);
}

byte* Pool::get_origin() const {
    return m_origin;
}

byte* Pool::get_end() const {
    return m_last - 1;
}

size_t Pool::get_max_elements() const {
    return m_block_amount;
}

size_t Pool::blocksize() const { return m_blocksize; }

byte* Pool::alloc() {
    if (m_free_positions.size() == 0) return nullptr;
    return m_free_positions.pop();
}

/**
@NOTE: Expensive function, only for debugging purposes
*/
bool Pool::is_block_used(byte* elem) const {
    if (!could_be_in_pool(elem)) {
        return false;
    }
    return !(m_free_positions.has_element(elem));
}

void Pool::free(byte* elem) {
    if (could_be_in_pool(elem)) {
        m_free_positions.push(elem);
    }
    else {
        std::cerr << "Trying to free an invalid memory allocation for this pool"
            << std::endl;
        std::terminate();
    }
}

void Pool::print_next_possible_elements(size_t count) const {
    this->m_free_positions.print_top(count);
}

void* Pool::operator new(std::size_t count) { return ::malloc(count); }

void Pool::operator delete(void* ptr) { ::free(ptr); }