﻿#include "CRTRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../utils/Debug.h"

#include "../window/WindowManager.h"

#include "../utils/File.h"

#include <imgui.h>

CRTRenderer::CRTRenderer(const sf::RenderWindow& window) :
	m_window_ref(window)
{

	m_window_ref.setActive(true);

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


		m_effect_textures.lut.setRepeated(false);
		m_effect_textures.ntsc.setRepeated(false);
		m_effect_textures.scanlines.setRepeated(true);

		m_effect_textures.lut.setSmooth(false);
		m_effect_textures.ntsc.setSmooth(false);
		m_effect_textures.scanlines.setSmooth(false);

	}


	{
		create_low_res_render_targets();
		create_window_render_target();
		generate_crt_mesh();
		init_ntsc_shader();
		init_composite_shader();
		init_screen_shader();
		init_final_fullscreen_shader();
	}

}


CRTRenderer::~CRTRenderer() {

	//
	// Set the OpenGL context to active because we might need it
	// when destructing the buffers/shaders/etc. that we have as 
	// members of this class.
	//
	m_window_ref.setActive(true);

}


void CRTRenderer::draw(const sf::Texture& texture) {

	m_window_ref.setActive(true);

	{
		draw_ntsc_color_effect(texture);
		auto render_buffer_to_use = draw_in_screen();
		render_buffer_to_use = draw_out_screen(render_buffer_to_use);
		draw_final_fullscreen(render_buffer_to_use);
	}

}


GLsizei CRTRenderer::draw_ntsc_color_effect(const sf::Texture & texture) {

	auto current_index = m_render_buffer_index;

	// Bind our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[current_index]);
	glViewport(0, 0, config::resolutions::internal_resolution_width, config::resolutions::internal_resolution_height);

	glClear(GL_COLOR_BUFFER_BIT);

	m_ntsc_shader->bind();
	{
		m_ntsc_shader->set_uniform_texture("game_frame", texture, 1);
		m_ntsc_shader->set_uniform_texture("lut_texture", m_effect_textures.lut, 0);
		m_ntsc_shader->set_uniform_1f("tuning_strength", m_effect_parameters.color.color_correction_strength);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	m_ntsc_shader->unbind();


	return current_index;
}

GLsizei CRTRenderer::draw_in_screen() {

	auto current_index = m_render_buffer_index;
	auto previous_index = m_render_buffer_index - 1 < 0 ? RENDER_BUFFER_COUNT - 1 : m_render_buffer_index - 1;

	//
	// Draw the in screen effects to the final render buffer that will be pasted on the CRT Mesh
	//
	{
		// Bind our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[FINAL_RENDER_BUFFER]);
		glViewport(0, 0, config::resolutions::internal_resolution_width, config::resolutions::internal_resolution_height);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_composite_shader->bind();
		{
			m_composite_shader->set_uniform_texture("ntsc_artifact_tex", m_effect_textures.ntsc, 2);
			m_composite_shader->set_uniform_texture("previous_frame", m_rendered_textures[previous_index], 1);
			m_composite_shader->set_uniform_texture("current_frame", m_rendered_textures[current_index], 0);

			// We switch between lerping to the right or the left of the pixel with ntsc lerp
			static bool pair = true;
			m_composite_shader->set_uniform_1f("ntsc_lerp",
				(pair = !pair) ? m_effect_parameters.composite.ntsc_lerp : 1.f - m_effect_parameters.composite.ntsc_lerp);


			m_composite_shader->set_uniform_1f("tuning_sharp", m_effect_parameters.composite.tuning_sharp);
			m_composite_shader->set_uniform_4f("tuning_persistence", m_effect_parameters.composite.tuning_persistence);
			m_composite_shader->set_uniform_1f("tuning_bleed", m_effect_parameters.composite.tuning_bleed);

			m_composite_shader->set_uniform_1f("tuning_ntsc", m_effect_parameters.composite.tuning_ntsc);



			// Draw Call for fullscreen shader
			glDrawArrays(GL_TRIANGLES, 0, 3);

		}
		m_composite_shader->unbind();
	}

	//
	// Copy the texture we just rendered to to the buffer that will be the previous buffer in the next iteration
	//
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffers[current_index]);
		glViewport(0, 0, config::resolutions::internal_resolution_width, config::resolutions::internal_resolution_height);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		m_copy_shader->bind();
		{
			m_copy_shader->set_uniform_texture("tex", m_rendered_textures[FINAL_RENDER_BUFFER], 0);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		m_copy_shader->unbind();
	}

	// Update the index to draw to the appropriate render target
	m_render_buffer_index = m_render_buffer_index + 1 < RENDER_BUFFER_COUNT ? m_render_buffer_index + 1 : 0;

	return FINAL_RENDER_BUFFER;
}

GLsizei CRTRenderer::draw_out_screen(GLsizei render_buffer_to_put_in_crt_mesh) {

	glBindFramebuffer(GL_FRAMEBUFFER, m_window_frame_buffer[0]);
	glViewport(0, 0, m_window_ref.getSize().x, m_window_ref.getSize().y);

	m_screen_shader->bind();
	{

		//
		// @@NOTE: We need to bind texture unit 0 last here, or SFML freaks out about 1 being the last one bound
		// when it tries to push/pop OpenGL states.
		//

		m_screen_shader->set_uniform_texture("scanlines_texture", m_effect_textures.scanlines, 1);
		m_screen_shader->set_uniform_texture("low_res_texture", m_rendered_textures[render_buffer_to_put_in_crt_mesh], 0);

		m_screen_shader->set_uniform_3f("tuning_light_position", m_effect_parameters.screen.tuning_light_position);

		m_screen_shader->set_uniform_2f("uv_scalar", m_effect_parameters.screen.uv_scalar);
		m_screen_shader->set_uniform_2f("uv_offset", m_effect_parameters.screen.uv_offset);

		m_screen_shader->set_uniform_2f("crt_mask_scale", m_effect_parameters.screen.crt_mask_scale);
		m_screen_shader->set_uniform_2f("crt_mask_offset", m_effect_parameters.screen.crt_mask_offset);


		m_screen_shader->set_uniform_1f("tuning_overscan", m_effect_parameters.screen.tuning_overscan);
		m_screen_shader->set_uniform_1f("tuning_dimming", m_effect_parameters.screen.tuning_dimming);
		m_screen_shader->set_uniform_1f("tuning_saturation", m_effect_parameters.screen.tuning_saturation);
		m_screen_shader->set_uniform_1f("tuning_reflection_scalar", m_effect_parameters.screen.tuning_reflection_scalar);
		m_screen_shader->set_uniform_1f("tuning_barrel_distortion", m_effect_parameters.screen.tuning_barrel_distortion);
		m_screen_shader->set_uniform_1f("tuning_scanline_brightness", m_effect_parameters.screen.tuning_scanline_brightness);
		m_screen_shader->set_uniform_1f("tuning_scanline_opacity", m_effect_parameters.screen.tuning_scanline_opacity);
		m_screen_shader->set_uniform_1f("tuning_diffuse_brightness", m_effect_parameters.screen.tuning_diffuse_brightness);
		m_screen_shader->set_uniform_1f("tuning_specular_brightness", m_effect_parameters.screen.tuning_specular_brightness);
		m_screen_shader->set_uniform_1f("tuning_specular_power", m_effect_parameters.screen.tuning_specular_power);
		m_screen_shader->set_uniform_1f("tuning_fresnel_brightness", m_effect_parameters.screen.tuning_fresnel_brightness);


		m_vertex_array_object->bind();
		m_index_buffer_object->bind();

		glDrawElements(GL_TRIANGLES, m_index_buffer_object->get_component_count(), GL_UNSIGNED_SHORT, 0);

		m_index_buffer_object->unbind();
		m_vertex_array_object->unbind();
	}
	m_screen_shader->unbind();

	return m_window_render_texture[0];
}

GLsizei CRTRenderer::draw_final_fullscreen(GLsizei render_buffer_to_draw) {

	auto window_size = m_window_ref.getSize();
	auto resolution = glm::vec2(window_size.x, window_size.y);

	assert(m_window_render_texture[0] == render_buffer_to_draw);

#define PASSES 9

	for (auto i = 0; i < PASSES; ++i) {
		// Render first pass of the fullscreen effects
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_window_frame_buffer[1]);
			glViewport(0, 0, m_window_ref.getSize().x, m_window_ref.getSize().y);

			glClear(GL_COLOR_BUFFER_BIT);

			m_final_fullscreen_shader->bind();
			{
				m_final_fullscreen_shader->set_uniform_texture("image", m_window_render_texture[0], 0);
				m_final_fullscreen_shader->set_uniform_1f("tuning_blur", m_effect_parameters.final.tuning_blur);
				m_final_fullscreen_shader->set_uniform_2f("direction", glm::vec2(1, 0));
				m_final_fullscreen_shader->set_uniform_2f("resolution", resolution);

				// Draw Call for fullscreen shader
				glDrawArrays(GL_TRIANGLES, 0, 3);
			}
			m_final_fullscreen_shader->unbind();
		}

		// Render second (and last) pass of the fullscreen effects
		{
			if (i == PASSES - 1) {
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			else {
				glBindFramebuffer(GL_FRAMEBUFFER, m_window_frame_buffer[0]);
			}
			glViewport(0, 0, m_window_ref.getSize().x, m_window_ref.getSize().y);

			glClear(GL_COLOR_BUFFER_BIT);

			m_final_fullscreen_shader->bind();
			{
				m_final_fullscreen_shader->set_uniform_texture("image", m_window_render_texture[1], 0);
				m_final_fullscreen_shader->set_uniform_1f("tuning_blur", m_effect_parameters.final.tuning_blur);
				m_final_fullscreen_shader->set_uniform_2f("direction", glm::vec2(0, 1));
				m_final_fullscreen_shader->set_uniform_2f("resolution", resolution);

				// Draw Call for fullscreen shader
				glDrawArrays(GL_TRIANGLES, 0, 3);
			}
			m_final_fullscreen_shader->unbind();
		}
	}

	return 0;
}


void CRTRenderer::generate_crt_mesh() {

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> textureUVs;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> colors;
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

			// 4:3 resolution for the image to be stretched from 8:7
			auto x_total_side = 8.f;
			auto y_total_side = 6.f;

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
						//
						// @@TODO: Calculate the normal direction based on the
						// position of the vertex in the quad.
						//

						normals.push_back(0.0f);
						normals.push_back(0.0f);
						normals.push_back(1.0f);

						normals.push_back(0.0f);
						normals.push_back(0.0f);
						normals.push_back(1.0f);

						normals.push_back(0.0f);
						normals.push_back(0.0f);
						normals.push_back(1.0f);

						normals.push_back(0.0f);
						normals.push_back(0.0f);
						normals.push_back(1.0f);
					}

					// Add to Colors
					{
						//
						// @@TODO: Darken the pixels that are closer to the edges
						//

						colors.push_back(1.0f);
						colors.push_back(1.0f);
						colors.push_back(1.0f);
						colors.push_back(1.0f);

						colors.push_back(1.0f);
						colors.push_back(1.0f);
						colors.push_back(1.0f);
						colors.push_back(1.0f);

						colors.push_back(1.0f);
						colors.push_back(1.0f);
						colors.push_back(1.0f);
						colors.push_back(1.0f);

						colors.push_back(1.0f);
						colors.push_back(1.0f);
						colors.push_back(1.0f);
						colors.push_back(1.0f);
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
	m_vertex_array_object->addBuffer(std::make_unique<ArrayBuffer>(colors.data(), colors.size(), 4, BufferUsage::STATIC_DRAW),
		3);	// Colors

}

void CRTRenderer::init_ntsc_shader() {

	m_ntsc_shader = std::make_unique<Shader>("res/shaders/crt/ntsc.shader");

	m_ntsc_shader->bind();
	{
		m_ntsc_shader->set_uniform_texture("lut_texture", m_effect_textures.lut, 0);
		m_ntsc_shader->set_uniform_1f("lut_resolution", m_effect_textures.lut.getSize().y);
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



	m_copy_shader = std::make_unique<Shader>("res/shaders/crt/copy.shader");
	m_composite_shader->bind();
	//
	// Set copy shader parameters here
	//
	m_composite_shader->unbind();


}

void CRTRenderer::init_screen_shader() {

	m_screen_shader = std::make_unique<Shader>("res/shaders/crt/screen.shader");

	m_screen_shader->bind();

	auto camera_position = glm::vec3(0, 0, 0);
	auto mesh_position = glm::vec3(0, 0, -10);

	auto projection = glm::perspective(glm::radians(40.0f), 8.0f / 7.0f, 0.1f, 100.0f);
	auto view = glm::lookAt(camera_position, mesh_position, glm::vec3(0, 1, 0));
	auto model = glm::translate(glm::mat4(), mesh_position);
	auto mvp_matrix = projection * view * model;

	// We set theh MVP matrix for the shader
	m_screen_shader->set_uniform_mat4("mvp_matrix", mvp_matrix);
	m_screen_shader->set_uniform_mat4("model_to_world_matrix", model);

	m_screen_shader->set_uniform_3f("camera_position", camera_position);



	m_screen_shader->unbind();
}

void CRTRenderer::init_final_fullscreen_shader() {

	m_final_fullscreen_shader = std::make_unique<Shader>("res/shaders/crt/final.shader");

	m_final_fullscreen_shader->bind();

	m_final_fullscreen_shader->unbind();
}


void CRTRenderer::create_render_target(GLuint& frame_buffer, GLuint& render_texture, GLsizei width, GLsizei height) {

	{
		// Create and bind the framebuffer
		glGenFramebuffers(1, &frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

		// Create the texture we are going to render to
		glGenTextures(1, &render_texture);

		// Bind the texture so we can modify it
		glBindTexture(GL_TEXTURE_2D, render_texture);

		// Fill the texture with an empty image
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
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
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, render_texture, 0);

		// Set the draw buffers
		GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
		glNamedFramebufferDrawBuffers(frame_buffer, 1, draw_buffers);

		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			LOG("There has been an error generation the framebuffer, the status is: " << status);
		}

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

} // create_render_target

void CRTRenderer::create_low_res_render_targets() {


	for (auto i = 0; i <= RENDER_BUFFER_COUNT; ++i) { // We make an extra buffer to render as the result of low-res phase.

		create_render_target(
			m_frame_buffers[i],
			m_rendered_textures[i],
			config::resolutions::internal_resolution_width,
			config::resolutions::internal_resolution_height
		);

	}

}

void CRTRenderer::create_window_render_target() {

	auto window_size = m_window_ref.getSize();

	for (auto i = 0; i < FULLSCREEN_PASSES; ++i) {
		create_render_target(
			m_window_frame_buffer[i],
			m_window_render_texture[i],
			window_size.x,
			window_size.y
		);
	}

	LOG_NAMED(m_window_frame_buffer);
	LOG_NAMED(m_window_render_texture);

}

void CRTRenderer::on_resize(GLuint width, GLuint height) {

	//
	// We resize the texture we use to then directly render to the screen
	//
	for (auto i = 0; i < FULLSCREEN_PASSES; ++i)
	{
		// Bind the texture so we can modify it
		glBindTexture(GL_TEXTURE_2D, m_window_render_texture[i]);

		// Fill the texture with an empty image
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			0);
	}

}

#define MIN_SLIDER 0
#define MAX_SLIDER 3
void CRTRenderer::draw_parameter_gui() {

	ImGui::Begin("CRT Simulation Parameters");

	ImGui::Separator();
	{
		ImGui::SliderFloat("Color Tuning Strength",
			&m_effect_parameters.color.color_correction_strength, 0, 1);

	}
	ImGui::Separator();
	{
		ImGui::SliderFloat("ntsc_lerp",
			&m_effect_parameters.composite.ntsc_lerp, 0, 1);
		ImGui::SliderFloat("Sharpness",
			&m_effect_parameters.composite.tuning_sharp, MIN_SLIDER, MAX_SLIDER);
		// This is a Color with Alpha but, alpha persistence doesn't make sense so we'll just expose the RGB values
		ImGui::SliderFloat3("Persistence",
			&m_effect_parameters.composite.tuning_persistence[0], 0, 1);
		ImGui::SliderFloat("Bleed",
			&m_effect_parameters.composite.tuning_bleed, MIN_SLIDER, MAX_SLIDER);
		ImGui::SliderFloat("NTSC Artifacts",
			&m_effect_parameters.composite.tuning_ntsc, MIN_SLIDER, MAX_SLIDER);
	}
	ImGui::Separator();
	{
		ImGui::SliderFloat3("Light Position", &m_effect_parameters.screen.tuning_light_position[0], -10, 10);

		ImGui::DragFloat2("UV Scaling", &m_effect_parameters.screen.uv_scalar[0], 0, 10);
		ImGui::DragFloat2("UV Offset", &m_effect_parameters.screen.uv_offset[0], -10, 10);

		ImGui::DragFloat2("CRT Mask Scale", &m_effect_parameters.screen.crt_mask_scale[0], 0, 2000);
		ImGui::DragFloat2("CRT Mask Offset", &m_effect_parameters.screen.crt_mask_offset[0], -10, 10);

		ImGui::SliderFloat("Overscan", &m_effect_parameters.screen.tuning_overscan, MIN_SLIDER, MAX_SLIDER);
		ImGui::SliderFloat("Dimming", &m_effect_parameters.screen.tuning_dimming, MIN_SLIDER, MAX_SLIDER);
		ImGui::SliderFloat("Saturation", &m_effect_parameters.screen.tuning_saturation, MIN_SLIDER, MAX_SLIDER);
		ImGui::SliderFloat("Reflection Scalar", &m_effect_parameters.screen.tuning_reflection_scalar, MIN_SLIDER, MAX_SLIDER);
		ImGui::SliderFloat("Barrel Distortion", &m_effect_parameters.screen.tuning_barrel_distortion, MIN_SLIDER, MAX_SLIDER);
		ImGui::SliderFloat("Scanline Brightness", &m_effect_parameters.screen.tuning_scanline_brightness, MIN_SLIDER, MAX_SLIDER);
		ImGui::SliderFloat("Scanline Opacity", &m_effect_parameters.screen.tuning_scanline_opacity, MIN_SLIDER, MAX_SLIDER);
		ImGui::SliderFloat("Diff Brightness", &m_effect_parameters.screen.tuning_diffuse_brightness, MIN_SLIDER, MAX_SLIDER);
		ImGui::SliderFloat("Specular Brightness", &m_effect_parameters.screen.tuning_specular_brightness, MIN_SLIDER, MAX_SLIDER);
		ImGui::SliderFloat("Specular Power", &m_effect_parameters.screen.tuning_specular_power, MIN_SLIDER, MAX_SLIDER);
		ImGui::SliderFloat("Fresnel Brightness", &m_effect_parameters.screen.tuning_fresnel_brightness, MIN_SLIDER, MAX_SLIDER);
	}
	ImGui::Separator();
	{
		ImGui::SliderFloat("Blur", &m_effect_parameters.final.tuning_blur, 0, 1);
	}
	ImGui::Separator();

	ImGui::End();

}

