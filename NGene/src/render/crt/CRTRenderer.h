#pragma once

#include <memory>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <SFML/Graphics.hpp>

#include "../render/buffers/ArrayBuffer.h"
#include "../render/buffers/ElementBuffer.h"
#include "../render/buffers/VertexArray.h"
#include "../render/shader/Shader.h"


class CRTRenderer {

private:
	const sf::Window& m_window_ref;

	std::unique_ptr<Shader> m_ntsc_shader;
	std::unique_ptr<Shader> m_composite_shader;
	std::unique_ptr<Shader> m_screen_shader;

	std::unique_ptr<VertexArray> m_vertex_array_object;
	std::unique_ptr<ElementBuffer> m_index_buffer_object;


public:
	CRTRenderer(const sf::Window& window);
	~CRTRenderer();

	void draw(sf::Texture& texture);

private:
	void generate_crt_mesh();

	void init_ntsc_shader();
	void init_composite_shader();
	void init_screen_shader();

	void create_low_res_render_targets();

};
