#pragma once

#include <vector>
#include <memory>

#include <GL/glew.h>

#include "ArrayBuffer.h"

/**

VAO (Vertex Array Object) that stores one or more VBO (Vertex Buffer Objects).
It stores the information for a complete renderable object including all the required
attributes (Positions, UVs, Colors, etc.)

@see ArrayBuffer

*/
class VertexArray {

private:
	GLuint m_id;
	std::vector<std::unique_ptr<ArrayBuffer>> m_buffers;

public:
	VertexArray();
	~VertexArray();

	/**
	
	Adds a new buffer to our array.

	The attribute_index parametes is the attribute index that will be used in the shader to reference
	the elements of the particular buffer getting added in a line such as

	layout(location = 0) in vec4 position; // 0 would be our attribute_index

	*/
	void addBuffer(std::unique_ptr<ArrayBuffer> buffer, GLuint attribute_index);

	void bind() const;
	void unbind() const;

};