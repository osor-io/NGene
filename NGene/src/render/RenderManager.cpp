#include "RenderManager.h"
#include "../debug/DeveloperModeManager.h"
#include "../time/TimeManager.h"
#include "../window/WindowManager.h"

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

RenderManager::RenderManager() {}

RenderManager::~RenderManager() {}

void RenderManager::start_up() {

	/*
	We first initialize the window and the debug GUI.
	*/
	m_window_target = WindowManager::get().get_window_render_target();


	/*
	Init ImGui
	*/
	ImGui::SFML::Init(*m_window_target);


	/*
	We set up the opengl context
	*/
	m_window_target->setActive(true);
	glewInit();

	m_crt_renderer = std::make_unique<CRTRenderer>(*m_window_target);

	/*
	We create the lower resolution render target
	*/
	m_main_target = std::make_unique<sf::RenderTexture>();
	m_main_target->create(config::resolutions::internal_resolution_width, config::resolutions::internal_resolution_height);
	m_main_target->setSmooth(false);
	m_main_target->setRepeated(false);

	m_current_texture_scale.x = config::resolutions::external_resolution_multiplier;
	m_current_texture_scale.y = config::resolutions::external_resolution_multiplier;

	if (!m_main_target) {
		std::cerr <<
			"The render manager needs a correct pointer to the render target, incorrect start_up order" <<
			std::endl;
		std::terminate();
	}
}

void RenderManager::shut_down() {

	m_main_target.release();
	m_crt_renderer.release();

	/*
	In reverse order of creation first we shutdown the debug GUI
	and then we release the render window.
	*/
	ImGui::SFML::Shutdown();

}

sf::RenderTarget* RenderManager::get_main_render_target() {
	return m_main_target.get();
}

void RenderManager::set_main_target_params(sf::Vector2f center, float zoom) {

	m_current_center = center;
	m_current_zoom = zoom;

	auto view = m_main_target->getView();
	auto size = m_main_target->getDefaultView().getSize();
	size *= 1.0f / zoom;
	view.setCenter(center);
	view.setSize(size);
	m_main_target->setView(view);
}

sf::Vector2f RenderManager::get_current_center() const {
	return m_current_center;
}

float RenderManager::get_current_zoom() const {
	return m_current_zoom;
}

void RenderManager::draw_debug_gui() {

	if (ImGui::BeginMenu("Render")) {

		{
			ImGui::Checkbox("Simulate CRT", &m_simulating_crt);
		}
		ImGui::EndMenu();
	}

}

void RenderManager::begin_frame() {
	m_main_target->clear(m_clear_color);
	m_window_target->clear(m_clear_color);
}


void RenderManager::end_frame() {

	/*
	First we render the overlayed debug info that should be over the real game
	and not in the final window.
	*/
	DeveloperModeManager::get().draw_debug_overlay();

	/*
	We get the main low-res target ready to be rendered
	*/
	m_main_target->display();


	if (m_simulating_crt) {
		
		/*
		We just give the texture with the game to our CRT Renderer and
		let it handle the magic :D
		*/
		m_crt_renderer->draw(m_main_target->getTexture());
		
	}
	else {
		/*
		We make a sprite with the texture and render it to the main window
		*/
		sf::Sprite main_target_sprite(m_main_target->getTexture());
		main_target_sprite.scale(m_current_texture_scale);

		m_window_target->pushGLStates();
		m_window_target->draw(main_target_sprite);
		m_window_target->popGLStates();
	}


	/*
	Now we set up everything we want to render for the debug GUI

	We must draw all widgets between Update and Render
	*/
	ImGui::SFML::Update(*m_window_target, DELTA_TIME);

	DeveloperModeManager::get().draw_gui();
	if(m_simulating_crt) m_crt_renderer->draw_parameter_gui();

	/*
	Now we render overlayed things such as the debug GUI
	*/
	m_window_target->pushGLStates();
	ImGui::SFML::Render(*m_window_target);
	m_window_target->popGLStates();

	/*
	And finally we display the image
	*/
	m_window_target->display();
	TimeManager::get().end_of_frame();
}


sf::Vector2f RenderManager::map_pixel_to_coords(sf::Vector2i position)
{
	auto window_coords = m_window_target->mapPixelToCoords(position);
	auto center = m_main_target->getView().getCenter();
	auto size = m_main_target->getView().getSize();
	return sf::Vector2f(
		(window_coords.x / m_current_texture_scale.x) + center.x - (size.x / 2),
		(window_coords.y / m_current_texture_scale.y) + center.y - (size.y / 2)
	);
}
