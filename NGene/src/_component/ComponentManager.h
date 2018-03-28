#pragma once

#include <unordered_map>
#include <typeindex>
#include <sol.hpp>
#include <functional>
#include <meta.h>
#include <Debug.h>

#include <Manager.h>
#include "../_entity/Entity.h"

#include "./components/PlatformerPhysicsComponent.h"
#include "./components/CollisionComponent.h"
#include "./components/CameraComponent.h"
#include "./components/TiledMapComponent.h"
#include "./components/ExtentComponent.h"
#include "./components/InputComponent.h"
#include "./components/BehaviourComponent.h"
#include "./components/TransformComponent.h"
#include "./components/SpriteComponent.h"
#include "./components/PhraseComponent.h"

class ComponentManager : public Manager<ComponentManager> {
public:
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

    template<typename T>
    bool has_components() const;

    template<typename T>
    const ComponentVector& get_components();

    void add_component_instance(const std::type_index& type, Component* component);
    void remove_component_instance(const std::type_index& type, Component* component);

    void add_component_to_entity(Entity& entity, const std::string& name, const sol::table& table) {
        m_factory_map.at(m_type_map.at(name))(entity, table);
    }

    bool supports_component(const std::string& name) const;
    
private:
    InstanceMap m_instance_map{};


#define DEFINE_TYPE(x) std::make_pair(meta::get_name<##x##>(),std::type_index(typeid(##x##))),

    const TypeMap m_type_map{

        DEFINE_TYPE(PlatformerPhysicsComponent)
        DEFINE_TYPE(CollisionComponent)
        DEFINE_TYPE(CameraComponent)
        DEFINE_TYPE(TiledMapComponent)
        DEFINE_TYPE(ExtentComponent)
        DEFINE_TYPE(InputComponent)
        DEFINE_TYPE(BehaviourComponent)
        DEFINE_TYPE(TransformComponent)
        DEFINE_TYPE(SpriteComponent)
        DEFINE_TYPE(PhraseComponent)
    };

#define DEFINE_FACTORY(x) std::make_pair(std::type_index(typeid(##x##)), [](Entity& entity, const sol::table& table) {entity.make_component<##x##>(entity.get_id(), table);}),

    const FactoryMap m_factory_map{

        DEFINE_FACTORY(PlatformerPhysicsComponent)
        DEFINE_FACTORY(CollisionComponent)
        DEFINE_FACTORY(CameraComponent)
        DEFINE_FACTORY(TiledMapComponent)
        DEFINE_FACTORY(ExtentComponent)
        DEFINE_FACTORY(InputComponent)
        DEFINE_FACTORY(BehaviourComponent)
        DEFINE_FACTORY(TransformComponent)
        DEFINE_FACTORY(SpriteComponent)
        DEFINE_FACTORY(PhraseComponent)
    };

};

template<typename T>
inline bool ComponentManager::has_components() const {
    return m_instance_map.find(std::type_index(typeid(T))) != m_instance_map.end();
}

template<typename T>
inline const ComponentManager::ComponentVector & ComponentManager::get_components() {
    return m_instance_map.at(std::type_index(typeid(T)));
}
