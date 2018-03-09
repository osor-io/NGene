#include "EntityManager.h"
#include "../_component/ComponentManager.h"

EntityManager::EntityManager() {}


EntityManager::~EntityManager() {}

Entity * EntityManager::loadEntity(const sol::table & table, const std::string & name)
{
    auto e = new Entity();
    e->setType(name);
    const sol::table componentTable = table[name];
    componentTable.for_each([&e](const auto& key, const auto& value){
   
        if (key.is<std::string>()) {
            auto component_name = key.as<std::string>();
            ComponentManager::get().addComponentToEntity(*e, component_name , value.as<sol::table>());
        }
        else {
        // The key was not a string, we will ignore it.
        }

    });

    m_topEntities.push_back(std::unique_ptr<Entity>(e));

    return e;
}
