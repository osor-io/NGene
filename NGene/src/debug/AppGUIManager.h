#pragma once

#include <Manager.h>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>


class AppGUIManager : public Manager<AppGUIManager> {
    friend class CRSP<AppGUIManager>;
private:
    AppGUIManager();
    ~AppGUIManager();

public:

    void start_up() override;
    void shut_down() override;

    void drawCornerOverlayDebugInfo();
    void drawEntityComponentEditor();

    void drawGUI();

private:
    ImFont * m_font;
};
