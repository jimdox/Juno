#pragma once
#include "pch.h"


#ifdef JN_PLATFORM_WIN
void readConfigFile()
{
	
}



#endif

#ifdef JN_PLATFORM_LINUX
static void readConfigFile()
{

}

#endif

namespace juno {
static float aspectRatio = 1.0f;
static unsigned int anti_aliasing_factor = 4;
static unsigned int default_screen_width = 1000;
static unsigned int default_screen_height = 900;

static float getAspectRatio()
{
    return aspectRatio;
}

}