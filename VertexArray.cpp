// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "VertexArray.h"

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/*
** If you're using VAOs, then you should not disable attribute arrays, as they are encapsulated in the VAO.
** No need to exaglDisableVertexAttribArray(vPositionAttrib);
*/

namespace exa
{
	VertexArray::VertexArray()
	{
		exaglGenVertexArrays(1, &m_VAO);
	}

	VertexArray::~VertexArray()
	{
		exaglDeleteVertexArrays(1, &m_VAO);
	}

	void VertexArray::bind() {
		exaglBindVertexArray(m_VAO);
	}

	void VertexArray::unbind() {

		exaglBindVertexArray(0);
	}
}