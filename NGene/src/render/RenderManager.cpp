#include "RenderManager.h"
#include "../debug/DeveloperModeManager.h"
#include "../time/TimeManager.h"
#include "../window/WindowManager.h"

#include <SFML/OpenGL.hpp>

RenderManager::RenderManager() {}


RenderManager::~RenderManager() {}


void RenderManager::start_up() {

	/*
	We first initialize the window and the debug GUI.
	*/
	m_window_target = WindowManager::get().get_window_render_target();

	/*
	We set up the opengl context
	*/
	m_window_target->setActive(true);
	glViewport(0, 0, m_window_target->getSize().x, m_window_target->getSize().y);


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

	ImGui::SFML::Init(*m_window_target);
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
	DeveloperModeManager::get().draw_debug_overlay();

	/*
	We get the main low-res target ready to be rendered
	*/
	m_main_target->display();


	if (m_simulating_crt) {

		/*

		@@TODO @@DOING: Rendering main target texture with OpenGL. We have to look into what
		are the places where some of this code should be. For example, I believe the glViewport should be
		on initialization and on resize of the window.

		Also, the RenderTextures are always flipped: https://stackoverflow.com/questions/22424124/sfml-rendertexture-flipped-output

		The next step is to actually do the crt shaped mesh and try to paste the texture in it, then we'll start to look into the shaders
		for low res and high res.

		A nice shape for the CRT mesh could be this one that I did:

			z=(-(sqrt(x^2+y^2))^7)/7

			See it here: https://academo.org/demos/3d-surface-plotter/?expression=(-(sqrt(x%5E2%2By%5E2))%5E7)%2F7&xRange=-1%2C1&yRange=-1.333333%2C1.333333&resolution=100
			And here to implement the faster version of the formula by wolfram alpha: http://www.wolframalpha.com/input/?i=(-sqrt(x%5E2+%2B++y%5E2)%5E7)%2F7

		*/

		m_window_target->setActive(true);
		m_window_target->pushGLStates();
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glPushMatrix();

			sf::Texture::bind(&m_main_target.get()->getTexture());

			glBegin(GL_QUADS);

			auto size = 1.f;

			glTexCoord2f(0, 1); glVertex3f(-size, -size, 0.f);
			glTexCoord2f(0, 0); glVertex3f(-size, size, 0.f);
			glTexCoord2f(1, 0); glVertex3f(size, size, 0.f);
			glTexCoord2f(1, 1); glVertex3f(size, -size, 0.f);

			glEnd();
			glPopMatrix();
		}
		m_window_target->popGLStates();
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

	We must create all widgets between Update and Render
	*/
	ImGui::SFML::Update(*m_window_target, DELTA_TIME);

	DeveloperModeManager::get().draw_gui();


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
