// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include "RenderPlatforms.h"

#define LOGGER() log::Log::Instance()

#define LOG_VA_MSG(format, category, priority) va_list args; \
va_start(args, format);	\
LOGGER().logVaSdl(args, format, category, priority); \
va_end(args);

namespace exa
{
    namespace log
    {
		/**
		* Outputs message with info priority, has end line.
		**/
		void message(const char* format, ...);

		/**
		* Outputs message with warning priority, has end line.
		**/
		void warning(const char* format, ...);

		/**
		* Outputs message with error priority, has end line.
		**/
		void error(const char* format, ...);

		/**
		* Outputs message with debug priority, has end line.
		* @note Message will be printed if DEBUG defined and NDEBUG not defined.
		**/
		void debug(const char* format, ...);

		class Log
		{
		public:

			// Singleton in Lazy-thread-safe style.
			static Log& Instance()
			{
				static Log s;
				return s;
			}

			/**
			* By default logging is enabled.
			**/
			void enable()
			{
				m_enabled = true;
			}

			/**
			* Turns logging off
			**/
			void disable()
			{
				m_enabled = false;
			}

			bool isEnabled() const {
				return m_enabled;
			}

			/**
			* @note Each passed value will be logged at new line!
			* @note Instead prefer calling simple functions like "log::message("%s %d", "abc", 123)"
			* Usage example:
			*	LOGGER() << 123 << 456.1 << "abc";
			**/
			template <typename T>
			Log &operator<<(const T &a) {
				display(a);
				return *this;
			}

			// @see Available formats https://github.com/davidsiaw/SDL2/blob/c315c99d46f89ef8dbb1b4eeab0fe38ea8a8b6c5/src/stdlib/SDL_string.c#L1479
			template<typename T>
			auto display(T const& t) -> decltype(t.display(), void())
			{
				log::debug("display(T const& t)");
			}

			void display(int data)
			{
				logSdl("%d", m_category, m_priority, data);
			}

			void display(bool data)
			{
				logSdl("%s", m_category, m_priority, data ? "true" : "false");
			}

			void display(double data)
			{
				logSdl("%f", m_category, m_priority, data);
			}

			void display(const char* data)
			{
				logSdl("%s", m_category, m_priority, data);
			}

			void logVaSdl(va_list args,
				const char* format = "%s",
				int category = SDL_LOG_CATEGORY_APPLICATION,
				SDL_LogPriority priority = SDL_LogPriority::SDL_LOG_PRIORITY_INFO);

			void logSdl(const char* format,
				int category,
				SDL_LogPriority priority, ...);

			void init();

		private:
			Log() {
				init();
			}
			~Log() {}

			Log(Log const&) = delete;
			Log& operator= (Log const&) = delete;

		private:
			bool m_enabled = true;

			// Will	be set to true if log initialized successfully.
			bool m_initialized = false;

			int m_category = SDL_LOG_CATEGORY_CUSTOM;

			SDL_LogPriority m_priority = SDL_LOG_PRIORITY_INFO;
		};
    }
}