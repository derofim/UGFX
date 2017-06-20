// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "VertexBuffer.h"

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace exa
{
	VertexBuffer::VertexBuffer()
	{
		exaglGenBuffers(1, &m_VBO);
	}

	VertexBuffer::~VertexBuffer()
	{
		exaglDeleteBuffers(1, &m_VBO);
	}

	void VertexBuffer::setData(const void* data, size_t length, GLuint usage)
	{
		bind();

		exaglBufferData(
			/* Type of the buffer we want to copy data into */ GL_ARRAY_BUFFER,
			/* Size of the data (in bytes) */ length,
			/* Actual data we want to send */ data,
			/* Can data change? (GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW) */ usage
		);
	}

	void VertexBuffer::bind()
	{
		exaglBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}

	void VertexBuffer::unbind()
	{
		exaglBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
