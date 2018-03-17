#pragma once
#include "../ComponentTemplate.h"
#include <SFML\Graphics.hpp>

namespace config {
    const float sprite_padding = 20.0f;
}


class SpriteComponent : public ComponentTemplate<SpriteComponent> {
    friend auto meta::registerMembers<SpriteComponent>();
public:
    SpriteComponent(EntityId id);
    SpriteComponent(EntityId id, const sol::table& table);
    ~SpriteComponent();

    json to_json() override;
    void load_json(const json& j) override;

    std::string get_filename() const;
    void set_filename(const std::string& filename);

    sf::Sprite* get_sprite_ptr();
    sf::Sprite get_sprite() const;
    void set_sprite(const sf::Sprite& sprite);
    void set_sprite_rval(sf::Sprite&& sprite);

    void draw_component_inspector() override;

    static void expose_to_lua();

    void load_sprite();

    std::string m_filename;
    sf::Sprite m_sprite;
    sf::Texture* m_texture;
    unsigned int m_layer{ 1000 };

};

template<>
inline auto meta::registerName<SpriteComponent>() {
    return "SpriteComponent";
}

template<>
inline auto meta::registerMembers<SpriteComponent>() {
    return members(
        member("filename", &SpriteComponent::m_filename),
        member("layer", &SpriteComponent::m_layer)
    );
}

