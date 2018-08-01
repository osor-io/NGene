#pragma once

#include <memory>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <SFML/Graphics.hpp>

#include "../render/buffers/ArrayBuffer.h"
#include "../render/buffers/ElementBuffer.h"
#include "../render/buffers/VertexArray.h"
#include "../render/shader/Shader.h"


#define RENDER_BUFFER_COUNT 2
#define FINAL_RENDER_BUFFER RENDER_BUFFER_COUNT

#define FULLSCREEN_PASSES 2

namespace config {
	namespace crt {
		namespace paths {
			constexpr auto lut = "res/assets/crt/lut.png";
			constexpr auto scanlines = "res/assets/crt/scanlines.png";
			constexpr auto ntsc_pattern = "res/assets/crt/ntsc_pattern.bmp";
		}
	}
}

class CRTRenderer {

private:

	struct CRTEffectInputTextures {
		sf::Texture lut;
		sf::Texture scanlines;
		sf::Texture ntsc;
	};
	CRTEffectInputTextures m_effect_textures;

	struct CRTEffectInputParameters {

		// Color correction
		struct CRTColorCorrectionParameters {
			float color_correction_strength = 1.0f;
		};
		CRTColorCorrectionParameters color;

		// Composite Shader
		struct CRTCompositeParameters {
			float ntsc_lerp = 1.f;
			float tuning_sharp = 0.056f;
			glm::vec4 tuning_persistence{ 0.8f, 0.3f, 0.3f, 0.0f };
			float tuning_bleed = 0.1f;
			float tuning_ntsc = 0.2f;
		};
		CRTCompositeParameters composite;

		// Screen Shader
		struct CRTScreenParameters {
			glm::vec3 tuning_light_position{ 1.9, 1.7f, -9.7f };

			glm::vec2 uv_scalar{ 1,1 };
			glm::vec2 uv_offset{ 0,0 };

			glm::vec2 crt_mask_scale{ 528, 812 };
			glm::vec2 crt_mask_offset{ 0,0 };

			float tuning_overscan = 0.9f;
			float tuning_dimming = 1.f;
			float tuning_saturation = 1.5f;
			float tuning_reflection_scalar = 0.7f;
			float tuning_barrel_distortion = 0.6f;
			float tuning_scanline_brightness = 0.67f;
			float tuning_scanline_opacity = 1.4f;
			float tuning_diffuse_brightness = 3.f;
			float tuning_specular_brightness = 0.62f;
			float tuning_specular_power = 3.0f;
			float tuning_fresnel_brightness = 3.0f;
		};
		CRTScreenParameters screen;

		struct CRTFinalParameters {
			float tuning_blur{ 0.160f };
		};
		CRTFinalParameters final;

	};
	CRTEffectInputParameters m_effect_parameters;

	const sf::RenderWindow& m_window_ref;

	std::unique_ptr<Shader> m_ntsc_shader;
	std::unique_ptr<Shader> m_composite_shader;
	std::unique_ptr<Shader> m_screen_shader;
	std::unique_ptr<Shader> m_copy_shader;
	std::unique_ptr<Shader> m_final_fullscreen_shader;


	std::unique_ptr<VertexArray> m_vertex_array_object;
	std::unique_ptr<ElementBuffer> m_index_buffer_object;


	GLuint m_frame_buffers[RENDER_BUFFER_COUNT + 1];
	GLuint m_rendered_textures[RENDER_BUFFER_COUNT + 1];
	GLsizei m_render_buffer_index{ 0 };

	GLuint m_window_frame_buffer[FULLSCREEN_PASSES];
	GLuint m_window_render_texture[FULLSCREEN_PASSES];

public:
	CRTRenderer(const sf::RenderWindow& window);
	~CRTRenderer();

	void draw(const sf::Texture& texture);

	void draw_parameter_gui();

	void on_resize(GLuint width, GLuint height);

private:
	void generate_crt_mesh();

	void init_ntsc_shader();
	void init_composite_shader();
	void init_screen_shader();
	void init_final_fullscreen_shader();

	void create_render_target(GLuint& frame_buffer, GLuint& render_texture, GLsizei width, GLsizei height);
	void create_low_res_render_targets();
	void create_window_render_target();

	/*
	These return the id of the FrameBuffer they drew to (0 is screen)
	*/
	GLsizei draw_ntsc_color_effect(const sf::Texture & texture);
	GLsizei draw_in_screen();
	GLsizei draw_out_screen(GLsizei render_buffer_to_put_in_crt_mesh);
	GLsizei draw_final_fullscreen(GLsizei render_buffer_to_draw);

};
