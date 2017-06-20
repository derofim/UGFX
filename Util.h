// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include <type_traits>

template <typename T>
struct vec2{
	T x, y;
};

template <typename T>
struct vec3{
	T x, y, z;
};

typedef vec3<float> vec3f;
typedef vec2<float> vec2f;

// Converts C++ type to OpenGL type
// @see http://stackoverflow.com/questions/28775659/c-interface-for-managing-opengl-vertex-attributes
template <typename T>
struct type_to_gl
{						 
    static_assert( std::is_same<T, void>::value,  "Invalid type!" );
    static constexpr GLenum type = 0;
	static constexpr GLint arity = 0;
};

template <> struct type_to_gl<unsigned char>  
{
	static constexpr GLenum type = GL_UNSIGNED_BYTE;
	static constexpr GLint arity = 1;
};

template <> struct type_to_gl<signed char>
{
	static constexpr GLenum type = GL_BYTE;
	static constexpr GLint arity = 1;
};

template <> struct type_to_gl<unsigned short>
{
	static constexpr GLenum type = GL_UNSIGNED_SHORT;
	static constexpr GLint arity = 1;
};

template <> struct type_to_gl<signed short>
{
	static constexpr GLenum type = GL_SHORT;
	static constexpr GLint arity = 1;
};

template <> struct type_to_gl<unsigned int>
{
	static constexpr GLenum type = GL_UNSIGNED_INT;
	static constexpr GLint arity = 1;
};

template <> struct type_to_gl<signed int>
{
	static constexpr GLenum type = GL_INT;
	static constexpr GLint arity = 1;
};

template <> struct type_to_gl<float>
{
	static constexpr GLenum type = GL_FLOAT;
	static constexpr GLint arity = 1;
};

template <> struct type_to_gl<double>
{
	static constexpr GLenum type = GL_DOUBLE;
	static constexpr GLint arity = 1;
};

template <> struct type_to_gl<vec3f>
{
	static constexpr GLenum type = GL_FLOAT;
	static constexpr GLint arity = 3;
};

template <> struct type_to_gl<vec2f>
{
	static constexpr GLenum type = GL_FLOAT;
	static constexpr GLint arity = 2;
};

template <typename T>
inline void gfxVertexAttribIPointer(GLuint index, GLint size, GLsizei stride, const GLvoid *pointer) {
	static_assert(type_to_gl<T>::type, "Invalid type!");
	exaglVertexAttribIPointer(index, size, type_to_gl<T>::type, stride, pointer);
}

template <typename T>
inline void gfxVertexAttribPointer(GLuint index, GLsizei stride, const GLvoid *pointer) {
	static_assert(type_to_gl<T>::type, "Invalid type!");
	exaglVertexAttribPointer(
		/* Index of the vertex attribute */ index,
		/* Number of components per vertex attribute (from 1 to 4). */ type_to_gl<T>::arity,
		/* Specifies the data type of each component in the array. */ type_to_gl<T>::type,
		/* Normalize to (-1;1) signed or (0;1) unsigned ranges */ GL_FALSE,
		/* Byte offset between vertex attributes. */ stride,
		/* Offset of the first component */  pointer
	);
}