// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "Exagine.h"

#include <iostream>

#include "Util.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Log.h"
#include "Window.h"
#include "Image.h"

namespace exa
{
	bool Exagine::close()
	{
		SafeDelete(m_VAO);
		SafeDelete(m_VBO);
		SafeDelete(m_mainShader);
		SafeDelete(m_texture);

		// Delete window and quit SDL
		// @note Must be called last
		SafeDelete(m_Window);

		if (!setStatusCode(StatusCode::EXA_DONE)) {
			return false;
		}

		return true;
	}

	void Exagine::handleWindowEvents()
	{
		m_Window->pollEvents();
	}

	void Exagine::drawScreen()
	{
		m_mainShader->bind();

		m_mainShader->bindUniform("color", glm::vec4(1, 0, 1, 1));

		m_mainShader->activateTexture2D(0, m_texture->get(), "diffuseTexture");

		m_VAO->bind();

		exaglDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

		exaglBindVertexArray(0);

		m_mainShader->unbind();
	}

	void Exagine::beforeDraw()
	{
		// Clear screen
		exaglClear(GL_COLOR_BUFFER_BIT);
	}

	void Exagine::afterDraw()
	{
		// Update window
		m_Window->swapWindow();
	}

	void Exagine::frame()
	{
		handleWindowEvents();

		beforeDraw();

		drawScreen();

		afterDraw();
	}

	bool Exagine::init()
	{
		// Set locale for language localization. Can be "" for the user-preferred locale 
		// @note Source code files must be in UTF-8 without BOM encoding
		setlocale(LC_ALL, "");

		// Initialize logger
		LOGGER();

		if (!setStatusCode(StatusCode::EXA_STARTING)) {
			return false;
		}

		log::debug("Initializing engine subsystems\n\n");

		m_Window = new Window();

		if (!m_Window->init()) {
			return false;
		}

		m_vertices.push_back({ vec3f{ -1.0f, -1.0f, 0.0f }, vec2f{ 0.0f, 0.0f } }); // Bottom Left
		m_vertices.push_back({ vec3f{ -1.0f, 1.0f, 0.0f },  vec2f{ 0.0f, 1.0f } });	// Top Left 
		m_vertices.push_back({ vec3f{ 1.0f, -1.0f, 0.0f },  vec2f{ 1.0f, 0.0f } });	// Bottom Right

		m_vertices.push_back({ vec3f{ 1.0f, -1.0f, 0.0f },  vec2f{ 1.0f, 0.0f } });	// Bottom Right
		m_vertices.push_back({ vec3f{ -1.0f, 1.0f, 0.0f },  vec2f{ 0.0f, 1.0f } });	// Top Left 
		m_vertices.push_back({ vec3f{ 1.0f, 1.0f, 0.0f },   vec2f{ 1.0f, 1.0f } }); // Top Right

		m_mainShader = exanew Shader();
		m_mainShader->addShader("main.vert");

		m_mainShader->addShader("main.frag");
			m_mainShader->link();

		m_VAO = exanew VertexArray();

		m_VAO->bind();

		m_VBO = exanew VertexBuffer();
		m_VBO->setData(m_vertices.data(), m_vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

		m_mainShader->attribute<Vertex, vec3f>("vPosition", &Vertex::Position);
		m_mainShader->attribute<Vertex, vec2f>("texCoord", &Vertex::TexCoords);

		m_VBO->unbind(); // Unbind VBO

		m_VAO->unbind(); // Unbind VAO

		m_texture = exanew Texture("smiley.png");

		// Set OpenGL clear color
		exaglClearColor(0.5, 0.5, 0.5, 1);

		log::debug("Initialized all engine subsystems");

		return true;
	}

	bool Exagine::loop() 
	{
		if (!setStatusCode(StatusCode::EXA_RUNNING)) {
			return false;
		}

		while (!m_quit)
		{
			frame();
		}

		return true;
	}

	int Exagine::start()
	{
		// Start up SDL and create window
		if (!init())
		{
			log::error("Failed to initialize!");
			return 1;
		}

		// Create and start main loop
		if (!loop())
		{
			log::error("Failed to start main loop!");
			return 1;
		}

		// Free resources and close SDL
		if (!close())
		{
			log::error("Failed to free resources and close window!");
			return 1;
		}

		// Some error appeared
		if (isErrorStatusCode()) {
			return 1;
		}

		return 0;
	}
}
