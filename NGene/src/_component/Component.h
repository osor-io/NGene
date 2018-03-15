#pragma once
#include <typeindex>
#include <string>
#include <Debug.h>
#include "../_entity/Entity.h"
#include "../memory/SmallMemoryAllocator.h"
#include "json.hpp"

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

    virtual void draw_debug_gui() = 0;

    virtual nlohmann::json to_json() = 0;
    virtual void load_json(const nlohmann::json& j) = 0;

    virtual std::string get_component_type_name() = 0;

protected:
    bool m_gui_open{ false };
    EntityId m_parent_id;
private:
    std::type_index m_type;
};




