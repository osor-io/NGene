#pragma once

#include <unordered_map>
#include <string>
#include <typeindex>
#include <memory>
#include "../memory/SmallMemoryAllocator.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include "../config/Config.h"
#include <JsonCast.h>

class EntityManager;

class Component;

using EntityId = unsigned int;

namespace config {
    constexpr auto max_name_length = 32;
}

class Entity {
    friend auto meta::registerMembers<Entity>();
    friend class EntityManager;
    friend int textEditCallback(ImGuiTextEditCallbackData *data);
private:
    Entity(EntityId id, std::string&& name, const std::string& type);
public:
    ~Entity();

    json toJson();
    void loadJson(const json& j);

    void* operator new(size_t count) {
        return SmallMemoryAllocator::get().alloc(count);
    }

    void operator delete(void* ptr) {
        return SmallMemoryAllocator::get().dealloc(ptr);
    }

    EntityId getId() const;

    std::string getType() const;
    void setType(const std::string& type);
    void setType(std::string&& type);

    std::string getName() const;
    const std::string& getNameRef() const;
    void setName(const std::string& name);

    void setEnabled(bool);
    bool isEnabled();

    static void exposeToLua();

    void drawDebugGUI();
    std::string getShowName();

    template<typename T, typename ... Args>
    T* makeComponent(Args&& ... args) {
        auto ptr = new T(std::forward<Args>(args)...);
        m_components[std::type_index(typeid(T))] = std::unique_ptr<T>(ptr);
        return ptr;
    }

    template<typename T>
    T* getComponent() {
        auto it = m_components.find(std::type_index(typeid(T)));
        if (it != m_components.end()) {
            return dynamic_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    bool hasComponent() {
        auto it = m_components.find(std::type_index(typeid(T)));
        if (it != m_components.end()) {
            return true;
        }
        return false;
    }

private:
    std::string m_name;
    std::string m_showName;
    bool m_changedHeader{ false };
    unsigned int m_id;
    bool m_enabled{ true };
    std::string m_type;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;


    void generateShowName();
};

template<>
inline auto meta::registerName<Entity>() {
    return "Entity";
}

template<>
inline auto meta::registerMembers<Entity>() {
    return members(
        member("id", &Entity::m_id),
        member("name", &Entity::getNameRef, &Entity::setName),
        member("type", &Entity::m_type)
    );
}
