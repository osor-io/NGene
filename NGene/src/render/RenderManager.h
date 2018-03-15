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

    bool isWindowOpen() const;
    sf::RenderTarget* getMainRenderTarget();

    void beginFrame();
    void endFrame();

private:

    std::unique_ptr<sf::RenderWindow> m_renderWindow;
    sf::Color m_clearColor{ sf::Color::Black };

};
