#pragma once

#include <unordered_map>
#include <string>
#include <typeindex>
#include <memory>
#include <Meta.h>
#include <JsonCast.h>
#include "../memory/SmallMemoryAllocator.h"
#include "../config/Config.h"
#include "../types/def.h"

class EntityManager;

class Component;


namespace config {
    constexpr auto max_name_length = 32;
}

/**

Represents an entity in the engine. Each one is made by a set of component that define
what it is. The combination of the components and the parameters these have will define
how each entity behaves.

It contains metainformation to make it easier to work with it, that is, instantiate the
entity from an entity type in LUA, and ID and name to identify it, wether we can serialize
it or not, etc.

*/
class Entity {
    friend auto meta::registerMembers<Entity>();
    friend class EntityManager;
    friend int text_edit_callback(ImGuiTextEditCallbackData *data);
private:
    Entity(EntityId id, std::string&& name, const std::string& type);
public:
    ~Entity();

    nlohmann::json to_json();
    void load_json(const nlohmann::json& j);

    void* operator new(size_t count) {
        return SmallMemoryAllocator::get().alloc(count);
    }

    void operator delete(void* ptr) {
        return SmallMemoryAllocator::get().dealloc(ptr);
    }

    EntityId get_id() const;

    std::string get_type() const;
    void set_type(const std::string& type);
    void set_type(std::string&& type);

    std::string get_name() const;
    const std::string& get_name_ref() const;
    void set_name(const std::string& name);

    void set_enabled(bool);
    bool is_enabled() const;

    void set_in_relevant_chunk(bool);
    bool is_in_relevant_chunk() const;

    static void expose_to_lua();

    void draw_debug_gui();
    std::string get_show_name() const;

    template<typename T, typename ... Args>
    T* make_component(Args&& ... args) {
        auto ptr = new T(std::forward<Args>(args)...);
        m_components[std::type_index(typeid(T))] = std::unique_ptr<T>(ptr);
        return ptr;
    }

    template<typename T>
    T* get_component() {
        auto it = m_components.find(std::type_index(typeid(T)));
        if (it != m_components.end()) {
            return dynamic_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    bool has_component() const {
        auto it = m_components.find(std::type_index(typeid(T)));
        if (it != m_components.end()) {
            return true;
        }
        return false;
    }

private:
    std::string m_name;
    std::string m_showName;
    bool m_changed_header{ false };
    unsigned int m_id;
    bool m_enabled{ true };
    bool m_in_relevant_chunk{ true };
    std::string m_type;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;
    bool m_serializable{ true };

    void generate_show_name();
};

template<>
inline auto meta::registerName<Entity>() {
    return "Entity";
}

template<>
inline auto meta::registerMembers<Entity>() {
    return members(
        member("id", &Entity::m_id),
        member("name", &Entity::get_name_ref, &Entity::set_name),
        member("type", &Entity::m_type),
        member("enabled", &Entity::m_enabled)
    );
}
