#pragma once

#include "BufferCommon.h"


class ElementBuffer {

private:
	GLuint m_id;
	GLuint m_count;
	BufferUsage m_usage;

public:
	ElementBuffer(GLushort* data, GLsizei count, BufferUsage usage);
	~ElementBuffer();

	void bind() const;
	void unbind() const;

	inline GLuint get_component_count() const { return m_count; }

};