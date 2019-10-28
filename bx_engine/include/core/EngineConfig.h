#pragma once
#include "pch.h"


#ifdef BX_PLATFORM_WIN
void readConfigFile()
{
	
}



#endif

#ifdef BX_PLATFORM_LINUX
static void readConfigFile()
{

}

#endif

namespace bx {
static float aspectRatio = 1.0f;
static unsigned int anti_aliasing_factor = 4;

static float getAspectRatio()
{
    return aspectRatio;
}

}