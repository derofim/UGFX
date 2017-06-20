// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "Exagine.h"

using namespace exa;

int main(int argc, char** argv)
{
	Exagine& engine = EXAGINE();
	int result = engine.start();
	return result;
}
