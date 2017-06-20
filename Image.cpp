// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "Image.h"

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Log.h"
#include "Exagine.h"

// before you include stb_image.h file in *one* C or C++ file to create the implementation.
#define STB_IMAGE_IMPLEMENTATION

// The function stbi_failure_reason()
// can be queried for an extremely brief, end-user unfriendly explanation
// of why the load failed. Define STBI_NO_FAILURE_STRINGS to avoid
// compiling these strings at all, and STBI_FAILURE_USERMSG to get slightly
// more user-friendly ones.
#define STBI_FAILURE_USERMSG

#include "stb_image.h"

#include "File.h"

#define USE_STB_FILEMANAGER 1

namespace exa
{
	Image::Image()
	{
		stbi_set_flip_vertically_on_load(false);
	}

	Image::~Image()
	{
		freeData();
	}

	Image::Image(const char* fileName)
	{
		load(fileName);
	}
	/**
	* @see https://github.com/nothings/stb/blob/master/stb_image.h#L227
	**/
	void Image::load(const char* fileName)
	{
		std::string path = PROJECT_IMAGES_DIR;
		std::string filefullpath = path + fileName;

#ifdef USE_STB_FILEMANAGER
		data = stbi_load(filefullpath.c_str(), &width, &height, &numComponents, /* desired_channels */ requestedFormat);

		if (data == nullptr) {
			log::error("Unable to load Image: %s", fileName);
			log::error("stbi error: %s", stbi_failure_reason());
			EXAGINE().stop();
		}
#else
		File ImageFile(filefullpath.c_str());
		loadFromMemory(ImageFile.getBuffer(), ImageFile.getLength());
#endif
	}

	void Image::loadFromMemory(const unsigned char* cBuf, int bufLen)
	{
		data = stbi_load_from_memory(cBuf, bufLen, &width, &height, &numComponents, /* desired_channels */ requestedFormat);

		if (data == nullptr) {
			log::error("Unable to load Image from memory");
			log::error("stbi error: %s", stbi_failure_reason());
			EXAGINE().stop();
		}
	}

	void Image::freeData()
	{
		if (data != nullptr) {
			stbi_image_free(data);
			data = nullptr;
		}
	}
}

