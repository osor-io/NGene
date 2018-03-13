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

    void startUp() override;
    void shutDown() override;

    void drawCornerOverlayDebugInfo();
    void drawEntityComponentEditor();

    void drawGUI();

private:
    ImFont * m_font;
};
