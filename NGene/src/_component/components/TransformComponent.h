#pragma once
#include "../ComponentTemplate.h"
#include <SFML\Graphics.hpp>
#include "../lua/LuaTypes.h"

namespace config {

    const sf::Vector2f min_position(-2048.0f, -2048.0f);
    const sf::Vector2f max_position(2048.0f, 2048.0f);

}

class TransformComponent : public ComponentTemplate<TransformComponent> {
public:
    TransformComponent(EntityId id);
    TransformComponent(EntityId id, const sol::table& table);
    ~TransformComponent();

    json to_json() override;
    void load_json(const json& j) override;

    sf::Vector2f get_position() const;
    sf::Vector2f& get_position_ref();
    void set_position(const sf::Vector2f& position);

    LuaVector2f& lua_get_position();
    void lua_set_position(const LuaVector2f& position);

    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua();
    friend auto meta::registerMembers<TransformComponent>();
    // ====== END OF REQUIREMENTS ======

    // ====== BEG OF MEMBERS ======
    sf::Vector2f m_position;
    // ====== END OF MEMBERS ======

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

