#pragma once

#include <string>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>

#include <SFML/Graphics/Texture.hpp>

#include <unordered_map>

/**

Represents an OpenGL shader class.

It can store multiple types of shaders defined in the same file
with the directive "#shader TYPE" before the region that represents
a particular shader.

*/
class Shader
{

private:
	struct ShaderSources {
		std::string vertex;
		std::string geometry;
		std::string fragment;

		bool are_valid() const {
			return (!vertex.empty() && !fragment.empty());
		}

	};

	enum ShaderType : int {
		NONE = -1,
		VERTEX = 0,
		GEOMETRY = 1,
		FRAGMENT = 2,
		COUNT = 3,
	};

	ShaderSources m_sources;

	bool m_valid_program{ false };

	GLuint m_id;

	const char* m_filename;

	std::unordered_map<const GLchar*, GLint> m_name_cache;

public:
	Shader(const char* filename);
	Shader(const std::string& vertex_code, const std::string& geometry_code, const std::string& fragment_code);
	~Shader();

	/**
	Returns the ID of the shader program for OpenGL
	*/
	GLuint get_program_id() { return m_id; }

	/**
	Binds the shader so it will be used for rendering
	*/
	void bind() const;

	/**
	Unbinds the shader
	*/
	void unbind() const;

	// Functions to set uniforms to the shader
#pragma region Uniform Setters

	void set_uniform_1i(const GLchar* name, int value);

	void set_uniform_1f(const GLchar* name, float value);
	void set_uniform_2f(const GLchar* name, const glm::vec2& value);
	void set_uniform_3f(const GLchar* name, const glm::vec3& value);
	void set_uniform_4f(const GLchar* name, const glm::vec4& value);

	void set_uniform_mat4(const GLchar* name, const glm::mat4 value);

	void set_uniform_texture(const GLchar* name, const sf::Texture& texture, GLuint unit);
	void set_uniform_texture(const GLchar* name, GLuint texture_id, GLuint unit);


#pragma endregion

private:


	static ShaderSources get_sources_from_text(const std::string& all_sources);

	void create_program_with_sources(const ShaderSources& sources);

	GLint get_uniform_location(const GLchar* name);


};

