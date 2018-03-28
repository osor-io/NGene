#include "Entity.h"

#include <sstream>
#include <Debug.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include "../lua/LuaManager.h"

#include "../debug/LoggingManager.h"


Entity::Entity(EntityId id, std::string&& name, const std::string& type) : m_id(id), m_type(type) {
    set_name(name);
    generate_show_name();
}


Entity::~Entity() {
    m_components.clear();
}

EntityId Entity::get_id() const
{
    return m_id;
}

std::string Entity::get_type() const {
    return m_type;
}

void Entity::set_type(const std::string& type) {
    m_type = type;
}

void Entity::set_type(std::string&& type) {
    m_type = std::move(type);
}


std::string Entity::get_name() const {
    return m_name;
}

const std::string & Entity::get_name_ref() const {
    return m_name;
}

void Entity::set_name(const std::string& name) {
    m_name = name;
    generate_show_name();
}

void Entity::set_enabled(bool enabled) {
    m_enabled = enabled;
}

std::string Entity::get_show_name() const {
    return m_showName;
}

void Entity::generate_show_name() {

    auto ss = std::stringstream{};
    ss << "[" << m_id << "] " << m_type << ": " << m_name << " (" << m_components.size() << ")";
    m_showName = ss.str();

}

bool Entity::is_enabled() const {
    return m_enabled;
}

void Entity::set_in_relevant_chunk(bool in_relevant_chunk) {
    m_in_relevant_chunk = in_relevant_chunk;
}

bool Entity::is_in_relevant_chunk() const {
    return m_in_relevant_chunk;
}

#include "../_component/components/PlatformerPhysicsComponent.h"
#include "../_component/components/CollisionComponent.h"
#include "../_component/components/CameraComponent.h"
#include "../_component/components/TiledMapComponent.h"
#include "../_component/components/ExtentComponent.h"
#include "../_component/components/InputComponent.h"
#include "../_component/components/BehaviourComponent.h"
#include "../_component/components/TransformComponent.h"
#include "../_component/components/SpriteComponent.h"
#include "../_component/components/PhraseComponent.h"

#define GET_COMPONENT_NAME(type) #type
#define MAKE_STRING(s) #s
#define REGISTER_GET_COMPONENT(type) "get" #type , &Entity::get_component< ## type ## >

void Entity::expose_to_lua() {

    /*
    Here we expose the members of the components
    */
    PlatformerPhysicsComponent::expose_to_lua();
    CollisionComponent::expose_to_lua();
    CameraComponent::expose_to_lua();
    TiledMapComponent::expose_to_lua();
    ExtentComponent::expose_to_lua();
    InputComponent::expose_to_lua();
    BehaviourComponent::expose_to_lua();
    TransformComponent::expose_to_lua();
    SpriteComponent::expose_to_lua();
    PhraseComponent::expose_to_lua();

    /*
    And here a way to access them from the entity that contains them
    */
    LUA.new_usertype<Entity>("Entity",

        REGISTER_GET_COMPONENT(PlatformerPhysicsComponent),
        REGISTER_GET_COMPONENT(CollisionComponent),
        REGISTER_GET_COMPONENT(CameraComponent),
        REGISTER_GET_COMPONENT(TiledMapComponent),
        REGISTER_GET_COMPONENT(ExtentComponent),
        REGISTER_GET_COMPONENT(InputComponent),
        REGISTER_GET_COMPONENT(BehaviourComponent),
        REGISTER_GET_COMPONENT(TransformComponent),
        REGISTER_GET_COMPONENT(SpriteComponent),
        REGISTER_GET_COMPONENT(PhraseComponent),

        "id", sol::readonly_property(&Entity::get_id),
        "type", sol::readonly_property(&Entity::get_type),
        "enabled", sol::property(&Entity::is_enabled, &Entity::set_enabled),
        "name", sol::property(&Entity::get_name, &Entity::set_name)

        );

}


int text_edit_callback(ImGuiTextEditCallbackData *data) {
    auto entity = reinterpret_cast<Entity*>(data->UserData);
    entity->set_name(std::string(data->Buf));
    entity->generate_show_name();
    entity->m_changed_header = true;
    return 0;
}


void Entity::draw_debug_gui() {

    auto rustyPalette = config::get_rusty_palette();

    char name_arr[config::max_name_length];

    strncpy(name_arr, m_name.c_str(), sizeof(name_arr));
    name_arr[sizeof(name_arr) - 1] = 0;

    char id_add[config::max_name_length];
    snprintf(id_add, sizeof(id_add), "%s_%d", m_type.c_str(), m_id);

    ImGui::PushID(id_add);

    auto open = ImGui::CollapsingHeader(name_arr);

    if (open || m_changed_header) {

        m_changed_header = false;

        ImGui::Text("Id: %d", m_id); ImGui::SameLine(100); ImGui::Text("Type: %s", m_type.c_str());
        ImGui::Text("Name: "); ImGui::SameLine(100); ImGui::InputText("##Name", name_arr, config::max_name_length, ImGuiInputTextFlags_CallbackAlways, text_edit_callback, reinterpret_cast<void*>(this));
        //@@TODO @@MAYBE: Give the option to enable and disable entity
        ImGui::Text("%s", (m_enabled ? "Enabled" : "Disabled")); ImGui::SameLine(100); ImGui::Text("%s", (m_in_relevant_chunk ? "Relevant" : "Not Relevant"));

        for (auto& c : m_components) {
            c.second->draw_debug_gui();
        }
    }

    ImGui::PopID();

}

json Entity::to_json() {
    auto content = json{};

    ::to_json(content, *this);

    auto components = json{};

    for (const auto& c : m_components) {
        components[c.second->get_component_type_name()] = c.second->to_json();
    }

    content["Components"] = components;

    return content;
}

void Entity::load_json(const json & j){
    auto content = j;
    from_json(content, *this);
    auto components = content["Components"];
    for (const auto& c : m_components) {
        c.second->load_json(components[c.second->get_component_type_name()]);
    }
}
