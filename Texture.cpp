// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "Texture.h"

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "File.h"
#include "Image.h"
#include "Memory.h"

namespace exa
{
	Texture::Texture()
	{
	}

	Texture::~Texture()
	{
		SafeDelete(m_image);
	}

	Texture::Texture(const char* fileName)
	{
		m_image = exanew Image(fileName);
		generate();
		m_image->freeData();
	}

	void Texture::generate()
	{
		exaglGenTextures(1, &m_glTexture);

		bind();

		// Set texture clamp vs. wrap (repeat)
		// @note In ES2 all no-power of 2 textures an only be GL_CLAMP_TO_EDGE
		exaglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		exaglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set magnification (texel > pixel) and minification (texel < pixel) filters
		exaglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		exaglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
		GLenum bufferFormat = GL_RGBA;
		if (m_image->getNumComponents() == 3)
			bufferFormat = GL_RGB;

		// the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL
		GLenum storeFormat = bufferFormat;

		exaglTexImage2D(
			/*  texture target */ GL_TEXTURE_2D,
			/*  mipmap level  */ 0,
			/*  format we want to store the texture */ storeFormat,
			/*  width of the resulting texture */ m_image->getWidth(),
			/*  height of the resulting texture */ m_image->getHeight(),
			/*  always 0 */ 0,
			/*  format of the source image */ bufferFormat,
			/*  datatype of the source image */ GL_UNSIGNED_BYTE,
			/*  actual image data */ m_image->getData()
		);

		// Automatically generate all the required mipmaps for the currently bound texture
		exaglGenerateMipmap(GL_TEXTURE_2D);

		unbind();
	}

	void Texture::bind()
	{
		exaglBindTexture(GL_TEXTURE_2D, m_glTexture);
	}

	void Texture::unbind()
	{
		// Unbind texture when done, so we won't accidentily mess up our texture.
		exaglBindTexture(GL_TEXTURE_2D, 0);
	}
}