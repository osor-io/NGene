#pragma once
#include "../Component.h"
#include <string>
#include <meta.h>
#include <sol.hpp>
#include <SFML\Graphics.hpp>

namespace config {
    const float sprite_padding = 20.0f;
}


class SpriteComponent : public Component {
    friend auto meta::registerMembers<SpriteComponent>();
public:
    SpriteComponent(EntityId id);
    SpriteComponent(EntityId id, const sol::table& table);
    ~SpriteComponent();

    void drawDebugGUI() override;
    void drawComponentInspector() override;

    static void exposeToLua();

    std::string getFilename() const;
    void setFilename(const std::string& filename);

    sf::Sprite getSprite() const;
    void setSprite(const sf::Sprite& sprite);
    void moveSprite(sf::Sprite&& sprite);

private:
    std::string m_filename;
    sf::Sprite m_sprite;
    sf::Texture m_texture;
};

template<>
inline auto meta::registerName<SpriteComponent>() {
    return "SpriteComponent";
}

template<>
inline auto meta::registerMembers<SpriteComponent>() {
    return members(
        member("filename", &SpriteComponent::m_filename)
    );
}

