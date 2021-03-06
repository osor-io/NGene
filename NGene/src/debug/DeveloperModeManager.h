#pragma once

#include <Manager.h>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>


class DeveloperModeManager : public Manager<DeveloperModeManager> {
    friend class CRSP<DeveloperModeManager>;
private:
    DeveloperModeManager();
    ~DeveloperModeManager();

public:

    void start_up() override;
    void shut_down() override;

    void draw_corner_overlay_debug_info();
    void draw_entity_component_editor();

	void draw_debug_overlay();
	
    void draw_gui();

    void set_debug_open(bool);
    bool is_debug_open() const;

    bool show_collisions() const;

private:
    ImFont * m_font;
    bool m_debug_open{ true };


    bool m_show_entities_components{ true };
    bool m_show_imgui_demo{ false };
    bool m_show_debug_overlay{ true };
    bool m_show_chunks{ false };
    bool m_show_collisions{ false };
    bool m_show_chunk_configuration{ false };

};
