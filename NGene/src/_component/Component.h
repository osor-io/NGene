#pragma once
#include <typeindex>
#include <string>
#include <Debug.h>
#include "../_entity/Entity.h"
#include "../memory/SmallMemoryAllocator.h"
#include "json.hpp"


/**

This abstract class represents every component in the engine.
All of the components will inherit from this. It requires the
implementation of general fuctionality that all components should
have in order to behave correctly during gameplay and development.

This includes serialization, development-mode inspection, etc.

The components in this engine can only be owned by one entity and each
represent a characteristic or behaviour requirements of said entity.

*/
class Component {
public:
    Component(EntityId id, std::type_index type);
    virtual ~Component();

    /**
    
    We override the new and delete operators to use our custom
    memory allocation for all of the components of the game.

    */

    void* operator new(size_t count) {
        return SmallMemoryAllocator::get().alloc(count);
    }

    void operator delete(void* ptr) {
        return SmallMemoryAllocator::get().dealloc(ptr);
    }

    /**
    
    It draws the inspector of this component.

    We require every component to be able to be drawn in developer
    /edit mode so we make iteration time faster and development easier.
    
    */
    virtual void draw_debug_gui() = 0;

    /**
    
    It serializes the component to a json object that can then
    be written to a file.
    
    */
    virtual nlohmann::json to_json() = 0;

    /**
    
    Loads the data from a json object into the component. The object
    should have been generated from the "to_json" function above.

    @see to_json
    
    */
    virtual void load_json(const nlohmann::json& j) = 0;

    /**
    
    Returns the name of the type of the component so it can be easily
    identified in developer mode.
    
    */
    virtual std::string get_component_type_name() = 0;

protected:
    
    /**
    
    It stores wether the component should be shown in the developer mode GUI
    or not.

    */
    bool m_gui_open{ false };

    /**
    
    It stores the ID of the entity that contains this particular component.
    
    */
    EntityId m_parent_id;

private:

    /**
    
    It stores the real underlying type of the concrete instantiated component.
    
    */
    std::type_index m_type;
};




