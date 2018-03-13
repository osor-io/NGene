#include "Entity.h"
#include "../lua/LuaManager.h"
#include <sstream>

Entity::Entity(EntityId id) : m_id(id) {
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

void Entity::setEnabled(bool enabled) {
    m_enabled = enabled;
}

std::string Entity::getShowName() {
    if (m_showName.size() == 0) {
        auto ss = std::stringstream{};
        ss << "[" << m_id << "] " << m_type << " (" << m_components.size() << ")";
        m_showName = ss.str();
    }
    return m_showName;
}

bool Entity::isEnabled() {
    return m_enabled;
}

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
    TransformComponent::exposeToLua();
    SpriteComponent::exposeToLua();
    PhraseComponent::exposeToLua();

    /*
    And here a way to access them from the entity that contains them
    */
    LUA.new_usertype<Entity>("Entity",

        /*
        We don't allow constructors to be called from Lua since
        we want the EntityManager to act as a factory for all the entities
        */

        // Members
        /*
        Add here all the components that we want to expose to Lua via "getComponentName()" functions
        */
        REGISTER_GET_COMPONENT(TransformComponent),
        REGISTER_GET_COMPONENT(SpriteComponent),
        REGISTER_GET_COMPONENT(PhraseComponent),

        "id", sol::readonly_property(&Entity::getId),
        "type", sol::readonly_property(&Entity::getType),
        "enabled", sol::property(&Entity::isEnabled, &Entity::setEnabled)

        );

}

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include "../config/Config.h"

void Entity::drawDebugGUI() {


    //ImGui::PushID(this);
    auto rustyPalette = config::getRustyPalette();

    auto open = ImGui::CollapsingHeader(getShowName().c_str());
   
    if (open)
    {

        ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());

        for (auto& c : m_components) {
            c.second->drawDebugGUI();
        }

        ImGui::PopItemWidth();
    }

    //ImGui::PopID();


    /*

    //Implementation for tree view

    ImGui::PushID(getId());                      // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
    bool node_open = ImGui::TreeNode("Entity", "[%u] %s", getId(), this->getType().c_str());
    ImGui::NextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Entity with the id [%d] of type [%s] with [%d] components", getId(), getType().c_str(), m_components.size());
    ImGui::NextColumn();
    if (node_open)
    {
        for (auto& c : m_components){

            auto component = c.second.get();

            component->drawDebugGUI();

        }
        ImGui::TreePop();
    }
    ImGui::PopID();
    */
}
