#include "WindowManager.h"
#include "../time/TimeManager.h"



WindowManager::WindowManager()
{
}


WindowManager::~WindowManager()
{
}

void WindowManager::start_up() {
    m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1200, 800), "NGene");
    m_window->setFramerateLimit(60);
}

void WindowManager::shut_down() {
    m_window.reset();
}

void WindowManager::fill_events() {
    auto event = sf::Event{};
    m_frame_events.clear();
    while (m_window->pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);
        m_frame_events.push_back(event);
        if (event.type == sf::Event::Closed) {
            m_window->close();
            return;
        }
    }
}


sf::RenderWindow * WindowManager::get_window_render_target(){
    return m_window.get();
}

bool WindowManager::is_window_open() const {
    return m_window->isOpen();
}

const std::vector<sf::Event>& WindowManager::get_frame_events() const {
    return m_frame_events;
}
