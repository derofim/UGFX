// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include "RenderPlatforms.h"

namespace exa
{
	class VertexBuffer
	{
	private:

	public:

		VertexBuffer();
		~VertexBuffer();

		void setData(const void* data, size_t length, GLuint usage);
		void bind();
		void unbind();

	private:
		GLuint m_VBO = 0;
	};
}