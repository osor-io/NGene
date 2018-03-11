#pragma once

#include <CRSP.h>

/**
Base Class for every manager in the engine. They should be retrieved
at the initialization phase of the program before the game loop and
call the startUp function in the proper order.

At the end of the program they should be shutDown also in the proper
order. Generally the opposed order to initialization.

*/
template <typename T>
class Manager : public CRSP<T> {
    friend class CRSP<T>;
protected:
    Manager() {}
    virtual ~Manager() {}
public:
    virtual void startUp() = 0;
    virtual void shutDown() = 0;
};