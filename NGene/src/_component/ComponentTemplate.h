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

- Implement the "exposeToLua" function if we want to expose it and then
go to Entity.cpp and register it adding to the "exposeToLua" function.

        void SimplePhraseComponent::exposeToLua() {

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
    ~ComponentTemplate() {}
    
    void drawDebugGUI() {
        ImGui::PushID(this);
        if (m_guiOpen) { drawComponentInspector(); }
        ImGui::Bullet();
        auto name = std::string(meta:: template getName<T>());
        auto size = name.size();
        auto ss = std::make_unique<std::stringstream>();
        for (int i = 0; i < size; ++i) {
            if (!islower(name[i]))*ss << ' ';
            *ss << name[i];
        }
        if (ImGui::SmallButton(ss->str().c_str())) {
            m_guiOpen = true;
        };
        ImGui::PopID();
    }

protected:
    std::string calculateShowname() {

        auto name = std::string(meta:: template getName<T>());
        auto size = name.size();
        auto ssn = std::stringstream{};
        for (int i = 0; i < size; ++i) {
            if (!islower(name[i])) { ssn << ' '; }
            ssn << name[i];
        }

        auto ss = std::stringstream{};
        ss << "[" << m_parentId << "] " << ssn.str();

        return ss.str();
    }

    virtual void drawComponentInspector() = 0;

};


