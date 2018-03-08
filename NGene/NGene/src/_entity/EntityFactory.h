#pragma once

#include <unordered_map>
#include <typeindex>
#include <sol.hpp>
#include <functional>
#include <meta.h>
#include "../_entity/Entity.h"
#include "../utils/CRSP.h"

#include "../_component/components/SimpleGraphicsComponent.h"
#include "../_component/components/SimplePhraseComponent.h"



/**
Factory class that has the capability to instantiate an Entity based on
a Lua script and add all of its necessary components.

It holds two maps containing the supported components for every entity and
the function necessary to initialize each of those components from the
data read from the Lua script.

@see Entity
*/
class EntityFactory : public CRSP<EntityFactory> {
public:
    EntityFactory();
    ~EntityFactory();

    /*
    @@DOING: Implementing the loadEntity and addComponentToEntity
    with the different arguments (taking the name of the Component
    for example).

    Useful resources:
        - http://sol2.readthedocs.io/en/latest/tutorial/all-the-things.html
        - https://eliasdaler.wordpress.com/2015/09/08/using-lua-with-cpp-in-practice-part-2/
    */

    template<typename T>
    void addComponentToEntity(Entity& entity, sol::table table) {
        m_constructorMap.at(std::type_index(typeid(T)))(entity, table);
    }

private:
    const std::unordered_map<std::string, std::type_index> m_typeMap{
        std::make_pair(
            meta::getName<SimpleGraphicsComponent>(),
            std::type_index(typeid(SimpleGraphicsComponent))),
        std::make_pair(
            meta::getName<SimplePhraseComponent>(),
            std::type_index(typeid(SimplePhraseComponent))),
    };

    const std::unordered_map<std::type_index, std::function<void(Entity& entity, const sol::table& table)>> m_constructorMap{
    std::make_pair(std::type_index(typeid(
        SimpleGraphicsComponent
        )),[](Entity& entity ,const sol::table& table) {
        //@@TODO: Implement construction of SimpleGraphicsComponent from table and insertion into entity.
    }),

    std::make_pair(std::type_index(typeid(
        SimplePhraseComponent
        )),[](Entity& entity ,const sol::table& table) {
        //@@TODO: Implement construction of SimplePhraseComponent from table and insertion into entity.
    }),

    };

};

