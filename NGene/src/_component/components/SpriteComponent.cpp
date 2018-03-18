#include "SpriteComponent.h"
#include <Debug.h>
#include "../lua/LuaManager.h"
#include <fstream>
#include "../../resources/TextureManager.h"
#include "../../config/Config.h"

SpriteComponent::SpriteComponent(EntityId id) : ComponentTemplate(id, std::type_index(typeid(SpriteComponent))) {
    LOGF("NOT THISSSS");
}


SpriteComponent::SpriteComponent(EntityId id, const sol::table& table)
    : ComponentTemplate(id, std::type_index(typeid(SpriteComponent))) {

    meta::doForAllMembers<SpriteComponent>([this, &table](auto& member) {
        using MemberT = meta::get_member_type<decltype(member)>;
        auto name = member.get_name();
        sol::object value_obj = table[name];
        if (value_obj.valid()) {
            auto value = value_obj.as<MemberT>();
            member.set(*this, value);
        }
    });
    m_texture = nullptr;
    load_sprite();
}

SpriteComponent::~SpriteComponent() {
    if (m_texture) {
        auto released = TextureManager::get().release_required_resource(m_filename);
        assert(released);
    }
}

void SpriteComponent::load_sprite() {

    if (m_texture) {
        auto released = TextureManager::get().release_required_resource(m_filename);
        assert(released);
    }

    if (TextureManager::get().exists_resource(m_filename))
        m_texture = TextureManager::get().get_required_resource(m_filename);

    if (m_texture)
        m_sprite.setTexture(*m_texture);


    auto origin = m_sprite.getOrigin();
    m_sprite.setOrigin(origin + (sf::Vector2f(m_texture->getSize()) / 2.f));

    /*
    //Old way of loading the texture directly
    if (m_filename.size() > 0) {
        std::ifstream f(m_filename.c_str());

        if (f.good() && m_texture.loadFromFile(m_filename)) {
            m_sprite = sf::Sprite(m_texture);
        }
    }
    */
}


json SpriteComponent::to_json() {
    auto j = json{};

    j["filename"] = m_filename;
    j["layer"] = m_layer;

    return j;
}

void SpriteComponent::load_json(const json & j) {
    m_filename = j["filename"].get<std::string>();
    m_layer = j["layer"];
    load_sprite();
}


std::string SpriteComponent::get_filename() const {
    return m_filename;
}

sf::Sprite * SpriteComponent::get_sprite_ptr() {
    return &m_sprite;
}

sf::Sprite SpriteComponent::get_sprite() const {
    return m_sprite;
}

void SpriteComponent::set_sprite(const sf::Sprite & sprite) {
    m_sprite = sprite;
}

void SpriteComponent::set_sprite_rval(sf::Sprite && sprite) {
    m_sprite = std::move(sprite);
}

void SpriteComponent::draw_component_inspector() {

    ImGui::SetNextWindowSize(ImVec2(320, 420), ImGuiCond_FirstUseEver);

    ImGui::Begin(calculate_showname().c_str(), &m_gui_open);

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

    ImGui::End();
}

void SpriteComponent::expose_to_lua() {

    LUA.new_usertype<SpriteComponent>(meta::get_name<SpriteComponent>(),


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "filename", sol::readonly_property(&SpriteComponent::get_filename),
        "layer", &SpriteComponent::m_layer

        );

}