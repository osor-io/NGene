#include "RenderManager.h"
#include "../debug/DeveloperModeManager.h"
#include "../time/TimeManager.h"
#include "../window/WindowManager.h"

RenderManager::RenderManager() {}


RenderManager::~RenderManager() {}


void RenderManager::start_up() {
    /*
    We first initialize the window and the debug GUI.
    */
    m_main_target = WindowManager::get().get_window_render_target();

    if (!m_main_target) {
        std::cerr <<
            "The render manager needs a correct pointer to the render target, incorrect start_up order" <<
            std::endl;
        std::terminate();
    }

    ImGui::SFML::Init(*m_main_target);
}


void RenderManager::shut_down() {

    /*
    In reverse order of creation first we shutdown the debug GUI
    and then we release the render window.
    */
    ImGui::SFML::Shutdown();

}


sf::RenderTarget* RenderManager::get_main_render_target() {
    return m_main_target;
}

void RenderManager::begin_frame() {
    m_main_target->clear(m_clear_color);
}

void RenderManager::end_frame() {

    /*
    Now we set up everything we want to render for the debug GUI

    We must create all widgets between Update and Render
    */
    ImGui::SFML::Update(*m_main_target, DELTA_TIME);

    DeveloperModeManager::get().draw_gui();


    /*
    Now we render overlayed things such as the debug GUI
    */
    ImGui::SFML::Render(*m_main_target);


    /*
    And finally we display the image
    */
    m_main_target->display();
    TimeManager::get().end_of_frame();
}
