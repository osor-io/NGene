#pragma once

#include <cstddef>
#include <memory>
#include <iostream>
#include <array>
#include "../utils/CRSP.h"
#include "./Pool.h"
#include "../math/OtherMath.h"
#include "../types/def.h"


namespace config {
	/**
	Default pool size for our Small Memory Allocator pools.
	*/
	constexpr size_t sma_pool_size = 1024 * 1024;
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
	Pool * getPoolForSize(size_t size);

	bool fits(size_t size) const;

	void* alloc(size_t size);

	template <typename T>
	T* alloc() {
		return reinterpret_cast<T*>(alloc(sizeof(T)));
	}

	void dealloc(void* elem);

	void printStatus(void* elem) const;

private:
	size_t getPoolIndex(size_t size) const {
		return static_cast<size_t>(std::log2(static_cast<double>(nextPowerOf2(static_cast<size_t>(size)))));
	}

	std::array<std::unique_ptr<Pool>, 10> m_poolArray{};
	byte* m_pGeneralPool{ nullptr };
};

