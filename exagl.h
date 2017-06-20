// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include "glad/glad.h"

#if _WIN32
#   define GL_PREFIX
#else
#   define GL_PREFIX
#endif

#define PASTER(x,y) x ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define DECLARE_GL_EXT(y)  EVALUATOR(GL_PREFIX,y)

#define exaglDrawArrays DECLARE_GL_EXT(glDrawArrays)

#define exaglClear DECLARE_GL_EXT(glClear)

#define exaglClearColor DECLARE_GL_EXT(glClearColor)

#define exaglGetString DECLARE_GL_EXT(glGetString)

#define exaglGenTextures DECLARE_GL_EXT(glGenTextures)

#define exaglTexParameteri DECLARE_GL_EXT(glTexParameteri)
#define exaglTexParameterf DECLARE_GL_EXT(glTexParameterf)
#define exaglTexImage2D DECLARE_GL_EXT(glTexImage2D)
#define exaglBindTexture DECLARE_GL_EXT(glBindTexture)

#define exaglUniform4f DECLARE_GL_EXT(glUniform4f)

#define exaglGenVertexArrays DECLARE_GL_EXT(glGenVertexArrays)
#define exaglDeleteVertexArrays DECLARE_GL_EXT(glDeleteVertexArrays)	
#define exaglBindVertexArray DECLARE_GL_EXT(glBindVertexArray)

#define exaglValidateProgram DECLARE_GL_EXT(glValidateProgram)

#define exaglDetachShader DECLARE_GL_EXT(glDetachShader)

#define exaglEnableVertexAttribArray DECLARE_GL_EXT(glEnableVertexAttribArray)
#define exaglDisableVertexAttribArray DECLARE_GL_EXT(glDisableVertexAttribArray)

#define exaglShaderSource DECLARE_GL_EXT(glShaderSource)
#define exaglUseProgram DECLARE_GL_EXT(glUseProgram)
#define exaglVertexAttribPointer DECLARE_GL_EXT(glVertexAttribPointer)
#define exaglActiveTexture DECLARE_GL_EXT(glActiveTexture)
#define exaglDeleteProgram DECLARE_GL_EXT(glDeleteProgram)
#define exaglGetShaderiv DECLARE_GL_EXT(glGetShaderiv)
#define exaglGetShaderInfoLog DECLARE_GL_EXT(glGetShaderInfoLog)
#define exaglCreateShader DECLARE_GL_EXT(glCreateShader)
#define exaglCreateProgram DECLARE_GL_EXT(glCreateProgram)
#define exaglAttachShader DECLARE_GL_EXT(glAttachShader)
#define exaglCompileShader DECLARE_GL_EXT(glCompileShader)
#define exaglBindAttribLocation DECLARE_GL_EXT(glBindAttribLocation)
#define exaglGetAttribLocation DECLARE_GL_EXT(glGetAttribLocation)
#define exaglLinkProgram DECLARE_GL_EXT(glLinkProgram)
#define exaglUniform1i DECLARE_GL_EXT(glUniform1i)
#define exaglUniform2fv DECLARE_GL_EXT(glUniform2fv)
#define exaglUniform2f DECLARE_GL_EXT(glUniform2f)
#define exaglUniform3fv DECLARE_GL_EXT(glUniform3fv)
#define exaglUniform4fv DECLARE_GL_EXT(glUniform4fv)
#define exaglUniform1f DECLARE_GL_EXT(glUniform1f)
#define exaglUniformMatrix4fv DECLARE_GL_EXT(glUniformMatrix4fv)
#define exaglBindFramebuffer DECLARE_GL_EXT(glBindFramebuffer)
#define exaglGenFramebuffers DECLARE_GL_EXT(glGenFramebuffers)
#define exaglCheckFramebufferStatus DECLARE_GL_EXT(glCheckFramebufferStatus)
#define exaglDeleteFramebuffers DECLARE_GL_EXT(glDeleteFramebuffers)
#define exaglGetUniformLocation DECLARE_GL_EXT(glGetUniformLocation)
#define exaglFramebufferTexture2D DECLARE_GL_EXT(glFramebufferTexture2D)
#define exaglCompressedTexImage2D DECLARE_GL_EXT(glCompressedTexImage2D)
#define exaglBindBuffer DECLARE_GL_EXT(glBindBuffer)
#define exaglGenBuffers DECLARE_GL_EXT(glGenBuffers)
#define exaglBufferData DECLARE_GL_EXT(glBufferData)
#define exaglGetProgramiv DECLARE_GL_EXT(glGetProgramiv)
#define exaglGenerateMipmap DECLARE_GL_EXT(glGenerateMipmap)
#define exaglStencilOpSeparate DECLARE_GL_EXT(glStencilOpSeparate)
#define exaglGenRenderbuffers DECLARE_GL_EXT(glGenRenderbuffers)
#define exaglBindRenderbuffer DECLARE_GL_EXT(glBindRenderbuffer)
#define exaglRenderbufferStorage DECLARE_GL_EXT(glRenderbufferStorage)
#define exaglFramebufferRenderbuffer DECLARE_GL_EXT(glFramebufferRenderbuffer)
#define exaglDeleteRenderbuffers DECLARE_GL_EXT(glDeleteRenderbuffers)
#define exaglDeleteShader DECLARE_GL_EXT(glDeleteShader)
#define exaglDeleteBuffers DECLARE_GL_EXT(glDeleteBuffers)
#define exaglGetProgramInfoLog DECLARE_GL_EXT(glGetProgramInfoLog)