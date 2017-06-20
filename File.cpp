// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "File.h"

#include "Memory.h"
#include "Window.h"
#include "Log.h"

namespace exa
{
	File::File() {}

	File::File(const char* filename) {
		load(filename);
	}

	File::~File() {
		SafeDelete(m_fileBuffer);
	}

	/**
	* @brief Open file and store its data and length in memory.
	*
	* @param filename
	*	Full path to file.
	* @param openMode
	*	The mode string is treated roughly the same as in a call to the C library's fopen(), even if SDL doesn't happen to use fopen() behind the scenes.
	*
	* Available mode strings:
	* -# r
	*    Open a file for reading. The file must exist.
	* -# w
	*    Create an empty file for writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file.
	* -# a
	*    Append to a file. Writing operations append data at the end of the file. The file is created if it does not exist.
	* -# r+
	*    Open a file for update both reading and writing. The file must exist.
	* -# w+
	*    Create an empty file for both reading and writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file.
	* -# a+
	*    Open a file for reading and appending. All writing operations are performed at the end of the file, protecting the previous content to be overwritten. You can reposition (fseek, rewind) the internal pointer to anywhere in the file for reading, but writing operations will move it back to the end of file. The file is created if it does not exist.
	**/

	bool File::load(const char* filename, const char* openMode)
	{
		log::debug("loading file %s in mode %s", filename, openMode);

		// Create a new SDL_RWops structure for reading from and/or writing to a named file.
		SDL_RWops *rw = SDL_RWFromFile(filename, openMode);

		// File does not exist
		if (rw == nullptr) {
			log::error("Unable to read file: %s");
			Window::checkSDLError(__LINE__);
			return false;
		}

		Sint64 res_size = SDL_RWsize(rw);
		unsigned char* res = exanew unsigned char[static_cast<long>(res_size + 1)];

		Sint64 totalRead = 0, nb_read = 1;
		unsigned char* buffOffset = res;
		while (totalRead < res_size && nb_read != 0) {
			nb_read = SDL_RWread(rw, buffOffset, 1, static_cast<long>(res_size - totalRead));
			totalRead += nb_read;
			buffOffset += nb_read;
		}


		SDL_RWclose(rw);

		if (totalRead != res_size) {
			SafeDelete(res);
			return false;
		}

		res[totalRead] = '\0';

		m_fileLen = static_cast<int>(totalRead);
		m_fileBuffer = res;

		return true;
	}
}