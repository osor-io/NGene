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
		
		// NTSC Shader
		float color_grading_strength = 0.1f;

		// Composite Shader

		// Screen Shader

	};
	CRTEffectInputParameters m_effect_parameters;

	const sf::Window& m_window_ref;

	std::unique_ptr<Shader> m_ntsc_shader;
	std::unique_ptr<Shader> m_composite_shader;
	std::unique_ptr<Shader> m_screen_shader;

	std::unique_ptr<VertexArray> m_vertex_array_object;
	std::unique_ptr<ElementBuffer> m_index_buffer_object;


	GLuint m_frame_buffers[RENDER_BUFFER_COUNT + 1];
	GLuint m_rendered_textures[RENDER_BUFFER_COUNT + 1];
	GLsizei m_render_buffer_index{ 0 };

public:
	CRTRenderer(const sf::Window& window);
	~CRTRenderer();

	void draw(sf::Texture& texture);

	void draw_parameter_gui();

private:
	void generate_crt_mesh();

	void init_ntsc_shader();
	void init_composite_shader();
	void init_screen_shader();

	void create_low_res_render_targets();

	/*
	These return the id of the FrameBuffer they drew to (0 is screen)
	*/

	GLsizei draw_ntsc_color_effect(const sf::Texture & texture);
	GLsizei draw_in_screen();
	GLsizei draw_out_screen(GLsizei render_buffer_to_put_in_crt_mesh);

};
