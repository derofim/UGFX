// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include <cstdint>
#include <vector>

#include "exa.h"

namespace exa
{
	class Shader;
	class Texture;
	class VertexBuffer;
	class VertexArray;
	class Window;

	// Engine status
	enum class StatusCode : std::int8_t 
	{ 
		// Not created
		EXA_NONE,
		// Creating and initializing subsystems
		EXA_STARTING,
		// Working fine
		EXA_RUNNING,
		// Pause and may continue working
		EXA_PAUSED,
		// Unhandled or unknown error
		EXA_CRASHED,
		// Known error handled
		EXA_ERROR,
		// Done all job fine
		EXA_DONE,
		EXA_TOTAL_ITEMS
	};

	struct Vertex {
		vec3f Position;
		vec2f TexCoords;
	};

	class Exagine
	{
	public:
		// Singleton in Lazy-thread-safe style.
		static Exagine& Instance()
		{
			static Exagine s;
			return s;
		}

		// Initialize engine subsystems.
		bool init();

		// Start engine. 
		int start();

		// Create and start main loop
		bool loop();

		// Poll events, call subsystems.
		void frame();

		// Free media and shut down SDL.
		bool close();

		// Poll and handle SDL events
		void handleWindowEvents();

		// Draw objects on screen
		void drawScreen();

		void beforeDraw();

		void afterDraw();

		inline bool isRunning() const {
			return 	!m_quit;
		}

		inline void stop(const StatusCode statusCode = StatusCode::EXA_ERROR) 
		{
			log::message("Stopping engine");
			m_quit = true;
			setStatusCode(statusCode);
		}

		// Prevent overriding of status code with wrong values
		// For example, we can`t set "Starting" status if we are in "Error" status 
		bool setStatusCode(const StatusCode statusCode) 
		{
			switch (statusCode) {

				case StatusCode::EXA_STARTING:
				{
					if (m_statusCode != StatusCode::EXA_NONE) {
						log::error("Engine may be initialized only from StatusCode::EXA_NONE");
						setStatusCode(StatusCode::EXA_ERROR);
						return false;
					}
					else {
						m_statusCode = statusCode;
					}
					break;
				}

				case StatusCode::EXA_RUNNING:
				{
					if (m_statusCode != StatusCode::EXA_STARTING) {
						log::error("Engine may be runned only from StatusCode::EXA_STARTING");
						setStatusCode(StatusCode::EXA_ERROR);
						return false;
					}
					else {
						m_statusCode = statusCode;
					}
					break;
				}

				case StatusCode::EXA_DONE:
				{
					if (m_statusCode != StatusCode::EXA_RUNNING) {
						log::error("Engine may be shut down only from StatusCode::EXA_RUNNING");
						setStatusCode(StatusCode::EXA_ERROR);
						return false;
					}
					else {
						m_statusCode = statusCode;
					}
					break;
				}

				default:
					m_statusCode = statusCode;
			}
			return true;
		}

		inline StatusCode getStatusCode() const {
			return m_statusCode;
		}

		// Returns true if some error appeared
		// Used to set application return code
		bool isErrorStatusCode() const {
			return getStatusCode() == StatusCode::EXA_ERROR 
				|| getStatusCode() == StatusCode::EXA_CRASHED;
		}

	private:
		Exagine() {}
		~Exagine() {}

		Exagine(Exagine const&) = delete;
		Exagine& operator= (Exagine const&) = delete;

		// Main application Window
		Window* m_Window = nullptr;

		// Main loop flag.
		bool m_quit = false;
		StatusCode m_statusCode = StatusCode::EXA_NONE;

		// Vertex Buffer Object stores vertex data
		VertexBuffer* m_VBO = nullptr;

		// Vertex Array Object manages VBO
		VertexArray* m_VAO = nullptr;

		// OpenGL GLSL shader
		Shader* m_mainShader = nullptr;

		// OpenGL texture
		Texture* m_texture = nullptr;

		std::vector<Vertex> m_vertices;
	};
}