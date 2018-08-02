#pragma once

#include "BufferCommon.h"

/**

IBO (Index Buffer Object). Stores the information about which indices should be drawn in which order
for a renderable object. This is tied to the attributes that each vertex of the object has which should
be in a VertexArray object.

@see VertexArray

@@NOTE: Since these are (as one would expect) heavily tied to a VertexArray, having to deal with both
objects shouldn't be necessary, I should consider integrating into the same class. It hasn't been done yet
since 3D rendering has just been introduced into this game and the requirements for what will be necessary
are not yet that clear. Still, this is something that looks like should be prioritized.

*/
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