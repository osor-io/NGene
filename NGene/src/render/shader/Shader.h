#pragma once

#include <string>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

/**

Represents an OpenGL shader class.

It can store multiple types of shaders defined in the same file
with the directive "#shader TYPE" before the region that represents
a particular shader.

*/
class Shader
{
public:
	Shader(const char* filename);
	Shader(const std::string& vertex_code, const std::string& geometry_code, const std::string& fragment_code);

	void bind() const;
	void unbind() const;

	~Shader();

private:

	struct ShaderSources {
		std::string vertex;
		std::string geometry;
		std::string fragment;

		bool is_valid() const {
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

	static ShaderSources get_sources_from_text(const std::string& all_sources);

	void create_program_with_sources();

	ShaderSources m_sources;

	bool m_valid_program{false};

	GLuint m_id;
};

