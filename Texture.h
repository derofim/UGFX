// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include "RenderPlatforms.h"

namespace exa
{
	class Image;

	class Texture
	{
	private:
		void generate();

	public:

		Texture();
		Texture(const char * fileName);

		~Texture();

		void bind();

		void unbind();

		GLuint get() const {
			return m_glTexture;
		}

		Image* getImage() const {
			return m_image;
		}

	private:
		Image* m_image;
		GLuint m_glTexture;
	};
}