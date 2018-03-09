#pragma once

#include "../_component/Component.h"
#include <unordered_map>
#include <string>
#include <typeindex>
#include <memory>

class Entity {
public:
    Entity();
    ~Entity();

    std::string getType() const;
    void setType(const std::string& type);
    void setType(std::string&& type);

    void exposeToLua();

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

private:
    std::string m_type;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;
};

