#include "WindowManager.h"
#include "../time/TimeManager.h"



WindowManager::WindowManager() {}

WindowManager::~WindowManager() {}

void WindowManager::start_up() {


	sf::ContextSettings settings;
	settings.depthBits = config::opengl::depth_bits;
	settings.stencilBits = config::opengl::stencil_bits;
	settings.antialiasingLevel = config::opengl::antialiasing_level;
	settings.majorVersion = config::opengl::major_version;
	settings.minorVersion = config::opengl::minor_version;

	auto style = sf::Style::Titlebar | sf::Style::Close;

	auto external_resolution_width = config::resolutions::internal_resolution_width * config::resolutions::external_resolution_multiplier;
	auto external_resolution_height = config::resolutions::internal_resolution_height * config::resolutions::external_resolution_multiplier;

	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(external_resolution_width, external_resolution_height), "NGene", style, settings);

	/*
	@@TODO @@NOTE @@MAYBE
	I'm still not sure about how to optimally configure the game's framerate, for the
	time being I will leave it with just VSync enabled since just limiting the framerate
	with the line shown below seems to cause some variable frame rates creating somewhat
	noticeable stutter. Maybe it is caused by something else?

	m_window->setFramerateLimit(60);

	*/

	m_window->setVerticalSyncEnabled(true);
}

void WindowManager::shut_down() {
	m_window.reset();
}


#include <SFML/OpenGL.hpp>

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
		else if (event.type == sf::Event::Resized) {
			glViewport(0, 0, event.size.width, event.size.height);
		}
	}
}

sf::RenderWindow * WindowManager::get_window_render_target() {
	return m_window.get();
}

bool WindowManager::is_window_open() const {
	return m_window->isOpen();
}

const std::vector<sf::Event>& WindowManager::get_frame_events() const {
	return m_frame_events;
}

sf::WindowHandle WindowManager::get_window_handle() const {
	return m_window->getSystemHandle();
}
