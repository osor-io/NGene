#include "Shader.h"

#include <iostream>
#include <sstream>
#include <assert.h>

#include "../utils/Debug.h"
#include "../resources/TextFileManager.h"


Shader::Shader(const char* filename) {

	auto source_text_resource = TextFileManager::get().get_scoped_resource(filename);

	m_sources = get_sources_from_text(*source_text_resource.resource);

	if (!m_sources.is_valid()) {
		LOG_ERROR("The shader in " << source_text_resource.m_filename << " is not valid");
		return;
	}
	else {
		create_program_with_sources();
	}
}

Shader::Shader(
	const std::string& vertex_code,
	const std::string& geometry_code,
	const std::string& fragment_code) {

	m_sources.vertex = vertex_code;
	m_sources.geometry = geometry_code;
	m_sources.fragment = fragment_code;

	if (!m_sources.is_valid()) {
		LOG_ERROR("The shader provided directly in source strings is not valid");
		return;
	}
	else {
		create_program_with_sources();
	}

}

void Shader::create_program_with_sources() {


	auto has_geometry_shader = !m_sources.geometry.empty();

	auto vertex_shader_id = GLuint{};
	auto geometry_shader_id = GLuint{};
	auto fragment_shader_id = GLuint{};

	auto result = int{};
	char error_log_buffer[512];

	auto check_shader = [&error_log_buffer](unsigned int id) -> bool {
		auto result = int{};
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetShaderInfoLog(id, 512, nullptr, error_log_buffer);
			LOG_ERROR("We couldn't compile the shader with id " << id << "\n\t"
				"The error provided is: " << error_log_buffer);
		}
		return result;
	};

	auto compile_shader = [&check_shader](const GLchar* source, GLenum shader_type) -> GLuint {
		auto id = glCreateShader(shader_type);
		glShaderSource(id, 1, &source, nullptr);
		glCompileShader(id);
		if (!check_shader(id)) {
			LOG_ERROR("Error compiling shader");
		};
		return id;
	};

	// Compile Shaders
	{
		vertex_shader_id = compile_shader(m_sources.vertex.c_str(), GL_VERTEX_SHADER);
		fragment_shader_id = compile_shader(m_sources.fragment.c_str(), GL_FRAGMENT_SHADER);

		if (has_geometry_shader)
			geometry_shader_id = compile_shader(m_sources.geometry.c_str(), GL_GEOMETRY_SHADER);
	}


	// Create the program
	{
		m_id = glCreateProgram();

		// Attach the shaders
		glAttachShader(m_id, vertex_shader_id);
		glAttachShader(m_id, fragment_shader_id);
		if (has_geometry_shader)
			glAttachShader(m_id, geometry_shader_id);

		// Link the program
		glLinkProgram(m_id);
		
		// Check for errors
		glGetProgramiv(m_id, GL_LINK_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(m_id, 512, nullptr, error_log_buffer);
			LOG_ERROR("We couldn't link the program with id " << m_id << "\n\t"
				"The error provided is: " << error_log_buffer);
		}

		// Validate the program
		glValidateProgram(m_id);

		// And check for errors for that
		glGetProgramiv(m_id, GL_VALIDATE_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(m_id, 512, nullptr, error_log_buffer);
			LOG_ERROR("We couldn't validate the program with id " << m_id << "\n\t"
				"The error provided is: " << error_log_buffer);
		}

		// Detach the shaders
		glDetachShader(m_id, vertex_shader_id);
		glDetachShader(m_id, fragment_shader_id);
		if (has_geometry_shader)
			glDetachShader(m_id, geometry_shader_id);

		// Delete the shaders
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
		if (has_geometry_shader)
			glDeleteShader(geometry_shader_id);
	}

	m_valid_program = true;
}


Shader::~Shader() {
	glDeleteProgram(m_id);
}

Shader::ShaderSources Shader::get_sources_from_text(const std::string& all_sources) {

	auto type = ShaderType::NONE;

	auto input_stream = std::istringstream(all_sources);
	auto line = std::string{};

	std::stringstream source_streams[ShaderType::COUNT];

	/*
	for (auto i = 0; i < ShaderType::COUNT; ++i) {
		source_streams[i] = std::stringstream(std::ios_base::app | std::ios_base::out | std::ios_base::in);
	}
	*/

	if (input_stream.good()) {
		while (std::getline(input_stream, line)) {

			if (line.find("#shader") != std::string::npos) { // Declaring a new shader source for a particular type

				if (line.find("vertex") != std::string::npos) {
					type = ShaderType::VERTEX;
				}
				else if (line.find("geometry") != std::string::npos) {
					type = ShaderType::GEOMETRY;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::FRAGMENT;
				}

			}
			else {
				assert(type != ShaderType::NONE);

				source_streams[(int)type] << line << '\n';
			}
		}
	}
	else {
		LOG_ERROR("Error reading the stream containing the source for the shader");
	}

	return {
		source_streams[ShaderType::VERTEX].str(),
		source_streams[ShaderType::GEOMETRY].str(),
		source_streams[ShaderType::FRAGMENT].str()
	};

}


void Shader::bind() const {
	if (m_valid_program) {
		glUseProgram(m_id);
	}
}

void Shader::unbind() const {
	glUseProgram(0);
}

