#pragma once

#include <unordered_map>
#include <typeindex>
#include <sol.hpp>
#include <functional>
#include <meta.h>
#include <Debug.h>

#include <Manager.h>
#include "../_entity/Entity.h"

#include "./components/TransformComponent.h"
#include "./components/SpriteComponent.h"
#include "./components/PhraseComponent.h"

class ComponentManager : public Manager<ComponentManager> {
    using ComponentVector = std::vector<Component*>;
    using InstanceMap = std::unordered_map<std::type_index, ComponentVector>;
    using TypeMap = std::unordered_map<std::string, std::type_index>;
    using FactoryMap = std::unordered_map<std::type_index, std::function<void(Entity& entity, const sol::table& table)>>;

private:
    friend class CRSP <ComponentManager>;
    ComponentManager();
    ~ComponentManager();

public:

    void startUp() override;
    void shutDown() override;

    void addComponentInstance(const std::type_index& type, Component* component);
    void removeComponentInstance(const std::type_index& type, Component* component);

    void addComponentToEntity(Entity& entity, const std::string& name, const sol::table& table) {
        m_factoryMap.at(m_typeMap.at(name))(entity, table);
    }

private:
    InstanceMap m_instanceMap{};

    const TypeMap m_typeMap{

        std::make_pair(
            meta::getName<TransformComponent>(),
            std::type_index(typeid(TransformComponent))),

        std::make_pair(
            meta::getName<SpriteComponent>(),
            std::type_index(typeid(SpriteComponent))),

        std::make_pair(
            meta::getName<PhraseComponent>(),
            std::type_index(typeid(PhraseComponent))),
    };

    const FactoryMap m_factoryMap{

        std::make_pair(std::type_index(typeid(
            TransformComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.makeComponent<TransformComponent>(table);
            }),

        std::make_pair(std::type_index(typeid(
            SpriteComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.makeComponent<SpriteComponent>(table);
            }),

        std::make_pair(std::type_index(typeid(
            PhraseComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.makeComponent<PhraseComponent>(table);
            }),

    };

};