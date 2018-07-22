#include "CRTRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../utils/Debug.h"

#include "../window/WindowManager.h"

#include "../utils/File.h"

#include <imgui.h>

CRTRenderer::CRTRenderer(const sf::Window& window) :
	m_window_ref(window)
{

	// We load and set up the textures for the effect
	{
		if (!file_exists(config::crt::paths::lut) ||
			!file_exists(config::crt::paths::ntsc_pattern) ||
			!file_exists(config::crt::paths::scanlines)) {
			
			LOG_ERROR("We couldn't find the required texture files for the CRT simulation effect.");
		}
		if (!m_effect_textures.lut.loadFromFile(config::crt::paths::lut)) {
			LOG_ERROR("We couldn't load the texture for the CRT effect: " << config::crt::paths::lut);
		}
		if (!m_effect_textures.ntsc.loadFromFile(config::crt::paths::ntsc_pattern)) {
			LOG_ERROR("We couldn't load the texture for the CRT effect: " << config::crt::paths::ntsc_pattern);
		}
		if (!m_effect_textures.scanlines.loadFromFile(config::crt::paths::scanlines)) {
			LOG_ERROR("We couldn't load the texture for the CRT effect: " << config::crt::paths::scanlines);
		}

		
		m_effect_textures.lut.setRepeated(true);
		m_effect_textures.ntsc.setRepeated(false);
		m_effect_textures.scanlines.setRepeated(false);

		m_effect_textures.lut.setSmooth(false);
		m_effect_textures.ntsc.setSmooth(false);
		m_effect_textures.scanlines.setSmooth(false);
		
	}

	m_window_ref.setActive(true);
	{
		create_low_res_render_targets();
		generate_crt_mesh();
		init_ntsc_shader();
		init_composite_shader();
		init_screen_shader();
	}
	m_window_ref.setActive(false);
}


CRTRenderer::~CRTRenderer() {

}


void CRTRenderer::draw(sf::Texture& texture) {

	m_window_ref.setActive(true);
	{
		draw_ntsc_color_effect(texture);
		auto render_buffer_to_use = draw_in_screen();
		draw_out_screen(render_buffer_to_use);
	}
	m_window_ref.setActive(false);
}


GLsizei CRTRenderer::draw_ntsc_color_effect(const sf::Texture & texture) {

	auto current_index = m_render_buffer_index;

	// Bind our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[current_index]);
	glViewport(0, 0, config::resolutions::internal_resolution_width, config::resolutions::internal_resolution_height);

	glClear(GL_COLOR_BUFFER_BIT);

	m_ntsc_shader->bind();
	{
		//
		// @TODO: Find out why we need to set this texture in every frame
		//
		m_ntsc_shader->setUniformTexture("lut_texture", m_effect_textures.lut, 0);
		m_ntsc_shader->setUniformTexture("game_frame", texture, 1);
		m_ntsc_shader->setUniform1f("tuning_strength", m_effect_parameters.color_grading_strength);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	m_ntsc_shader->unbind();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_window_ref.getSize().x, m_window_ref.getSize().y);


	return current_index;
}

GLsizei CRTRenderer::draw_in_screen() {

	auto current_index = m_render_buffer_index;
	auto previous_index = m_render_buffer_index - 1 < 0 ? RENDER_BUFFER_COUNT - 1 : m_render_buffer_index - 1;

	// Bind our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[FINAL_RENDER_BUFFER]);
	glViewport(0, 0, config::resolutions::internal_resolution_width, config::resolutions::internal_resolution_height);


	glClear(GL_COLOR_BUFFER_BIT);

	m_composite_shader->bind();
	{
		m_composite_shader->setUniformTexture("current_frame", m_rendered_textures[current_index], 0);
		m_composite_shader->setUniformTexture("previous_frame", m_rendered_textures[previous_index], 1);

		// Draw Call for fullscreen shader
		glDrawArrays(GL_TRIANGLES, 0, 3);


	}
	m_composite_shader->unbind();


	// Update the index to draw to the appropriate render target
	m_render_buffer_index = m_render_buffer_index + 1 < RENDER_BUFFER_COUNT ? m_render_buffer_index + 1 : 0;

	return FINAL_RENDER_BUFFER;
}

GLsizei  CRTRenderer::draw_out_screen(GLsizei render_buffer_to_put_in_crt_mesh) {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_window_ref.getSize().x, m_window_ref.getSize().y);

	m_screen_shader->bind();
	{
		m_screen_shader->setUniformTexture("low_res_texture", m_rendered_textures[render_buffer_to_put_in_crt_mesh], 0);

		m_vertex_array_object->bind();
		m_index_buffer_object->bind();

		glDrawElements(GL_TRIANGLES, m_index_buffer_object->get_component_count(), GL_UNSIGNED_SHORT, 0);

		m_index_buffer_object->unbind();
		m_vertex_array_object->unbind();
	}
	m_screen_shader->unbind();

	return 0;
}


void CRTRenderer::generate_crt_mesh() {

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> textureUVs;
	std::vector<GLfloat> normals;
	std::vector<GLushort> indices;

	// We populate the vectors with the data from the tesselated quad
	{
		static auto columns = 45;
		static auto rows = 45;
		static auto exp_factor = 6.f;
		static auto max_depth = 1.5f;

		auto generate_tesselated_quad = [&]() {

			auto x_uv_increment = 1.f / columns;
			auto y_uv_increment = 1.f / rows;

			auto x_total_side = 8.f;
			auto y_total_side = 7.f;

			auto x_side = x_total_side / columns;
			auto y_side = y_total_side / rows;

			auto index_count = 0;

			for (auto j = 0; j < rows; ++j) {
				for (auto i = 0; i < columns; ++i) {

					auto x_offset = (i * x_side) - (x_total_side / 2.f);
					auto y_offset = ((rows - j - 1) * -y_side) + (y_total_side / 2.f);

					auto calc_z = [x_total_side, y_total_side](float x, float y) -> float {

						x /= (x_total_side / 2.f);
						y /= (y_total_side / 2.f);

						auto ret = -pow(sqrt(x*x + y * y), exp_factor) / exp_factor;

						return ret * max_depth;
					};

					auto update_vertex_z = [&calc_z](glm::vec3& vertex) {
						vertex.z = calc_z(vertex.x, vertex.y);
					};


					// Create vertices
					glm::vec3 vertex_a(x_offset, y_offset - y_side, 0.f);
					glm::vec3 vertex_b(x_offset, y_offset, 0.f);
					glm::vec3 vertex_c(x_offset + x_side, y_offset, 0.f);
					glm::vec3 vertex_d(x_offset + x_side, y_offset - y_side, 0.f);
					update_vertex_z(vertex_a);
					update_vertex_z(vertex_b);
					update_vertex_z(vertex_c);
					update_vertex_z(vertex_d);

					// Create UVs
					auto x_uv_0 = (i)* x_uv_increment;
					auto x_uv_1 = (i)* x_uv_increment + x_uv_increment;
					auto y_uv_1 = (rows - j - 1) * y_uv_increment;
					auto y_uv_0 = (rows - j - 1) * y_uv_increment + y_uv_increment;

					// Add to vertices
					{

						vertices.push_back(vertex_a.x);
						vertices.push_back(vertex_a.y);
						vertices.push_back(vertex_a.z);

						vertices.push_back(vertex_b.x);
						vertices.push_back(vertex_b.y);
						vertices.push_back(vertex_b.z);

						vertices.push_back(vertex_c.x);
						vertices.push_back(vertex_c.y);
						vertices.push_back(vertex_c.z);

						vertices.push_back(vertex_d.x);
						vertices.push_back(vertex_d.y);
						vertices.push_back(vertex_d.z);
					}


					// Add to UVs
					{
						textureUVs.push_back(x_uv_0);
						textureUVs.push_back(y_uv_0);

						textureUVs.push_back(x_uv_0);
						textureUVs.push_back(y_uv_1);

						textureUVs.push_back(x_uv_1);
						textureUVs.push_back(y_uv_1);

						textureUVs.push_back(x_uv_1);
						textureUVs.push_back(y_uv_0);
					}

					// Add to Normals
					{
						normals.push_back(0.0f);
						normals.push_back(0.0f);
						normals.push_back(-1.0f);

						normals.push_back(0.0f);
						normals.push_back(0.0f);
						normals.push_back(-1.0f);

						normals.push_back(0.0f);
						normals.push_back(0.0f);
						normals.push_back(-1.0f);

						normals.push_back(0.0f);
						normals.push_back(0.0f);
						normals.push_back(-1.0f);
					}

					// Add to indices
					{
						auto offset_prod = x_offset * y_offset;

						//
						// We orient the division of the triangles of the quad so
						// it looks better when moving the z value.
						//

						/*

							This for left-up and				 This for left-down and
							  right down areas						 right-up areas
									_____							 	  _____
								   |    /|							     |\    |
								   |   / |							     | \   |
								   |  /  | 							     |  \  |
								   | /   |								 |   \ |
								   |/    |							     |    \|
									‾‾‾‾‾		                          ‾‾‾‾‾
						*/

						if (offset_prod > 0) {
							indices.push_back(index_count);
							indices.push_back(index_count + 1);
							indices.push_back(index_count + 3);
							indices.push_back(index_count + 3);
							indices.push_back(index_count + 1);
							indices.push_back(index_count + 2);
						}
						else {
							indices.push_back(index_count);
							indices.push_back(index_count + 1);
							indices.push_back(index_count + 2);
							indices.push_back(index_count + 2);
							indices.push_back(index_count + 3);
							indices.push_back(index_count);
						}


						index_count += 4;
					}


				} // End of column loop
			} // End of row loop

		};
		generate_tesselated_quad();
	}


	// We generate our vertex array object that will hold all of our buffers with info about each vertex
	m_vertex_array_object = std::make_unique<VertexArray>();

	// We generate the buffer of indices
	m_index_buffer_object = std::make_unique<ElementBuffer>(indices.data(), indices.size(), BufferUsage::STATIC_DRAW);

	// And we add each of the buffers that contain info about the vertices
	m_vertex_array_object->addBuffer(std::make_unique<ArrayBuffer>(vertices.data(), vertices.size(), 3, BufferUsage::STATIC_DRAW),
		0);	// Position
	m_vertex_array_object->addBuffer(std::make_unique<ArrayBuffer>(textureUVs.data(), textureUVs.size(), 2, BufferUsage::STATIC_DRAW),
		1); // UVs
	m_vertex_array_object->addBuffer(std::make_unique<ArrayBuffer>(normals.data(), normals.size(), 3, BufferUsage::STATIC_DRAW),
		2);	// Normals

}

void CRTRenderer::init_ntsc_shader() {

	m_ntsc_shader = std::make_unique<Shader>("res/shaders/crt/ntsc.shader");

	m_ntsc_shader->bind();
	{
		m_ntsc_shader->setUniformTexture("lut_texture", m_effect_textures.lut, 0);
		m_ntsc_shader->setUniform1f("lut_resolution", m_effect_textures.lut.getSize().y);
	}
	m_ntsc_shader->unbind();
}

void CRTRenderer::init_composite_shader() {

	m_composite_shader = std::make_unique<Shader>("res/shaders/crt/composite.shader");

	m_composite_shader->bind();

	//
	// Set shader parameters here
	//

	m_composite_shader->unbind();
}

void CRTRenderer::init_screen_shader() {

	m_screen_shader = std::make_unique<Shader>("res/shaders/crt/screen.shader");

	m_screen_shader->bind();



	auto projection = glm::perspective(glm::radians(40.0f), 8.0f / 7.0f, 0.1f, 100.0f);
	auto view = glm::mat4();
	auto model = glm::translate(glm::mat4(), glm::vec3(0, 0, -10));
	auto mvp_matrix = projection * view * model;

	// We set theh MVP matrix for the shader
	m_screen_shader->setUniformMat4("mvp_matrix", mvp_matrix);


	m_screen_shader->unbind();
}

void CRTRenderer::create_low_res_render_targets() {

	for (auto i = 0; i <= RENDER_BUFFER_COUNT; ++i) { // We make an extra buffer to render as the result of low-res phase.

		// Create and bind the framebuffer
		glGenFramebuffers(1, &m_frame_buffers[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[i]);

		// Create the texture we are going to render to
		glGenTextures(1, &m_rendered_textures[i]);

		// Bind the texture so we can modify it
		glBindTexture(GL_TEXTURE_2D, m_rendered_textures[i]);

		// Fill the texture with an empty image
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			config::resolutions::internal_resolution_width,
			config::resolutions::internal_resolution_height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			0);

		// Set up filtering for the texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set the texture to the framebuffer
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_rendered_textures[i], 0);

		// Set the draw buffers
		GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
		glNamedFramebufferDrawBuffers(m_frame_buffers[i], 1, draw_buffers);

		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			LOG("There has been an error generation the framebuffers, the status is: " << status);
		}
	}
}

void CRTRenderer::draw_parameter_gui() {

	ImGui::Begin("CRT Simulation Parameters");
	

	ImGui::Separator();
	{
		ImGui::SliderFloat("Color Tuning Strength",
			&m_effect_parameters.color_grading_strength, 0, 1);

	}
	ImGui::Separator();


	ImGui::End();

}

