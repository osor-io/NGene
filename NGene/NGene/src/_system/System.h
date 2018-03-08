#pragma once

#include "../utils/CRSP.h"

template <typename T>
class System : public CRSP<T> {
public:
	System() {}
	virtual ~System() {}

	virtual void startUp() = 0;
	virtual void shutDown() = 0;
};
