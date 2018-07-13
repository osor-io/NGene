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
	m_window_target = WindowManager::get().get_window_render_target();
	m_main_target = std::make_unique<sf::RenderTexture>();
	m_main_target->create(config::resolutions::internal_resolution_width, config::resolutions::internal_resolution_height);
	m_main_target->setSmooth(false);
	m_main_target->setRepeated(false);

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

void RenderManager::begin_frame() {
	m_main_target->clear(m_clear_color);
	m_window_target->clear(m_clear_color);
}

void RenderManager::end_frame() {

	/*
	First we render the overlayed debug info that should be over the real game
	and not in the final window.
	*/
	//DeveloperModeManager::get().draw_debug_overlay();

	/*
	We get the main low-res target ready to be rendered
	*/
	m_main_target->display();

	/*
	We make a sprite with the texture and render it to the main window
	*/
	sf::Sprite main_target_sprite(m_main_target->getTexture());
	main_target_sprite.scale(config::resolutions::external_resolution_multiplier,
		config::resolutions::external_resolution_multiplier);
	m_window_target->draw(main_target_sprite);


	/*
	Now we set up everything we want to render for the debug GUI

	We must create all widgets between Update and Render
	*/
	ImGui::SFML::Update(*m_window_target, DELTA_TIME);

	DeveloperModeManager::get().draw_gui();


	/*
	Now we render overlayed things such as the debug GUI
	*/
	ImGui::SFML::Render(*m_window_target);


	/*
	And finally we display the image
	*/
	m_window_target->display();
	TimeManager::get().end_of_frame();
}


/*

@@NOTE: This can still be used regardless of the fact that we might render to
a texture first and then draw that texture in some other way into the screen. This will
print stuff on top of the game but with the low internal resolution.

*/
sf::Vector2i RenderManager::map_coords_to_pixel(sf::Vector2f position, bool scaled) {

	if (scaled) {
		return m_main_target->mapCoordsToPixel(position);
	}
	else {
		auto view = m_main_target->getView();
		auto previous_size = view.getSize();

		auto size = m_main_target->getDefaultView().getSize();
		view.setSize(size);
		m_main_target->setView(view);

		auto ret = m_main_target->mapCoordsToPixel(position);

		view.setSize(previous_size);
		m_main_target->setView(view);

		return ret;
	}

	return sf::Vector2i();
}
