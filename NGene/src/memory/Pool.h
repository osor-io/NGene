#pragma once

#include "../containers/Stack.h"
#include <cstddef>
#include "../types/def.h"

/**
Represents one of our memory pools for a particular block size.
*/
class Pool {
public:
    Pool(byte* origin, const size_t& blocksize, const size_t& blockAmount);
    ~Pool();

    bool all_freed() const;

    size_t used_blocks() const;

    size_t free_blocks() const;

    bool could_be_in_pool(byte* elem) const;

    byte* get_origin() const;

    byte* get_end() const;

    size_t get_max_elements() const;

    size_t blocksize() const;

    byte* alloc();

    /**
    @NOTE: Expensive function, only for debugging purposes
    */
    bool is_block_used(byte* elem) const;

    void free(byte* elem);

    void print_next_possible_elements(size_t count) const;

    void* operator new(std::size_t count);

    void operator delete(void* ptr);

private:
    Stack<byte*> m_free_positions{};
    byte* m_origin{};
    byte* m_last{};
    size_t m_blocksize{};
    size_t m_block_amount{};
};

