#include "VertexArray.h"


VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_id);
}

void VertexArray::addBuffer(std::unique_ptr<ArrayBuffer> buffer, GLuint attribute_index) {

	bind();
	buffer->bind();

	glEnableVertexAttribArray(attribute_index);
	glVertexAttribPointer(attribute_index, buffer->get_component_count(), GL_FLOAT, GL_FALSE, 0, 0);

	buffer->unbind();
	unbind();

	m_buffers.push_back(std::move(buffer));
}

void VertexArray::bind() const {

	glBindVertexArray(m_id);

}

void VertexArray::unbind() const {

	glBindVertexArray(0);

}