#pragma once

#include "Component.h"
#include <string>
#include <meta.h>
#include <sol.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

/**
Class that every component in this engine should inherit from. In order to
correctly register everything in a component the usage is as follows:

- Include the required headers in the .h or .hpp

        #include "../ComponentTemplate.h"

- Include the required headers in the .cpp

        #include "../lua/LuaManager.h"

- Declare a friend registerMembers function:

        friend auto meta::registerMembers<SimpleGraphicsComponent>();

- Implement registerName and registerMembers functions:

        template<>
        inline auto meta::registerName<ConcreteComponent>() {
            return "ConcreteComponent";
        }

        template<>
        inline auto meta::registerMembers<ConcreteComponent>() {
            return members(
                member("Some Member", &ConcreteComponent::m_someMember),
                member("Some Other Member", &SimplePhraseComponent::m_someOtherMember),
            );
        }

- Implement the "expose_to_lua" function if we want to expose it and then
go to Entity.cpp and register it adding to the "expose_to_lua" function.

        void SimplePhraseComponent::expose_to_lua() {

            LUA.new_usertype<SimplePhraseComponent>("SimplePhraseComponent",


        Methods:
        Add here all the functions we want to expose to lua

        Data Members:
        Add here all the members we want to expose to lua

        "member", sol::property(&ThisComponent::getMember, &ThisComponent::setMember)

        );
        }

- Implement the drawComponentInspector function so we can show this component on
the inspector when using the debug GUI:

        void drawComponentInspector() override;

- Implement the JSON serialization methods to save and load state of components:

        json to_json() override;
        void load_json(const json& j) override;

- If we want the Entity Factory to be able to instantiate the entity, which we
probably do, we need to go to the ComponentManager.h file and register the component
class in both the (name, typeid) map and the (typeid, function) map with the function
implementing the way we can instantiate the component based on a Lua script


@see ComponentManager @ ComponentManager.h
@see Entity @ Entity.cpp
*/
template<typename T>
class ComponentTemplate : public Component {

public:
    ComponentTemplate(EntityId id, std::type_index type) : Component(id, type) {}
    virtual ~ComponentTemplate() {}

    /**
    
    Draws the general information about every component such as its
    name and all the functionality that indicates if we should
    show the inspector or not.

    */
    void draw_debug_gui() {
        ImGui::PushID(this);
        if (m_gui_open) { draw_component_inspector(); }
        ImGui::Bullet();
        auto name = std::string(meta:: template get_name<T>());
        auto size = name.size();
        auto ss = std::make_unique<std::stringstream>();
        for (int i = 0; i < size; ++i) {
            if (!islower(name[i]))*ss << ' ';
            *ss << name[i];
        }
        if (ImGui::SmallButton(ss->str().c_str())) {
            m_gui_open = true;
        };
        ImGui::PopID();
    }

    /**
    
    Returns the name of the real type of the concrete component

    */
    virtual std::string get_component_type_name() {
        return meta::get_name<T>();
    }

protected:

    /**
    
    Calculates the name that should be shown when rendering the inspector
    of the component in developer mode. It includes the component name
    with an space before all its caps and the ID of the parent entity.

    That is, if the entity 2 has a TransformComponent it will
    return the string: 
    
        [2] Transform Component

    */
    std::string calculate_showname() {

        auto name = std::string(meta:: template get_name<T>());
        auto size = name.size();
        auto ssn = std::stringstream{};
        for (int i = 0; i < size; ++i) {
            if (!islower(name[i])) { ssn << ' '; }
            ssn << name[i];
        }

        auto ss = std::stringstream{};
        ss << "[" << m_parent_id << "] " << ssn.str();

        return ss.str();
    }

    /**
    
    Draws the component inspector of the concrete component.

    */
    virtual void draw_component_inspector() = 0;
};


