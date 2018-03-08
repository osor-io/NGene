#pragma once

/**
Class that every component in this engine should inherit from. In order to
correctly register everything in a component the usage is as follows:

-Include the <meta.h> header

        #include <meta.h>

-Declare a friend registerMembers function:

        friend auto meta::registerMembers<SimpleGraphicsComponent>();

-Implement registerName and registerMembers functions:

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

-If we want the Entity Factory to be able to instantiate the entity, which we
probably do, we need to go to the EntityFactory.h file and register the component
class in both the (name, typeid) map and the (typeid, function) map with the function
implementing the way we can instantiate the component based on a Lua script

@see EntityFactory
*/
class Component {
public:
    virtual ~Component() = 0;
};



