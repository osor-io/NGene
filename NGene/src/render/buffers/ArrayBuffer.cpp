#include "ArrayBuffer.h"

#include "../utils/Debug.h"

ArrayBuffer::ArrayBuffer(GLfloat* data, GLsizei float_count, GLuint component_count, BufferUsage usage)
	: m_component_count(component_count), m_usage(usage) {

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);

	auto data_size_in_bytes = float_count * sizeof(GLfloat);

	switch (usage) {

	case BufferUsage::STATIC_DRAW: {
		glBufferData(GL_ARRAY_BUFFER, data_size_in_bytes, data, GL_STATIC_DRAW);
	}break;

	case BufferUsage::DYNAMIC_DRAW: {
		glBufferData(GL_ARRAY_BUFFER, data_size_in_bytes, data, GL_DYNAMIC_DRAW);
	}break;

	default:
		LOG_ERROR("Unsupported usage for Buffer");
		break;
	}

	auto size_in_buffer = GLint{};
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size_in_buffer);

	if (size_in_buffer != data_size_in_bytes) {
		glDeleteBuffers(1, &m_id);
		LOG_ERROR("We couldn't allocate a buffer with the intended size\n"
			"We wanted " << data_size_in_bytes << " bytes and got " << size_in_buffer << " bytes");
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


ArrayBuffer::~ArrayBuffer() {
	glDeleteBuffers(1, &m_id);
}

void ArrayBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}


void ArrayBuffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}