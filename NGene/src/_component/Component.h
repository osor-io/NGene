#pragma once
#include <typeindex>
#include <string>
#include <Debug.h>
#include "../_entity/Entity.h"
#include "../memory/SmallMemoryAllocator.h"
/**
Class that every component in this engine should inherit from. In order to
correctly register everything in a component the usage is as follows:

- Include the required headers in the .h or .hpp

        #include "../Component.h"
        #include <string>
        #include <meta.h>
        #include <sol.hpp>

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

- Implement the "exposeToLua" function if we want to expose it

        void SimplePhraseComponent::exposeToLua() {

            LUA.new_usertype<SimplePhraseComponent>("SimplePhraseComponent",


            Methods:
            Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)

            Data Members:
            Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)


                "member", sol::property(&SimplePhraseComponent::getPhrase, &SimplePhraseComponent::setPhrase)

            );
        }

- If we want the Entity Factory to be able to instantiate the entity, which we
probably do, we need to go to the ComponentManager.h file and register the component
class in both the (name, typeid) map and the (typeid, function) map with the function
implementing the way we can instantiate the component based on a Lua script

- If we want to expose the component to lua we need to go to Entity.cpp and register it
adding to the "exposeToLua" function.

@see ComponentManager @ ComponentManager.h
@see Entity @ Entity.cpp
*/
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

    virtual void drawComponentInspector() = 0;

    template<typename T>
    void drawBullet(EntityId id) {
        ImGui::PushID(this);
        if (m_guiOpen) { drawComponentInspector(); }
        ImGui::Bullet();
        auto name = std::string(meta::getName<T>());
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
    bool m_guiOpen{ false };
    EntityId m_parentId;
private:
    std::type_index m_type;
};


template<typename T>
std::string calculateShowname(EntityId id) {

    auto name = std::string(meta:: template getName<T>());
    auto size = name.size();
    auto ssn = std::stringstream{};
    for (int i = 0; i < size; ++i) {
        if (!islower(name[i])) { ssn << ' '; }
        ssn << name[i];
    }

    auto ss = std::stringstream{};
    ss << "[" << id << "] " << ssn.str();

    return ss.str();
}



