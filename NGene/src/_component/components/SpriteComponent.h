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

    json toJson() override;
    void loadJson(const json& j) override;

    std::string getFilename() const;
    void setFilename(const std::string& filename);

    sf::Sprite* getSpritePtr();
    sf::Sprite getSprite() const;
    void setSprite(const sf::Sprite& sprite);
    void moveSprite(sf::Sprite&& sprite);

    void drawComponentInspector() override;

    static void exposeToLua();

    std::string m_filename;
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    unsigned int m_layer{1000};

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

