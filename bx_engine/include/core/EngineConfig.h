#pragma once
#include <string>

static float aspectRatio = 1.0f;

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
static float getAspectRatio()
{
    return aspectRatio;
}

}