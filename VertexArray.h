// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include "RenderPlatforms.h"

namespace exa
{
	class VertexArray
	{
	private:

	public:

		VertexArray();
		~VertexArray();

		void bind();

		void unbind();

	private:
		GLuint m_VAO = 0;
	};
}