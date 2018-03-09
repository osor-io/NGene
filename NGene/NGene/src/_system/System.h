#pragma once

#include "../utils/CRSP.h"

template <typename T>
class System : public CRSP<T> {
public:
    System() {}
    virtual ~System() {}

    /*
    @@TODO: Check that when we call get() we are returning
    an inicialized system.
    */

    virtual void startUp() = 0;
    virtual void shutDown() = 0;
};
