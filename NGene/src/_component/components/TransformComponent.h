#pragma once
#include "../Component.h"
#include <string>
#include <meta.h>
#include <sol.hpp>
#include <SFML\Graphics.hpp>


namespace config{

    const sf::Vector2f minPosition(-2048.0f, -2048.0f);
    const sf::Vector2f maxPosition(2048.0f, 2048.0f);

}

struct LuaVector2f {
    static sf::Vector2f toSf(LuaVector2f vector) { return sf::Vector2f(vector.x, vector.y); }
    LuaVector2f() {}
    LuaVector2f(const sf::Vector2f vector) : x(vector.x), y(vector.y) {}
    float x;
    float y;
};


class TransformComponent : public Component {
    friend auto meta::registerMembers<TransformComponent>();
public:
    TransformComponent(EntityId id);
    TransformComponent(EntityId id, const sol::table& table);
    ~TransformComponent();

    void drawDebugGUI() override;
    void drawComponentInspector() override;

    static void exposeToLua();

    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& position);

    LuaVector2f luaGetPosition() const;
    void luaSetPosition(const LuaVector2f& position);

private:
    sf::Vector2f m_position;
};

template<>
inline auto meta::registerName<TransformComponent>() {
    return "TransformComponent";
}

template<>
inline auto meta::registerMembers<TransformComponent>() {
    return members(
        member("position", &TransformComponent::m_position)
    );
}

