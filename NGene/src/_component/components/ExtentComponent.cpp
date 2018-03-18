#include "ExtentComponent.h"
#include "../lua/LuaManager.h"

#define COMPONENT_TYPE ExtentComponent
#define CTOR(x) x##::##x
#define DTOR(x) x##::##~##x
#define STRINGIFY(s) #s

CTOR(COMPONENT_TYPE)(EntityId id) : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {}

CTOR(COMPONENT_TYPE)(EntityId id, const sol::table& table)
    : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {
    
        {
            sol::object value_x = table["extentX"];
            sol::object value_y = table["extentY"];
            assert(value_x.valid() && value_y.valid());
            m_extent = sf::Vector2f(value_x.as<float>(), value_y.as<float>());
        }

        {
            sol::object value_x = table["offsetX"];
            sol::object value_y = table["offsetY"];
            assert(value_x.valid() && value_y.valid());
            m_offset = sf::Vector2f(value_x.as<float>(), value_y.as<float>());
        }

}


DTOR(COMPONENT_TYPE)() {
}


json COMPONENT_TYPE::to_json() {
    auto j = json{};

    j["extent"]["x"] = m_extent.x;
    j["extent"]["y"] = m_extent.y;

    return j;
}

void COMPONENT_TYPE::load_json(const json& j) {
    m_extent.x = j["extent"]["x"];
    m_extent.y = j["extent"]["y"];
}

/*
Only for debug purposes, to draw the extent where it should be
*/
#include "../_entity/EntityManager.h"
#include "../_component/components/TransformComponent.h"

void COMPONENT_TYPE::draw_component_inspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculate_showname().c_str(), &m_gui_open);

    ImGui::Text("Extent: "); ImGui::SameLine(100); ImGui::DragFloat2("##Extent", &(m_extent.x));
    ImGui::Text("Offset: "); ImGui::SameLine(100); ImGui::DragFloat2("##Offset", &(m_offset.x));


    const auto transform = EntityManager::get().get_entity(m_parent_id)->get_component<TransformComponent>();

    if (transform) {

        auto center = transform->get_position();

        center += m_offset;

        auto draw_list = ImGui::GetWindowDrawList();
        draw_list->PushClipRectFullScreen();
        draw_list->AddQuad(
            ImVec2((center.x - m_extent.x), (center.y - m_extent.y)),
            ImVec2((center.x + m_extent.x), (center.y - m_extent.y)),
            ImVec2((center.x + m_extent.x), (center.y + m_extent.y)),
            ImVec2((center.x - m_extent.x), (center.y + m_extent.y)),
            ImGui::GetColorU32((ImVec4)ImColor(0, 255, 0, 150)),
            2.0f
        );
        draw_list->PopClipRect();

    }


    ImGui::End();

}

#define REGISTER_METHOD(method) #method , &PhraseComponent::method

void COMPONENT_TYPE::expose_to_lua()
{

    LUA.new_usertype<COMPONENT_TYPE>(STRINGIFY(COMPONENT_TYPE),


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "extent", &COMPONENT_TYPE::m_extent

        );

}