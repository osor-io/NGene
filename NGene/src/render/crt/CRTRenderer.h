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

	std::unique_ptr<Shader> m_main_shader;
	std::unique_ptr<VertexArray> m_vertex_array_object;
	std::unique_ptr<ElementBuffer> m_index_buffer_object;


public:
	CRTRenderer(const sf::Window& window);
	~CRTRenderer();

	void draw(sf::Texture& texture);

private:
	void generate_crt_mesh();
	void generate_shader_matrices();


};
