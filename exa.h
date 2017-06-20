// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#define EXAGINE() Exagine::Instance()

#define PROJECT_SHADERS_DIR "assets/shaders/"
#define PROJECT_IMAGES_DIR "assets/images/"

#if defined(_DEBUG) && !defined(NDEBUG)
#   define EXA_DEBUG 1
#endif

#include "RenderPlatforms.h"
#include "Memory.h"
#include "Util.h"
#include "Log.h"
#include "Types.h"