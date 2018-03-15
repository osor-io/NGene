#include "AppGUIManager.h"
#include "../time/TimeManager.h"
#include "../_entity/EntityManager.h"
#include <Debug.h>
#include <../config/Config.h>
#include "./cafe.inl"

AppGUIManager::AppGUIManager() {}


AppGUIManager::~AppGUIManager() {}


void AppGUIManager::start_up() {

    ImGuiStyle * style = &ImGui::GetStyle();

    auto rustyPalette = config::getRustyPalette();


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
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = rustyPalette[0];
    style->Colors[ImGuiCol_TextDisabled] = rustyPalette[1];
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.80f);
    style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = rustyPalette[1];
    style->Colors[ImGuiCol_BorderShadow] = rustyPalette[0];
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = rustyPalette[7];  //ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = rustyPalette[8]; //ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive] = rustyPalette[3]; //ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = rustyPalette[0];
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = rustyPalette[1];
    style->Colors[ImGuiCol_ScrollbarGrabActive] = rustyPalette[2];
    style->Colors[ImGuiCol_CheckMark] = rustyPalette[0];
    style->Colors[ImGuiCol_SliderGrab] = rustyPalette[0];
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = rustyPalette[2];
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = rustyPalette[0];
    style->Colors[ImGuiCol_ResizeGripHovered] = rustyPalette[1];
    style->Colors[ImGuiCol_ResizeGripActive] = rustyPalette[2];
    style->Colors[ImGuiCol_CloseButton] = rustyPalette[0];//ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    style->Colors[ImGuiCol_CloseButtonHovered] = rustyPalette[5];//ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    style->Colors[ImGuiCol_CloseButtonActive] = rustyPalette[7];//ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
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

void AppGUIManager::drawCornerOverlayDebugInfo() {
    bool open = true;
    const float DISTANCE = 10.0f;
    static int corner = 0;
    ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f)); // Transparent background
    if (ImGui::Begin("Debug Overlay", &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Mouse Position: (%6.1f,%6.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::Text("Frames Per Second: (%.1f)", (1.f / TimeManager::get().getDeltaTime().asSeconds()));
        ImGui::Text("Frame Time: (%d) ms", (TimeManager::get().getDeltaTime().asMilliseconds()));
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

void AppGUIManager::drawEntityComponentEditor() {

    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Entities & Components"))
    {
        ImGui::End();
        return;
    }


    for (auto key : EntityManager::get().getEntityKeys()) {
        EntityManager::get().getEntity(key)->drawDebugGUI();
    }

    ImGui::End();


}

void AppGUIManager::drawGUI() {

    ImGui::PushFont(m_font);

    ImGui::ShowDemoWindow();

    drawCornerOverlayDebugInfo();
    drawEntityComponentEditor();

    ImGui::PopFont();

}
