// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "Log.h"
#include "exa.h"

#include "rang.hpp"

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <cstdio>

#define CONSOLE_WARNING_STYLE() std::cout << rang::control::forceColor \
<< rang::style::bold << rang::style::reset << rang::bg::yellow << rang::fg::gray

#define CONSOLE_ERROR_STYLE() std::cout << rang::control::forceColor \
<< rang::style::bold <<  rang::style::reset << rang::bg::red << rang::fg::gray

#define CONSOLE_MESSAGE_STYLE() std::cout << rang::control::forceColor \
<< rang::style::bold <<  rang::style::reset << rang::bg::green << rang::fg::gray

#define CONSOLE_RESET_STYLE() std::cout << rang::style::reset

namespace exa
{
	namespace log
	{

		/**
		* @param args
		*	a variable argument list
		* @param format
		*	a printf() style message format string
		* @param category
		*	the category of the message; one of:
		*		SDL_LOG_CATEGORY_APPLICATION - application log
		*		SDL_LOG_CATEGORY_ERROR       - error log
		*		SDL_LOG_CATEGORY_ASSERT      - assert log
		*		SDL_LOG_CATEGORY_SYSTEM      - system log
		*		SDL_LOG_CATEGORY_AUDIO       - audio log
		*		SDL_LOG_CATEGORY_VIDEO       - video log
		*		SDL_LOG_CATEGORY_RENDER      - render log
		*		SDL_LOG_CATEGORY_INPUT       - input log
		*		SDL_LOG_CATEGORY_TEST        - test log
		*		SDL_LOG_CATEGORY_RESERVED    - # = 1-10; reserved for future SDL library use
		*		SDL_LOG_CATEGORY_CUSTOM      - reserved for application use
		* @param priority
		*	the priority of the message; one of:
		*		SDL_LOG_PRIORITY_VERBOSE
		*		SDL_LOG_PRIORITY_DEBUG
		*		SDL_LOG_PRIORITY_INFO
		*		SDL_LOG_PRIORITY_WARN
		*		SDL_LOG_PRIORITY_ERROR
		*		SDL_LOG_PRIORITY_CRITICAL
		*		SDL_NUM_LOG_PRIORITIES
		**/
		void Log::logVaSdl(va_list args,
			const char* format,
			int category,
			SDL_LogPriority priority)
		{
			if (LOGGER().isEnabled())
			{
				// This version of SDL_LogMessage() uses a stdarg variadic argument list.
				SDL_LogMessageV(category, priority, format, args);
			}
		}

		void Log::logSdl(const char* format,
			int category,
			SDL_LogPriority priority, ...)
		{
			va_list args; 
			va_start(args, priority);

			logVaSdl(args, format, category, priority);
			va_end(args);
		}

		// Set the priority of a particular log category.
		// @see  Logger class uses SDL_LOG_CATEGORY_CUSTOM
		// @note MUST be called before all subsystems (otherwise log will not work) 
		// @note All SDL2 Log Categories except Application are at SDL_LOG_PRIORITY_CRITICAL, 
		//		 whereas Application is at SDL_LOG_PRIORITY_INFO. 
		void Log::init() {
			SDL_LogSetPriority(SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_VERBOSE);
			rang::init();
			CONSOLE_RESET_STYLE();
			m_initialized = true;
		}

		/**
		* Usage example:
		*	log::message("printf formatting %s-%d", "abc", 123);
		**/
        void message(const char* format, ...)
        {
			CONSOLE_MESSAGE_STYLE();
			LOG_VA_MSG(format, SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_INFO);
			CONSOLE_RESET_STYLE();
        }

		/**
		* Usage example:
		*	log::warning("printf formatting %s-%d", "abc", 123);
		**/
		void warning(const char* format, ...)
		{
			CONSOLE_WARNING_STYLE();
			LOG_VA_MSG(format, SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_WARN);
			CONSOLE_RESET_STYLE();
		}

		/**
		* Usage example:
		*	log::error("printf formatting %s-%d", "abc", 123);
		**/
		void error(const char* format, ...)
		{
			CONSOLE_ERROR_STYLE();
			LOG_VA_MSG(format, SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_ERROR);
			CONSOLE_RESET_STYLE();
		}

		/**
		* Usage example:
		*	log::debug("printf formatting %s-%d", "abc", 123);
		**/
		void debug(const char* format, ...)
		{
			#if defined(EXA_DEBUG)
				LOG_VA_MSG(format, SDL_LOG_CATEGORY_CUSTOM, SDL_LOG_PRIORITY_DEBUG);
			#endif
		}
	}
}