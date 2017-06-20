// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include "RenderPlatforms.h"

namespace exa
{

	enum
	{
		EXA_default = 0, // only used for req_comp

		EXA_grey = 1,
		EXA_grey_alpha = 2,
		EXA_rgb = 3,
		EXA_rgb_alpha = 4
	};

	class Image
	{
	private:
		void load(const char * fileName);

		void loadFromMemory(const unsigned char * cBuf, int bufLen);

	public:

		Image();
		Image(const char * fileName);

		~Image();

		void Image::freeData();

		unsigned char* getData() const {
			return data;
		}

		int getWidth() const {
			return width;
		}

		int getHeight() const {
			return height;
		}

		int getNumComponents() const {
			return numComponents;
		}

		int getRequestedFormat() const {
			return requestedFormat;
		}

	private:
		int width = 0;
		int height = 0;

		// Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)
		int numComponents = 0;

		unsigned char* data = nullptr;

		int requestedFormat = EXA_default;
	};
}