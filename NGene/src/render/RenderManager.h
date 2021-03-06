#pragma once
#include <Manager.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>

#include "./crt/CRTRenderer.h" 


class RenderManager : public Manager<RenderManager> {
	friend class CRSP<RenderManager>;
private:
	RenderManager();
	~RenderManager();

public:

	void start_up() override;
	void shut_down() override;

	sf::RenderTarget* get_main_render_target();

	void set_main_target_params(sf::Vector2f center, float zoom);

	sf::Vector2f get_current_center() const;
	float get_current_zoom() const;

	void draw_debug_gui();

	void begin_frame();
	void end_frame();

	sf::Vector2f map_pixel_to_coords(sf::Vector2i position);


private:

	sf::Color m_clear_color{ sf::Color::Black };

	sf::RenderWindow* m_window_target;
	std::unique_ptr<sf::RenderTexture> m_main_target;

	sf::Vector2f m_current_texture_scale{ 1.f,1.f };

	sf::Vector2f m_current_center{ 0.0f,0.0f };
	float m_current_zoom{ 1.0f };

	std::unique_ptr<CRTRenderer> m_crt_renderer;

	bool m_simulating_crt{true};

};
