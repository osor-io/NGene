#pragma once
#include <Manager.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>


namespace config {

	namespace opengl {
		constexpr auto depth_bits = 24;			// Bits of depth buffer
		constexpr auto stencil_bits = 8;			// Bits of stencil buffer
		constexpr auto antialiasing_level = 4;	// MSAA samples
		constexpr auto major_version = 4;		// Version of OpenGL (3 should be widely supported)
		constexpr auto minor_version = 6;
	}

	namespace resolutions {
		constexpr auto internal_resolution_width = 256u;
		constexpr auto internal_resolution_height = 224u;

		constexpr auto external_resolution_multiplier = 4u;
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

	const sf::Window& get_window_ref() const { return *m_window; }

	void set_opengl_context_active() const { m_window->setActive(); }

private:
	std::vector<sf::Event> m_frame_events;
	std::unique_ptr<sf::RenderWindow> m_window;

};

