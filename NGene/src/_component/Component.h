#pragma once
#include <typeindex>
#include <string>
#include <Debug.h>
#include "../_entity/Entity.h"
#include "../memory/SmallMemoryAllocator.h"


class Component {
public:
    Component(EntityId id, std::type_index type);
    virtual ~Component();

    void* operator new(size_t count) {
        return SmallMemoryAllocator::get().alloc(count);
    }

    void operator delete(void* ptr) {
        return SmallMemoryAllocator::get().dealloc(ptr);
    }

    virtual void drawDebugGUI() = 0;

protected:
    bool m_guiOpen{ false };
    EntityId m_parentId;
private:
    std::type_index m_type;
};




