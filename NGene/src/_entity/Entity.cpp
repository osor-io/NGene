#include "Entity.h"
#include "../lua/LuaManager.h"
#include <sstream>
#include <Debug.h>

Entity::Entity(EntityId id, std::string&& name, const std::string& type) : m_id(id), m_type(type) {
    setName(name);
    generateShowName();
}


Entity::~Entity() {
}

EntityId Entity::getId() const
{
    return m_id;
}

std::string Entity::getType() const {
    return m_type;
}

void Entity::setType(const std::string& type) {
    m_type = type;
}

void Entity::setType(std::string&& type) {
    m_type = std::move(type);
}


std::string Entity::getName() const {
    return m_name;
}

const std::string & Entity::getNameRef() const {
    return m_name;
}

void Entity::setName(const std::string& name) {
    m_name = name;
    generateShowName();
}

void Entity::setEnabled(bool enabled) {
    m_enabled = enabled;
}

std::string Entity::getShowName() {
    return m_showName;
}

void Entity::generateShowName() {

    auto ss = std::stringstream{};
    ss << "[" << m_id << "] " << m_type << ": " << m_name << " (" << m_components.size() << ")";
    m_showName = ss.str();

}

bool Entity::isEnabled() {
    return m_enabled;
}

#include "../_component/components/BehaviourComponent.h"
#include "../_component/components/TransformComponent.h"
#include "../_component/components/SpriteComponent.h"
#include "../_component/components/PhraseComponent.h"

#define GET_COMPONENT_NAME(type) #type
#define MAKE_STRING(s) #s
#define REGISTER_GET_COMPONENT(type) "get" #type , &Entity::getComponent< ## type ## >

void Entity::exposeToLua() {

    /*
    Here we expose the members of the components
    */
    BehaviourComponent::expose_to_lua();
    TransformComponent::expose_to_lua();
    SpriteComponent::expose_to_lua();
    PhraseComponent::expose_to_lua();

    /*
    And here a way to access them from the entity that contains them
    */
    LUA.new_usertype<Entity>("Entity",

        REGISTER_GET_COMPONENT(BehaviourComponent),
        REGISTER_GET_COMPONENT(TransformComponent),
        REGISTER_GET_COMPONENT(SpriteComponent),
        REGISTER_GET_COMPONENT(PhraseComponent),

        "id", sol::readonly_property(&Entity::getId),
        "type", sol::readonly_property(&Entity::getType),
        "enabled", sol::property(&Entity::isEnabled, &Entity::setEnabled),
        "name", sol::property(&Entity::getName, &Entity::setName)

        );

}


int textEditCallback(ImGuiTextEditCallbackData *data) {
    auto entity = reinterpret_cast<Entity*>(data->UserData);
    entity->setName(std::string(data->Buf));
    entity->generateShowName();
    entity->m_changedHeader = true;
    return 0;
}


void Entity::drawDebugGUI() {

    auto rustyPalette = config::getRustyPalette();

    char name_arr[config::max_name_length];

    strncpy(name_arr, m_name.c_str(), sizeof(name_arr));
    name_arr[sizeof(name_arr) - 1] = 0;

    auto open = ImGui::CollapsingHeader(name_arr);

    if (open || m_changedHeader) {

        m_changedHeader = false;

        ImGui::Text("Id: %d", m_id); ImGui::SameLine(100); ImGui::Text("Type: %s", m_type.c_str());
        ImGui::Text("Name: "); ImGui::SameLine(100); ImGui::InputText("##Name", name_arr, config::max_name_length, ImGuiInputTextFlags_CallbackAlways, textEditCallback, reinterpret_cast<void*>(this));

        for (auto& c : m_components) {
            c.second->draw_debug_gui();
        }
    }

}

json Entity::toJson() {
    auto content = json{};

    to_json(content, *this);

    auto components = json{};

    for (const auto& c : m_components) {
        components[c.second->get_component_type_name()] = c.second->to_json();
    }

    content["Components"] = components;

    return content;
}

void Entity::loadJson(const json & j){
    auto content = j;
    from_json(content, *this);
    auto components = content["Components"];
    for (const auto& c : m_components) {
        c.second->load_json(components[c.second->get_component_type_name()]);
    }
}
