#pragma once

#include <sol.hpp>
#include "../_entity/Entity.h"
#include <Manager.h>


/**
Factory class that has the capability to instantiate an Entity based on
a Lua script and add all of its necessary components.

It holds two maps containing the supported components for every entity and
the function necessary to initialize each of those components from the
data read from the Lua script.

@see Entity
*/
class EntityManager : public Manager<EntityManager> {
    using OwningVector = std::vector<std::unique_ptr<Entity>>;

private:
    friend class CRSP<EntityManager>;
    EntityManager();
    ~EntityManager();

public:

    void startUp() override;
    void shutDown() override;

    /*
    @@DOING: Implementing the loadEntity and addComponentToEntity
    with the different arguments (taking the name of the Component
    for example).

    Useful resources:
    - http://sol2.readthedocs.io/en/latest/tutorial/all-the-things.html
    - https://eliasdaler.wordpress.com/2015/09/08/using-lua-with-cpp-in-practice-part-2/
    */


    Entity * loadEntity(const sol::table& table, const std::string& name);

private:
    OwningVector m_topEntities{};

};

