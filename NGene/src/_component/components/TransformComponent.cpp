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

void TransformComponent::draw_component_inspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 90), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculate_showname().c_str(), &m_gui_open);
    ImGui::Text("Position: "); ImGui::SameLine(100); ImGui::DragFloat2("##Position", &(m_position.x), 1.0f, config::min_position.x, config::max_position.x);

    if (ImGui::IsWindowFocused()) {
        auto draw_list = ImGui::GetWindowDrawList();
        auto centre = ImVec2(m_position.x, m_position.y);
        auto color_center = ImGui::GetColorU32((ImVec4)ImColor(255, 0, 0));
        auto color_line = ImGui::GetColorU32((ImVec4)ImColor(0, 255, 0));
        draw_list->PushClipRectFullScreen();
        draw_list->AddCircle(centre, 9.0f, color_line, 12, 4.0f);
        draw_list->AddCircleFilled(centre, 8.0f, color_center);
        draw_list->PopClipRect();
    }

  
    ImGui::End();
}



json TransformComponent::to_json() {

    auto j = json{};

    j["position"]["x"] = m_position.x;
    j["position"]["y"] = m_position.y;

    return j;
}

void TransformComponent::load_json(const json& j) {

    auto v = sf::Vector2f{};

    v.x = j["position"]["x"];
    v.y = j["position"]["y"];

    set_position(v);
}

sf::Vector2f TransformComponent::get_position() const {
    return m_position;
}

sf::Vector2f & TransformComponent::get_position_ref() {
    return m_position;
}


void TransformComponent::set_position(const sf::Vector2f & position) {
    m_position = position;
}


LuaVector2f& TransformComponent::lua_get_position() {
    return (*reinterpret_cast<LuaVector2f*>(&m_position));
}


void TransformComponent::lua_set_position(const LuaVector2f & position) {
    m_position = LuaVector2f::to_sf(position);
}

void TransformComponent::expose_to_lua() {

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

    LUA.new_usertype<TransformComponent>(meta::get_name<TransformComponent>(),


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "position", sol::property(&TransformComponent::lua_get_position, &TransformComponent::lua_set_position)

        );
}



