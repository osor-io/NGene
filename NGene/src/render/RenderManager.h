#pragma once
#include <Manager.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>


class RenderManager : public Manager<RenderManager> {
    friend class CRSP<RenderManager>;
private:
    RenderManager();
    ~RenderManager();

public:

    void start_up() override;
    void shut_down() override;

    sf::RenderTarget* get_main_render_target();

    void begin_frame();
    void end_frame();

private:

    sf::Color m_clear_color{ sf::Color::Black };
    sf::RenderWindow* m_main_target;


};
