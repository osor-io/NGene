#include "RenderManager.h"
#include "../debug/AppGUIManager.h"
#include "../time/TimeManager.h"


RenderManager::RenderManager() {}


RenderManager::~RenderManager() {}


void RenderManager::start_up() {
    /*
    We first initialize the window and the debug GUI.
    */
    m_render_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1200, 800), "NGene");
    m_render_window->setFramerateLimit(60);
     
    ImGui::SFML::Init(*m_render_window);
}


void RenderManager::shut_down() {

    /*
    In reverse order of creation first we shutdown the debug GUI
    and then we release the render window.
    */
    ImGui::SFML::Shutdown();
    m_render_window.reset();

}

bool RenderManager::is_window_open() const {
    return m_render_window->isOpen();
}

sf::RenderTarget* RenderManager::get_main_render_target() {
    return m_render_window.get();
}

void RenderManager::begin_frame() {
    auto event = sf::Event{};
    while (m_render_window->pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed) {
            m_render_window->close();
            return;
        }
    }
    m_render_window->clear(m_clear_color);
}

void RenderManager::end_frame() {

    /*
    Now we set up everything we want to render for the debug GUI

    We must create all widgets between Update and Render
    */
    ImGui::SFML::Update(*m_render_window, DELTA_TIME);

    AppGUIManager::get().draw_gui();


    /*
    Now we render overlayed things such as the debug GUI
    */
    ImGui::SFML::Render(*m_render_window);


    /*
    And finally we display the image
    */
    m_render_window->display();
    TimeManager::get().end_of_frame();
}
