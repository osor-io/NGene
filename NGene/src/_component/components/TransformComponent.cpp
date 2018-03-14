#include "TransformComponent.h"
#include "../lua/LuaManager.h"
#include <Debug.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include "../_entity/Entity.h"
#include <sstream>
#include <ImGuizmo.h>

TransformComponent::TransformComponent(EntityId id) : ComponentTemplate(id, std::type_index(typeid(TransformComponent))) {
}


TransformComponent::TransformComponent(EntityId id, const sol::table& table) : ComponentTemplate(id, std::type_index(typeid(TransformComponent))) {


    sol::object value_x = table["x"];
    sol::object value_y = table["y"];
    assert(value_x.valid() && value_y.valid());

    m_position = sf::Vector2f(value_x.as<float>(), value_y.as<float>());

}


TransformComponent::~TransformComponent() {}

void TransformComponent::drawComponentInspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 90), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculateShowname().c_str(), &m_guiOpen);
    ImGui::Text("Position: "); ImGui::SameLine(100); ImGui::DragFloat2("##Position", &(m_position.x), 1.0f, config::minPosition.x, config::maxPosition.x);

    /*
    //To Check the window size and adjust default sizes:
    ImGui::Text("Window Size: (%f, %f)", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    */
    ImGui::End();

}



json TransformComponent::toJson() {

    auto j = json{};

    j["position"]["x"] = m_position.x;
    j["position"]["y"] = m_position.y;

    return j;
}

void TransformComponent::loadJson(const json& j) {

    auto v = sf::Vector2f{};

    v.x = j["position"]["x"];
    v.y = j["position"]["y"];

    setPosition(v);
}

sf::Vector2f TransformComponent::getPosition() const {
    return m_position;
}

sf::Vector2f & TransformComponent::getPositionRef() {
    return m_position;
}


void TransformComponent::setPosition(const sf::Vector2f & position) {
    m_position = position;
}


LuaVector2f& TransformComponent::luaGetPosition() {
    return (*reinterpret_cast<LuaVector2f*>(&m_position));
}


void TransformComponent::luaSetPosition(const LuaVector2f & position) {
    m_position = LuaVector2f::toSf(position);
}

void TransformComponent::exposeToLua() {

    LUA.new_usertype<LuaVector2f>("Vector2f",


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "x", &LuaVector2f::x,
        "y", &LuaVector2f::y

        );

    LUA.new_usertype<TransformComponent>(meta::getName<TransformComponent>(),


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "position", sol::property(&TransformComponent::luaGetPosition, &TransformComponent::luaSetPosition)

        );
}



