#include "SpriteComponent.h"
#include <Debug.h>
#include "../lua/LuaManager.h"
#include <fstream>

SpriteComponent::SpriteComponent(EntityId id) : ComponentTemplate(id, std::type_index(typeid(SpriteComponent))) {
}


SpriteComponent::SpriteComponent(EntityId id, const sol::table& table)
    : ComponentTemplate(id, std::type_index(typeid(SpriteComponent))) {

    meta::doForAllMembers<SpriteComponent>([this, &table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.getName();
        sol::object value_obj = table[name];
        if (value_obj.valid()) {
            auto value = value_obj.as<MemberT>();
            member.set(*this, value);
        }
    });
    loadSprite();
}

SpriteComponent::~SpriteComponent() {
}

void SpriteComponent::loadSprite() {
    if (m_filename.size() > 0) {
        std::ifstream f(m_filename.c_str());

        if (f.good() && m_texture.loadFromFile(m_filename)) {
            m_sprite = sf::Sprite(m_texture);
        }
    }
}


json SpriteComponent::toJson() {
    auto j = json{};

    j["filename"] = m_filename;
    j["layer"] = m_layer;

    return j;
}

void SpriteComponent::loadJson(const json & j) {
    m_filename = j["filename"].get<std::string>();
    m_layer = j["layer"];
    loadSprite();
}


std::string SpriteComponent::getFilename() const {
    return m_filename;
}


void SpriteComponent::setFilename(const std::string& filename) {
    m_filename = filename;
}

sf::Sprite * SpriteComponent::getSpritePtr() {
    return &m_sprite;
}

sf::Sprite SpriteComponent::getSprite() const {
    return m_sprite;
}

void SpriteComponent::setSprite(const sf::Sprite & sprite) {
    m_sprite = sprite;
}

void SpriteComponent::moveSprite(sf::Sprite && sprite) {
    m_sprite = std::move(sprite);
}

void SpriteComponent::drawComponentInspector() {

    ImGui::SetNextWindowSize(ImVec2(320, 420), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculateShowname().c_str(), &m_guiOpen);

    ImGui::Text("File Name: %s", m_filename.c_str());

    if (auto texture = m_sprite.getTexture()) {
        ImGui::Text("Texture with size (%d, %d)", texture->getSize().x, texture->getSize().y);
        ImGui::Text("Layer: %u", m_layer);
        auto original_scale = m_sprite.getScale();
        auto original_position = m_sprite.getPosition();
        m_sprite.setScale(
            (ImGui::GetWindowSize().x - config::sprite_padding) / static_cast<float>(texture->getSize().x),
            (ImGui::GetWindowSize().x - config::sprite_padding) / static_cast<float>(texture->getSize().x));
        m_sprite.setPosition(sf::Vector2f(0.f, 0.f));
        ImGui::Image(m_sprite);
        m_sprite.setScale(original_scale);
        m_sprite.setPosition(original_position);
    }
    else {
        ImGui::Text("Texture for the sprite not available");
    }


    //To Check the window size and adjust default sizes:
    //ImGui::Text("Window Size: (%f, %f)", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

    ImGui::End();


}

void SpriteComponent::exposeToLua() {

    LUA.new_usertype<SpriteComponent>(meta::getName<SpriteComponent>(),


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "filename", sol::property(&SpriteComponent::getFilename, &SpriteComponent::setFilename),
        "layer", &SpriteComponent::m_layer


        );

}