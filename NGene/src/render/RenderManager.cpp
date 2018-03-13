#include "RenderManager.h"
#include "../debug/AppGUIManager.h"
#include "../time/TimeManager.h"


RenderManager::RenderManager() {}


RenderManager::~RenderManager() {}


void RenderManager::startUp() {
    /*
    We first initialize the window and the debug GUI.
    */
    m_renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(1200, 800), "NGene");
    m_renderWindow->setFramerateLimit(60);
     
    ImGui::SFML::Init(*m_renderWindow);
}


void RenderManager::shutDown() {

    /*
    In reverse order of creation first we shutdown the debug GUI
    and then we release the render window.
    */
    ImGui::SFML::Shutdown();
    m_renderWindow.reset();

}

bool RenderManager::isWindowOpen() const {
    return m_renderWindow->isOpen();
}

sf::RenderTarget* RenderManager::getMainRenderTarget() {
    return m_renderWindow.get();
}

void RenderManager::beginFrame() {
    sf::Event event;
    while (m_renderWindow->pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed) {
            m_renderWindow->close();
            return;
        }
    }
    m_renderWindow->clear(m_clearColor);
}

void RenderManager::endFrame() {

    /*
    Now we set up everything we want to render for the debug GUI

    We must create all widgets between Update and Render
    */
    ImGui::SFML::Update(*m_renderWindow, DELTA_TIME);

    //@@TODO: Call here all the ImGui functions
    AppGUIManager::get().drawGUI();


    /*
    Now we render overlayed things such as the debug GUI
    */
    ImGui::SFML::Render(*m_renderWindow);


    /*
    And finally we display the image
    */
    m_renderWindow->display();
    TimeManager::get().endOfFrame();
}
