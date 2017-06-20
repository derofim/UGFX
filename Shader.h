// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include <array>

#include "RenderPlatforms.h"
#include "Util.h"
#include "Log.h"
									   
#define VALIDATE_PROGRAM_IV(x) validateiv(x, __FILE__, __LINE__)
#define VALIDATE_SHADER_IV(x, y) validateShaderiv(x, y, __FILE__, __LINE__)

namespace exa
{
	// Internal shader types 
	enum class ShaderType : std::int8_t
	{
		EXA_VERTEX_SHADER,
		EXA_FRAGMENT_SHADER,
		EXA_GEOMETRY_SHADER,
		EXA_COMPUTE_SHADER,
		EXA_TESS_CONTROL_SHADER,
		EXA_TESS_EVALUATION_SHADER,
		EXA_TOTAL_ITEMS
	};

	class Shader
	{
	private:
		GLuint m_shaderProgram = 0;

		// Shaders: Vertex, fragment, geometry, e.t.c.
		std::array<GLuint, static_cast<size_t>(ShaderType::EXA_TOTAL_ITEMS)> m_shaders{ 0 };

	public:

		Shader();
		~Shader();


		// Calls loadShaderFromMemory based on type passed
		Shader & addShader(GLenum type, const char * shaderSrc);

		// Reads shader from file and calls loadShaderFromMemory based on filename passed
		Shader & addShader(const char * filename);

		// Calls glLinkProgram and glAttachShader 
		Shader & link();

		// Use the program
		void bind();
		void unbind();

		template<typename T> Shader & bindUniform(const char * name, T&& value)
		{
			int location = getUniformLocation(name);
			bindUniform(location, std::forward<T>(value));
			return *this;
		}

		void activateTexture(GLenum target, int index, GLuint textureGl, const char* uniformName);

		void activateTexture2D(int index, GLuint textureGl, const char * uniformName);

		void activateCubeMapTexture(int index, GLuint textureGl, const char * uniformName);

		GLint getAttribLocation(const char* atrName);

		GLint getUniformLocation(const char* atrName);

		// http://stackoverflow.com/questions/22729054/get-member-offset-in-a-template-for-glvertexattribpointer
		template <class Vertex, typename Element>
		void attribute(const char* name, Element Vertex::* memberPointer)
		{
			static_assert(std::is_pod<Vertex>::value, "Error: not a POD");
			static_assert(std::is_standard_layout<Vertex>::value, "Error: not a standard layout");

			GLint handle = getAttribLocation(name);
			exaglEnableVertexAttribArray(handle);
			union
			{
				Element Vertex::* x;
				GLvoid* y;
			}
			offset;

			offset.x = memberPointer;

			exaglVertexAttribPointer(
				handle,
				type_to_gl<Element>::arity,
				type_to_gl<Element>::type,
				GL_FALSE,
				sizeof(Vertex),
				offset.y
			);
		}

	private:

		// Checks call to glGetShaderiv(shader, flag, &success)
		bool validateShaderiv(GLuint shader, GLuint flag, const char *file, int line);

		// Checks call to glGetProgramiv(shaderProgram, flag, &success)
		bool validateProgramiv(GLuint shaderProgram, GLuint flag, const char *file, int line);

		// Shortcut to validateProgram, validates current program
		bool validateiv(GLuint flag, const char *file, int line);

		GLuint loadShaderFromMemory(GLenum type, const char * shaderSrc);

		void bindUniform(unsigned int location, glm::vec4 & value);

		void bindUniform(unsigned int location, float value);

		void bindUniform(unsigned int location, glm::mat4 const & matrix);

		void bindUniform(unsigned int location, int value);

		void init();
	};
}