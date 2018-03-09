#pragma once

#include "../utils/CRSP.h"

/**
Base Class for every system in the engine. They should be retrieved
at the initialization phase of the program before the game loop and
call the startUp function in the proper order.

At the end of the program they should be shutDown also in the proper
order. Generally the opposed order to initialization.

*/
template <typename T>
class System : public CRSP<T> {
    friend class CRSP<T>;
protected:
    System() {}
    virtual ~System() {}
public:
    virtual void update() = 0;
    virtual void startUp() = 0;
    virtual void shutDown() = 0;
};
