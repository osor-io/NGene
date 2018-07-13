#pragma once
#include <Manager.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>


namespace config {

	namespace opengl {
		constexpr auto depthBits = 24;			// Bits of depth buffer
		constexpr auto stencilBits = 8;			// Bits of stencil buffer
		constexpr auto antialiasingLevel = 4;	// MSAA samples
		constexpr auto majorVersion = 3;		// Version of OpenGL (3 should be widely supported)
		constexpr auto minorVersion = 0;
	}
	
}

class WindowManager : public Manager<WindowManager> {
    friend class CRSP<WindowManager>;
public:
    WindowManager();
    ~WindowManager();


    void start_up() override;
    void shut_down() override;

    void fill_events();

    sf::RenderWindow* get_window_render_target();
    bool is_window_open() const;
    const std::vector<sf::Event>& get_frame_events() const;

	sf::WindowHandle get_window_handle() const;

private:
    std::vector<sf::Event> m_frame_events;
    std::unique_ptr<sf::RenderWindow> m_window;

};

