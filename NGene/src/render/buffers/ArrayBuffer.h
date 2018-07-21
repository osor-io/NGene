#pragma once

#include "BufferCommon.h"

class ArrayBuffer {

private:
	GLuint m_id;
	GLuint m_component_count;
	BufferUsage m_usage;

public:

	/*
	
	Important to understand the difference between the two "count" parameters

	- float_count is how many floats there are in data.

	- component_count is how many components form one element of the array. This means
	that if this is a buffer of 3D positions component count would be 3, but if this is a 
	color with alpha this would be 4.

	Example:

		GLfloat positions[] =
		{
			0, 0, 0,
			0, 3, 0,
			8, 3, 0,
			8, 0, 0
		};
	
		//
		// float_count = 12 floats (4 vertices * 3 floats per position) and component_count = 3 floats per position
		//
		Buffer positions_buffer(positions, 12, 3);


		GLfloat colors[] =
		{
			1, 0, 1, 1,
			1, 0, 1, 1,
			1, 0, 1, 1,
			1, 0, 1, 1
		};

		//
		// float_count = 16 floats (4 vertices * 4 floats per color) and component_count = 4 floats per color
		//
		Buffer colors_buffer(colors, 16, 4); 

	
	*/
	ArrayBuffer(GLfloat* data, GLsizei float_count, GLuint component_count, BufferUsage usage);
	~ArrayBuffer();

	void bind() const;
	void unbind() const;

	inline GLuint get_component_count() const { return m_component_count; }

};