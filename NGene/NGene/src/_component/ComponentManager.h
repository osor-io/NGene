#pragma once

#include <unordered_map>
#include <typeindex>
#include <sol.hpp>
#include <functional>
#include <meta.h>
#include <Debug.h>

#include "../utils/CRSP.h"
#include "../_entity/Entity.h"

#include "./components/SimpleGraphicsComponent.h"
#include "./components/SimplePhraseComponent.h"

class ComponentManager : public CRSP<ComponentManager> {
    using ComponentVector = std::vector<Component*>;
    using InstanceMap = std::unordered_map<std::type_index, ComponentVector>;
    using TypeMap = std::unordered_map<std::string, std::type_index>;
    using FactoryMap = std::unordered_map<std::type_index, std::function<void(Entity& entity, const sol::table& table)>>;

private:
    friend class CRSP <ComponentManager>;
    ComponentManager();
    ~ComponentManager();
public:

    void addComponentInstance(const std::type_index& type, Component* component);
    void removeComponentInstance(const std::type_index& type, Component* component);

    void addComponentToEntity(Entity& entity, const std::string& name, const sol::table& table) {
        LOG_NAMED(name);
        m_factoryMap.at(m_typeMap.at(name))(entity, table);
    }

private:
    InstanceMap m_instanceMap{};

    const TypeMap m_typeMap{

        std::make_pair(
            meta::getName<SimpleGraphicsComponent>(),
            std::type_index(typeid(SimpleGraphicsComponent))),

        std::make_pair(
            meta::getName<SimplePhraseComponent>(),
            std::type_index(typeid(SimplePhraseComponent))),
    };

    const FactoryMap m_factoryMap{

        std::make_pair(std::type_index(typeid(
            SimpleGraphicsComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.makeComponent<SimpleGraphicsComponent>(table);
            }),

        std::make_pair(std::type_index(typeid(
            SimplePhraseComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.makeComponent<SimplePhraseComponent>(table);
            }),

    };

};