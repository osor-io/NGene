#include "CollisionComponent.h"
#include "../lua/LuaManager.h"

#define COMPONENT_TYPE CollisionComponent
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

        {
            sol::object value = table["type"];
            assert(value.valid());
            m_type = value.as<ColliderType>();
        }

}


DTOR(COMPONENT_TYPE)() {
}


json COMPONENT_TYPE::to_json() {
    auto j = json{};

    j["extent"]["x"] = m_extent.x;
    j["extent"]["y"] = m_extent.y;

    j["offset"]["x"] = m_offset.x;
    j["offset"]["y"] = m_offset.y;

    /*
    I don't want to change wether it is dynamic
    or not at runtime or in the editor, it is something
    inherent to the entity or the collider so it
    should be changed only when defining the type.

    //j["dynamic"] = m_dynamic;

    */


    return j;
}

void COMPONENT_TYPE::load_json(const json& j) {
    m_extent.x = j["extent"]["x"];
    m_extent.y = j["extent"]["y"];

    m_offset.x = j["offset"]["x"];
    m_offset.y = j["offset"]["y"];

    /*
    I don't want to change wether it is dynamic
    or not at runtime or in the editor, it is something
    inherent to the entity or the collider so it
    should be changed only when defining the type.

    //m_dynamic = j["dynamic"];

    */
}

/*
Only for debug purposes, to draw the extent where it should be
*/
#include "../_entity/EntityManager.h"
#include "../_component/components/TransformComponent.h"
#include "../render/RenderManager.h"

void COMPONENT_TYPE::draw_component_inspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculate_showname().c_str(), &m_gui_open);

    ImGui::Text("Extent: "); ImGui::SameLine(100); ImGui::DragFloat2("##Extent", &(m_extent.x));
    ImGui::Text("Offset: "); ImGui::SameLine(100); ImGui::DragFloat2("##Offset", &(m_offset.x));
   
    ImGui::Text("%s", ( m_type == ColliderType::TERRAIN ? "Terrain" :
                        m_type == ColliderType::MOVING_OBJECT ? "Moving Object"  :
                        "Unknown"));

    draw_rect();

    ImGui::End();
}

#define KEY(x) #x, ColliderType:: ## x

#define REGISTER_METHOD(method) #method , &PhraseComponent::method

void COMPONENT_TYPE::expose_to_lua()
{


    LUA.new_enum("ColliderType",
        KEY(TERRAIN),
        KEY(MOVING_OBJECT)
    );

    LUA.new_usertype<COMPONENT_TYPE>(STRINGIFY(COMPONENT_TYPE),


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "extent", &COMPONENT_TYPE::m_extent,
        "offset", &COMPONENT_TYPE::m_offset

        );

}

void CollisionComponent::draw_rect() {
    const auto transform = EntityManager::get().get_entity(m_parent_id)->get_component<TransformComponent>();

    if (transform) {

        auto center = transform->get_position() + m_offset;

        auto one = RenderManager::get().get_main_render_target()->mapCoordsToPixel(
            sf::Vector2f((center.x - m_extent.x), (center.y - m_extent.y))
        );
        auto two = RenderManager::get().get_main_render_target()->mapCoordsToPixel(
            sf::Vector2f((center.x + m_extent.x), (center.y - m_extent.y))
        );
        auto three = RenderManager::get().get_main_render_target()->mapCoordsToPixel(
            sf::Vector2f((center.x + m_extent.x), (center.y + m_extent.y))
        );
        auto four = RenderManager::get().get_main_render_target()->mapCoordsToPixel(
            sf::Vector2f((center.x - m_extent.x), (center.y + m_extent.y))
        );

        auto draw_list = ImGui::GetWindowDrawList();
        draw_list->PushClipRectFullScreen();
        draw_list->AddQuad(
            ImVec2(one.x, one.y),
            ImVec2(two.x, two.y),
            ImVec2(three.x, three.y),
            ImVec2(four.x, four.y),
            ImGui::GetColorU32((ImVec4)ImColor(255, 255, 0, 150)),
            2.0f
        );
        draw_list->PopClipRect();
    }
}
