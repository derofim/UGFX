// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include "exa.h"

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace exa
{
	class File
	{
	private:

	public:

		File();
		File(const char * filename);
		~File();

		bool load(const char* filename, const char* openMode = "rb");

		inline unsigned char* getBuffer() const {
			return m_fileBuffer;
		}

		int getLength() const {
			return m_fileLen;
		}

		static inline bool exists(const char* filename) {
			struct stat buffer;
			return (stat(filename, &buffer) == 0);
		}

	private:
		int m_fileLen = 0;
		unsigned char* m_fileBuffer = nullptr;

		/*
		* Usage:
		*	std::ifstream fd(filefullpath);
		*	auto src = slurp(fd);
		**/
		static inline std::string slurp(std::ifstream& in) {
			return static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str();
		}
	};
}