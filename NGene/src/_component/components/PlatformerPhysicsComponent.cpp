#include "PlatformerPhysicsComponent.h"
#include "../lua/LuaManager.h"

#define COMPONENT_TYPE PlatformerPhysicsComponent
#define CTOR(x) x##::##x
#define DTOR(x) x##::##~##x
#define STRINGIFY(s) #s

CTOR(COMPONENT_TYPE)(EntityId id) : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {}

CTOR(COMPONENT_TYPE)(EntityId id, const sol::table& table)
    : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {

        {
            sol::object value = table["maxFootSpeed"];
            assert(value.valid());
            m_max_foot_speed = value.as<float>();
        }

        {
            sol::object value = table["distanceToPeak"];
            assert(value.valid());
            m_horizontal_distance_to_peak = value.as<float>();
        }

        {
            sol::object value = table["jumpHeight"];
            assert(value.valid());
            m_jump_height = value.as<float>();
        }

        {
            sol::object value = table["downwardsGravityScale"];
            assert(value.valid());
            m_downwards_gravity_scale = value.as<float>();
        }

        m_need_recalculation = true;
}


DTOR(COMPONENT_TYPE)() {
}


json COMPONENT_TYPE::to_json() {
    auto j = json{};

    j["maxFootSpeed"] = m_max_foot_speed;
    j["distanceToPeak"] = m_horizontal_distance_to_peak;
    j["jumpHeight"] = m_jump_height;
    j["downwardsGravityScale"] = m_downwards_gravity_scale;

    return j;
}

void COMPONENT_TYPE::load_json(const json& j) {
    m_max_foot_speed = j["maxFootSpeed"];
    m_horizontal_distance_to_peak = j["distanceToPeak"];
    m_jump_height = j["jumpHeight"];
    m_downwards_gravity_scale = j["downwardsGravityScale"];
    m_need_recalculation = true;
}

void COMPONENT_TYPE::draw_component_inspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculate_showname().c_str(), &m_gui_open);

    ImGui::Text("Max Foot Speed: "); ImGui::SameLine(230);
    if (ImGui::InputFloat("##MaxFootSpeed", &m_max_foot_speed)) m_need_recalculation = true;
    
    ImGui::Text("Distance To Peak: "); ImGui::SameLine(230);
    if (ImGui::InputFloat("##DistanceToPeak", &m_horizontal_distance_to_peak)) m_need_recalculation = true;

    ImGui::Text("Jump Height: "); ImGui::SameLine(230);
    if (ImGui::InputFloat("##JumpHeight", &m_jump_height)) m_need_recalculation = true;
    
    ImGui::Text("Downwards Gravity Scale: "); ImGui::SameLine(230); 
    if (ImGui::InputFloat("##DownwardsGravityScale", &m_downwards_gravity_scale)) m_need_recalculation = true;

    ImGui::Separator();

    ImGui::Text("State: %s", m_grounded ? "Grounded" : "Not Grounded");
    ImGui::Text("Current Velocity: (%f, %f)", m_current_velocity.x, m_current_velocity.y);
    ImGui::Text("Current Gravity: (%f, %f)", m_our_gravity.x, m_our_gravity.y);
    ImGui::Text("Current Downwards Gravity: (%f, %f)",
        m_our_gravity.x * m_downwards_gravity_scale,
        m_our_gravity.y * m_downwards_gravity_scale);
    ImGui::Text("Current Jump Initial Velocity: (%f, %f)", m_initial_jump_velocity.x, m_initial_jump_velocity.y);
    ImGui::Text("Current Max Velocity: (%f, %f)", m_max_velocity.x, m_max_velocity.y);


    ImGui::End();
}

#define REGISTER_METHOD(method) #method , &PhraseComponent::method

void COMPONENT_TYPE::expose_to_lua()
{

    LUA.new_enum("MovementDirection",
        "Right", INPUT_MOVEMENT_RIGHT,
        "Left", INPUT_MOVEMENT_LEFT,
        "None", INPUT_MOVEMENT_NONE
    );

    LUA.new_usertype<COMPONENT_TYPE>(STRINGIFY(COMPONENT_TYPE),

        "move", &COMPONENT_TYPE::move,
        "jump", &COMPONENT_TYPE::jump,

        "maxFootSpeed", sol::property(&COMPONENT_TYPE::get_max_foot_speed, &COMPONENT_TYPE::set_max_foot_speed),
        "jumpHeight", sol::property(&COMPONENT_TYPE::get_jump_height, &COMPONENT_TYPE::set_jump_height),
        "distanceToPeak", sol::property(&COMPONENT_TYPE::get_horizontal_distance_to_peak, &COMPONENT_TYPE::set_horizontal_distance_to_peak),
        "downwardsGravityScale", sol::property(&COMPONENT_TYPE::get_downwards_gravity_scale, &COMPONENT_TYPE::set_downwards_gravity_scale),

        "ourGravity", sol::readonly(&COMPONENT_TYPE::m_our_gravity),
        "initialJumpVelocity", sol::readonly(&COMPONENT_TYPE::m_initial_jump_velocity)

        );
}

void PlatformerPhysicsComponent::move(InputMovementDirection direction) {

    if (direction == INPUT_MOVEMENT_LEFT) {
        m_current_velocity.x = -m_max_foot_speed;
    }
    else if (direction == INPUT_MOVEMENT_RIGHT) {
        m_current_velocity.x = m_max_foot_speed;
    }
    else if (direction == INPUT_MOVEMENT_NONE) {
        m_current_velocity.x = 0;
    }
}

void PlatformerPhysicsComponent::jump() {

    /*
    @@NOTE

    Here we could add some sort of timing threshold to allow to buffer a jump
    even when we are not grounded.
    */

    if (m_grounded) {
        m_requested_jump = true;
    }

}
