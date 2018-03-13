#include "SpriteComponent.h"
#include <Debug.h>
#include "../lua/LuaManager.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

SpriteComponent::SpriteComponent(EntityId id) : Component(id, std::type_index(typeid(SpriteComponent))) {
}


SpriteComponent::SpriteComponent(EntityId id, const sol::table& table)
    : Component(id, std::type_index(typeid(SpriteComponent))) {

    meta::doForAllMembers<SpriteComponent>([this, &table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.getName();
        sol::object value_obj = table[name];
        assert(value_obj.valid());
        auto value = value_obj.as<std::string>();
        member.set(*this, value);
    });

    if (m_filename.size() > 0) {
        if (m_texture.loadFromFile(m_filename)) {
            m_sprite = sf::Sprite(m_texture);
        }
    }
}

SpriteComponent::~SpriteComponent() {
}


void SpriteComponent::drawDebugGUI() {
    return Component::drawBullet<SpriteComponent>(m_parentId);
}

void SpriteComponent::drawComponentInspector() {



    ImGui::SetNextWindowSize(ImVec2(320, 420), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculateShowname<SpriteComponent>(m_parentId).c_str(), &m_guiOpen);

    ImGui::Text("File Name: %s", m_filename.c_str());

    if (auto texture = m_sprite.getTexture()) {
        ImGui::Text("Texture with size (%d, %d)", texture->getSize().x, texture->getSize().y);
        auto original_scale = m_sprite.getScale();
        m_sprite.setScale(
            (ImGui::GetWindowSize().x - config::sprite_padding) / static_cast<float>(texture->getSize().x),
            (ImGui::GetWindowSize().x - config::sprite_padding) / static_cast<float>(texture->getSize().x));
        ImGui::Image(m_sprite);
        m_sprite.setScale(original_scale);
    }
    else {
        ImGui::Text("Texture for the sprite not available");
    }


    //To Check the window size and adjust default sizes:
    ImGui::Text("Window Size: (%f, %f)", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

    ImGui::End();


}

void SpriteComponent::exposeToLua() {

    LUA.new_usertype<SpriteComponent>("SpriteComponent",


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "filename", sol::property(&SpriteComponent::getFilename, &SpriteComponent::setFilename)

        );

}


std::string SpriteComponent::getFilename() const {
    return m_filename;
}


void SpriteComponent::setFilename(const std::string& filename) {
    m_filename = filename;
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
