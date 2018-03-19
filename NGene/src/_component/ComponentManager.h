#pragma once

#include <unordered_map>
#include <typeindex>
#include <sol.hpp>
#include <functional>
#include <meta.h>
#include <Debug.h>

#include <Manager.h>
#include "../_entity/Entity.h"

#include "./components/TiledMapComponent.h"
#include "./components/ExtentComponent.h"
#include "./components/InputComponent.h"
#include "./components/BehaviourComponent.h"
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

    void start_up() override;
    void shut_down() override;

    void add_component_instance(const std::type_index& type, Component* component);
    void remove_component_instance(const std::type_index& type, Component* component);

    void add_component_to_entity(Entity& entity, const std::string& name, const sol::table& table) {
        m_factory_map.at(m_type_map.at(name))(entity, table);
    }

private:
    InstanceMap m_instance_map{};

    const TypeMap m_type_map{
      
        std::make_pair(
            meta::get_name<TiledMapComponent>(),
            std::type_index(typeid(TiledMapComponent))),

        std::make_pair(
            meta::get_name<ExtentComponent>(),
            std::type_index(typeid(ExtentComponent))),

        std::make_pair(
            meta::get_name<InputComponent>(),
            std::type_index(typeid(InputComponent))),

        std::make_pair(
            meta::get_name<BehaviourComponent>(),
            std::type_index(typeid(BehaviourComponent))),

        std::make_pair(
            meta::get_name<TransformComponent>(),
            std::type_index(typeid(TransformComponent))),

        std::make_pair(
            meta::get_name<SpriteComponent>(),
            std::type_index(typeid(SpriteComponent))),

        std::make_pair(
            meta::get_name<PhraseComponent>(),
            std::type_index(typeid(PhraseComponent))),
    };

    const FactoryMap m_factory_map{

        std::make_pair(std::type_index(typeid(
            TiledMapComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.make_component<TiledMapComponent>(entity.get_id(), table);
            }),

        std::make_pair(std::type_index(typeid(
            ExtentComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.make_component<ExtentComponent>(entity.get_id(), table);
            }),

        std::make_pair(std::type_index(typeid(
            InputComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.make_component<InputComponent>(entity.get_id(), table);
            }),

        std::make_pair(std::type_index(typeid(
            BehaviourComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.make_component<BehaviourComponent>(entity.get_id(), table);
            }),

        std::make_pair(std::type_index(typeid(
            TransformComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.make_component<TransformComponent>(entity.get_id(), table);
            }),

        std::make_pair(std::type_index(typeid(
            SpriteComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.make_component<SpriteComponent>(entity.get_id(), table);
            }),

        std::make_pair(std::type_index(typeid(
            PhraseComponent
            )),[](Entity& entity ,const sol::table& table) {
                entity.make_component<PhraseComponent>(entity.get_id(), table);
            }),

    };

};