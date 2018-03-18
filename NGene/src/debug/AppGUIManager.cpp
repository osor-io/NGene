#include "AppGUIManager.h"
#include "../time/TimeManager.h"
#include "../_entity/EntityManager.h"
#include <Debug.h>
#include <../config/Config.h>
#include "./cafe.inl"
#include "../utils/File.h"
#include "../physics/ChunkManager.h"

AppGUIManager::AppGUIManager() {}

AppGUIManager::~AppGUIManager() {}

void AppGUIManager::start_up() {

    ImGuiStyle * style = &ImGui::GetStyle();

    auto palette = config::get_rusty_palette();


    style->WindowPadding = ImVec2(15, 15);
    style->WindowRounding = 5.0f;
    style->FramePadding = ImVec2(10, 5);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = palette[0];
    style->Colors[ImGuiCol_TextDisabled] = palette[1];
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.80f);
    style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = palette[1];
    style->Colors[ImGuiCol_BorderShadow] = palette[0];
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = palette[7];  //ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = palette[8]; //ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive] = palette[3]; //ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = palette[0];
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = palette[1];
    style->Colors[ImGuiCol_ScrollbarGrabActive] = palette[2];
    style->Colors[ImGuiCol_CheckMark] = palette[0];
    style->Colors[ImGuiCol_SliderGrab] = palette[0];
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = palette[2];
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = palette[0];
    style->Colors[ImGuiCol_ResizeGripHovered] = palette[1];
    style->Colors[ImGuiCol_ResizeGripActive] = palette[2];
    style->Colors[ImGuiCol_CloseButton] = palette[0];//ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    style->Colors[ImGuiCol_CloseButtonHovered] = palette[5];//ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    style->Colors[ImGuiCol_CloseButtonActive] = palette[7];//ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);


    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    m_font = io.Fonts->AddFontFromMemoryCompressedTTF(cafe_compressed_data, cafe_compressed_size, 18.0f);
    ImGui::SFML::UpdateFontTexture();



}

void AppGUIManager::shut_down() {


}

void AppGUIManager::draw_corner_overlay_debug_info() {
    bool open = true;
    const float DISTANCE = 10.0f;
    static int corner = 2;
    auto window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
    auto window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f)); // Transparent background
    if (ImGui::Begin("Debug Overlay", &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
    {
        auto mouse_pos = ImGui::GetIO().MousePos;

        auto chunk = ChunkManager::get().get_chunk_from_position(mouse_pos.x, mouse_pos.y);

        ImGui::Text("Mouse Position: (%6.1f,%6.1f)", mouse_pos.x, mouse_pos.y);
        ImGui::Text("Chunk: (%3d,%3d)", chunk.first, chunk.second);
        ImGui::Text("Frames Per Second: (%.1f)", (1.f / TimeManager::get().get_delta_time().asSeconds()));
        ImGui::Text("Frame Time: (%d) ms", (TimeManager::get().get_delta_time().asMilliseconds()));
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            ImGui::EndPopup();
        }
        ImGui::End();
    }
    ImGui::PopStyleColor();
}

void AppGUIManager::draw_entity_component_editor() {

    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Entities & Components"))
    {
        ImGui::End();
        return;
    }


    for (auto key : EntityManager::get().get_entity_keys()) {
        EntityManager::get().get_entity(key)->draw_debug_gui();
    }

    ImGui::End();


}

void AppGUIManager::set_debug_open(bool open) {
    m_debug_open = open;
}

bool AppGUIManager::is_debug_open() const {
    return m_debug_open;
}

void AppGUIManager::draw_gui() {

    if (m_debug_open) {

        ImGui::PushFont(m_font);


        ImGui::BeginMainMenuBar();

        if (ImGui::BeginMenu("File")) {

            if (ImGui::Button("New State")) {
                ImGui::OpenPopup("Creating New State");
            }
            if (ImGui::BeginPopupModal("Creating New State", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                static char loading_filename[32];
                ImGui::TextWrapped("Are you sure you want to cleate a new state?\n This will delete all unsaved changes in the current one.");
                if (ImGui::Button("Just do it!", ImVec2(120, 0))) {
                    EntityManager::get().clear_entities();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Alright, maybe no", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }

            if (ImGui::Button("Load State")) {
                ImGui::OpenPopup("Loading State");
            }
            if (ImGui::BeginPopupModal("Loading State", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                static char loading_filename[32];
                ImGui::Text("Filename to load: "); ImGui::SameLine(150); ImGui::InputText("##Filename", loading_filename, 32);
                if (ImGui::Button("OK", ImVec2(120, 0))) {
                    EntityManager::get().clear_and_load_entities_from_file(loading_filename);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }

            if (ImGui::Button("Save State")) {
                ImGui::OpenPopup("Saving State");
            }
            if (ImGui::BeginPopupModal("Saving State", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                static char saving_filename[32];
                ImGui::Text("Filename to save: "); ImGui::SameLine(150); ImGui::InputText("##Filename", saving_filename, 32);
                if (ImGui::Button("OK", ImVec2(120, 0))) {
                    EntityManager::get().serialize_entities_to_file(saving_filename);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            ImGui::Checkbox("Entities & Components", &m_show_entities_components);
            ImGui::Checkbox("Debug Overlay", &m_show_debug_overlay);
            ImGui::Checkbox("Chunks", &m_show_chunks);
            ImGui::Checkbox("ImGui Demo", &m_show_imgui_demo);
            ImGui::EndMenu();
        }

        /*Put here overlay things*/
        if (m_show_chunks) ChunkManager::get().draw_debug_chunks();

        ImGui::EndMainMenuBar();


        if (m_show_entities_components) draw_entity_component_editor();
        if (m_show_imgui_demo) ImGui::ShowDemoWindow();
        if (m_show_debug_overlay) draw_corner_overlay_debug_info();


        ImGui::PopFont();

    }

}
