// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "Shader.h"

#include <memory>

#include "File.h"
#include "Exagine.h"
#include "Log.h"

namespace exa
{
	void Shader::bindUniform(unsigned int location, glm::vec4 & value)
	{
		exaglUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::bindUniform(unsigned int location, glm::mat4 const & matrix)
	{
		exaglUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::bindUniform(unsigned int location, float value)
	{
		exaglUniform1f(location, value);
	}

	void Shader::bindUniform(unsigned int location, int value)
	{
		exaglUniform1i(location, value);
	}

	void  Shader::activateTexture(GLenum target = GL_TEXTURE_2D, int index = 0,
		GLuint textureGl = 0, const char* uniformName = "textureUniformName")
	{
		if (index < 0 || index > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
			log::error("%s %d %s", "activateTexture", target, ": i < 0 || i > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS");
		}
		exaglActiveTexture(GL_TEXTURE0 + index);
		exaglBindTexture(GL_TEXTURE_2D, textureGl);
		exaglUniform1i(getUniformLocation(uniformName), index);
	}

	void  Shader::activateTexture2D(int index, GLuint textureGl, const char* uniformName)
	{
		activateTexture(GL_TEXTURE_2D, index, textureGl, uniformName);
	}

	void  Shader::activateCubeMapTexture(int index, GLuint textureGl, const char* uniformName)
	{
		activateTexture(GL_TEXTURE_CUBE_MAP, index, textureGl, uniformName);
	}

	/**
	 * Attach the shader source code to the shader object and compile the GLSL shader
	 *
	 * @param type			Shader type (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, e.t.c.)
	 * @param shaderSrc		Shader source code
	 */
	GLuint Shader::loadShaderFromMemory(GLenum type, const char *shaderSrc)
	{
		GLuint shader = exaglCreateShader(type);
		if (shader == GL_ZERO) {
			log::error("Could not create shader");
			return 0;
		}

		exaglShaderSource(
			/* Shader object to compile */ shader,
			/* How many strings we’re passing as source code */ 1,
			/* Actual source code of the vertex shader */ &shaderSrc,
			/* Array of string lengths */ nullptr
		);

		exaglCompileShader(shader);

		VALIDATE_SHADER_IV(shader, GL_COMPILE_STATUS);

		return shader;
	}

	GLint Shader::getAttribLocation(const char* atrName)
	{
		if (m_shaderProgram == 0) {
			log::error("shaderProgram == 0");
			EXAGINE().stop();
		}

		GLint attribLocation = exaglGetAttribLocation(m_shaderProgram, atrName);
		if (attribLocation == -1) {
			log::error("%s %d %s %s", "Could not get attribute location ", attribLocation, " of ", atrName);
			EXAGINE().stop();
		}
		return attribLocation;
	}

	GLint Shader::getUniformLocation(const char* atrName)
	{
		if (m_shaderProgram == 0) {
			log::error("shaderProgram == 0");
			EXAGINE().stop();
		}

		GLint uniformLocation = exaglGetUniformLocation(m_shaderProgram, atrName);
		if (uniformLocation == -1) {
			log::error("%s %d %s %s", "Could not get uniform location ", uniformLocation, " of ", atrName);
			EXAGINE().stop();
		}
		return uniformLocation;
	}

	/**
	* Load shader based on file extention
	* @see Extentions as in OpenGL ES Reference Compiler 
	*      https://www.khronos.org/opengles/sdk/tools/Reference-Compiler/
	* .vert - a vertex shader
	* .tesc - a tessellation control shader
	* .tese - a tessellation evaluation shader
	* .geom - a geometry shader
	* .frag - a fragment shader
	* .comp - a compute shader
	**/
	Shader & Shader::addShader(const char * filename)
	{
		// Get file extention
		std::string filenameStr(filename);
		auto index = filenameStr.rfind(".");
		auto ext = filenameStr.substr(index + 1);

		// Load GLSL Shader Source from File
		std::string path = PROJECT_SHADERS_DIR;
		std::string filefullpath = path + filenameStr;

		log::debug("adding shader %s", filefullpath.c_str());
		
		if (!File::exists(filefullpath.c_str())) {
			log::error("File %s doesn't exist %s:%d", filefullpath.c_str(), __FILE__, __LINE__);
		}

		// Read file										 
		File shaderfile(filefullpath.c_str());

		// Create a Shader Object
		// @note According to the C++ standard a reinterpret_cast between unsigned char* and char* is 
		//		 safe as they are the same size and have the same construction and constraints
		const char * source = reinterpret_cast<const char *>(shaderfile.getBuffer());

		if (ext == "comp") {
			addShader(GL_COMPUTE_SHADER, source);
		}
		else if (ext == "frag") {
			addShader(GL_FRAGMENT_SHADER, source);
		}
		else if (ext == "geom") {
			addShader(GL_GEOMETRY_SHADER, source);
		}
		else if (ext == "vert") {
			addShader(GL_VERTEX_SHADER, source);
		}
		else if (ext == "tesc") {
			addShader(GL_TESS_CONTROL_SHADER, source);
		}
		else if (ext == "tese") {
			addShader(GL_TESS_EVALUATION_SHADER, source);
		}
		else {
			log::error("Unknown shader type");
			EXAGINE().stop();
		}

		log::debug("added shader %s", filefullpath.c_str());

		return *this;
	}

	Shader & Shader::addShader(GLenum type, const char *shaderSrc)
	{
		if (type == GL_VERTEX_SHADER) {
			size_t index = static_cast<size_t>(ShaderType::EXA_VERTEX_SHADER);
			m_shaders[index] = loadShaderFromMemory(type, shaderSrc);
		}
		else if (type == GL_FRAGMENT_SHADER) {
			size_t index = static_cast<size_t>(ShaderType::EXA_FRAGMENT_SHADER);
			m_shaders[index] = loadShaderFromMemory(type, shaderSrc);
		}
		else if (type == GL_GEOMETRY_SHADER) {
			size_t index = static_cast<size_t>(ShaderType::EXA_GEOMETRY_SHADER);
			m_shaders[index] = loadShaderFromMemory(type, shaderSrc);
		}
		else if (type == GL_COMPUTE_SHADER) {
			size_t index = static_cast<size_t>(ShaderType::EXA_COMPUTE_SHADER);
			m_shaders[index] = loadShaderFromMemory(type, shaderSrc);
		}
		else if (type == GL_TESS_CONTROL_SHADER) {
			size_t index = static_cast<size_t>(ShaderType::EXA_TESS_CONTROL_SHADER);
			m_shaders[index] = loadShaderFromMemory(type, shaderSrc);
		}
		else if (type == GL_TESS_EVALUATION_SHADER) {
			size_t index = static_cast<size_t>(ShaderType::EXA_TESS_EVALUATION_SHADER);
			m_shaders[index] = loadShaderFromMemory(type, shaderSrc);
		}
		else {
			log::error("Unsupported shader type");
			EXAGINE().stop();
		}
		return *this;
	}

	Shader::Shader()
	{
		m_shaderProgram = 0;
		init();
	}

	void Shader::init()
	{
		// Creates a program and returns the ID reference to the newly created program object
		m_shaderProgram = exaglCreateProgram();
		if (m_shaderProgram == GL_ZERO) {
			log::error("Could not create shader program");
			return;
		}
	}

	Shader & Shader::link()
	{
		log::debug("linking shaders");

		size_t index = static_cast<size_t>(ShaderType::EXA_VERTEX_SHADER);
		if (m_shaders[index] == 0) {
			log::error("At least vertex shader must be set!");
		}

		for (unsigned int i = 0; i < m_shaders.size(); i++) {
			exaglAttachShader(m_shaderProgram, m_shaders[i]);
		}

		exaglLinkProgram(m_shaderProgram);
		VALIDATE_PROGRAM_IV(GL_LINK_STATUS);

		exaglValidateProgram(m_shaderProgram);
		VALIDATE_PROGRAM_IV(GL_VALIDATE_STATUS);

		// Delete the shader objects once we’ve linked them into the program object cause
		// we no longer need them anymore
		for (unsigned int i = 0; i < m_shaders.size(); i++) {
			if (m_shaders[i] > 0) {
				//Always detach shaders after a successful link.
				exaglDetachShader(m_shaderProgram, m_shaders[i]);
				exaglDeleteShader(m_shaders[i]);
			}
		}
		return *this;
	}

	Shader::~Shader()
	{
		if (m_shaderProgram == 0) {
			log::error("shaderProgram == 0");
			EXAGINE().stop();
		}

		// Delete shaders
		// @note: If a shader object to be deleted is attached to a program object, it will be 
		// flagged for deletion, but it will not be deleted until it is no longer attached
		for (unsigned int i = 0; i < m_shaders.size(); i++) {
			if (m_shaders[i] > 0) {
				exaglDetachShader(m_shaderProgram, m_shaders[i]);
				exaglDeleteShader(m_shaders[i]);
			}
		}

		if (m_shaderProgram > 0)
		{
			exaglDeleteProgram(m_shaderProgram);
			m_shaderProgram = 0;
		}
	}

	/**
	* Check shader for errors (logs errors)	by calling glGetShaderiv
	*
	* @param shader			Shader ID
	* @param flag			Shader check type (GL_COMPILE_STATUS, e.t.c.)
	* @param file			Filename for printing error (__FILE__)
	* @param line			Line number for printing error (__LINE__)
	*/
	bool Shader::validateShaderiv(GLuint shader, GLuint flag, const char *file, int line)
	{
		GLint success = GL_ZERO;
		exaglGetShaderiv(shader, flag, &success);
		if (success == GL_FALSE)
		{
			GLint logLength = 0;
			exaglGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 1)
			{
				auto infoLog = std::unique_ptr<char>(exanew char[logLength]);
				exaglGetShaderInfoLog(shader, logLength, nullptr, infoLog.get());
				log::error("Error linking shader %s %s %s", infoLog.get(), file, line);
			}
			if (shader > 0) {
				exaglDeleteShader(shader);
			}
			log::error("Error validating shader %s %d", file, line);
			EXAGINE().stop();
			return false;
		}
		return true;
	}


	/**
	* Check program for errors (logs errors) by calling glGetProgramiv
	*
	* @param shader			Shader ID
	* @param flag			Shader check type (GL_COMPILE_STATUS, e.t.c.)
	* @param file			Filename for printing error (__FILE__)
	* @param line			Line number for printing error (__LINE__)
	*/
	bool Shader::validateProgramiv(GLuint shaderProgram, GLuint flag, const char *file, int line)
	{
		GLint success = GL_ZERO;
		exaglGetProgramiv(shaderProgram, flag, &success);
		if (success == GL_FALSE)
		{
			GLint logLength = 0;
			exaglGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 1)
			{
				auto infoLog = std::unique_ptr<char>(exanew char[logLength]);
				exaglGetProgramInfoLog(shaderProgram, logLength, nullptr, infoLog.get());
				log::error("Error linking shader program %s %s %d", infoLog.get(), file, line);
			}
			if (shaderProgram > 0) {
				exaglDeleteProgram(shaderProgram);
			}
			// Don't leak shaders.
			// @see https://www.opengl.org/wiki/Shader_Compilation
			for (unsigned int i = 0; i < m_shaders.size(); i++) {
				if (m_shaders[i] > 0) {
					exaglDetachShader(shaderProgram, m_shaders[i]);
					exaglDeleteShader(m_shaders[i]);
				}
			}
			log::error("Error validating shader %s %d", file, line);
			EXAGINE().stop();
			return false;
		}
		return true;
	}

	bool Shader::validateiv(GLuint flag, const char * file, int line)
	{
		if (m_shaderProgram == 0) {
			log::error("shaderProgram == 0");
			EXAGINE().stop();
		}

		return validateProgramiv(m_shaderProgram, flag, file, line);
	}

	// Calls glUseProgram with current program
	void Shader::bind()
	{
		if (m_shaderProgram == 0) {
			log::error("shaderProgram == 0");
			EXAGINE().stop();
		}

		// ValidateProgram should be called just before glDrawArrays or glDrawElements. 
		// Test whether the current 'context' is valid for drawing actions 
		exaglValidateProgram(m_shaderProgram);

		VALIDATE_PROGRAM_IV(GL_VALIDATE_STATUS);

		exaglUseProgram(m_shaderProgram);
	}

	// Calls glUseProgram(0) 
	void Shader::unbind()
	{
		exaglUseProgram(0);
	}
}
