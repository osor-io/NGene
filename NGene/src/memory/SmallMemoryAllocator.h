#pragma once

#include <cstddef>
#include <memory>
#include <iostream>
#include <array>
#include <gsl\gsl>
#include "../utils/CRSP.h"
#include "./Pool.h"
#include "../math/OtherMath.h"
#include "../types/def.h"


namespace config {
    /**
    Default pool size for our Small Memory Allocator pools.
    */
    constexpr size_t sma_pool_size = 1024 * 1024;
	constexpr size_t sma_pool_amount = 10;
}  // namespace config


/**
Our allocator class that will be the general interface for any future
allocation. Inherits from the Curiously Recurring Singleton Pattern in order to 
have just one global one.

@see CRSP
*/
class SmallMemoryAllocator : public CRSP<SmallMemoryAllocator> {
private:
    friend class CRSP<SmallMemoryAllocator>;

    SmallMemoryAllocator();
    ~SmallMemoryAllocator();

public:
    Pool * get_pool_for_size(size_t size);

    bool fits(size_t size) const;

    void* alloc(size_t size);

    template <typename T>
    T* alloc() {
        return reinterpret_cast<T*>(alloc(sizeof(T)));
    }

    void dealloc(void* elem);

    void print_status(std::ostream& stream);

    void print_status(void* elem, std::ostream& stream) const;

private:
	size_t get_pool_index(size_t size) const;

    std::array<std::unique_ptr<Pool>, config::sma_pool_amount> m_pool_array{};
	std::array<size_t, config::sma_pool_amount + 1> m_extra_requested_elements{};
    std::array<size_t, config::sma_pool_amount> m_max_allocated_elements{};


    byte* m_general_pool{ nullptr };
};

