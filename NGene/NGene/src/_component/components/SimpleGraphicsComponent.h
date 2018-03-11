#pragma once
#include "../Component.h"
#include <string>
#include <meta.h>
#include <sol.hpp>

class SimpleGraphicsComponent : public Component {
    friend auto meta::registerMembers<SimpleGraphicsComponent>();
public:
    SimpleGraphicsComponent();
    SimpleGraphicsComponent(const sol::table& table);
    ~SimpleGraphicsComponent();

    static void exposeToLua();

    std::string getFilename() const;
    void setFilename(const std::string& filename);

private:
    std::string m_filename;
};

template<>
inline auto meta::registerName<SimpleGraphicsComponent>() {
    return "SimpleGraphicsComponent";
}

template<>
inline auto meta::registerMembers<SimpleGraphicsComponent>() {
    return members(
        member("Filename", &SimpleGraphicsComponent::m_filename)
    );
}

