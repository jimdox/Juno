#pragma once


#ifdef JN_PLATFORM_WIN
void ReadConfigFile()
{
	
}



#endif

#ifdef JN_PLATFORM_LINUX
static void ReadConfigFile()
{

}
#endif

namespace Juno {


class ConfigManager 
{
public:
    static ConfigManager& Get()
    {
        static ConfigManager s_instance;
        return s_instance;
    }


private:

};

enum class GraphicsAPI
{
    OpenGL, Vulkan
};

static float g_aspectRatio = 1.0f;
static unsigned int g_antiAliasingFactor = 4;
static unsigned int g_defaultScreen_Width = 1000;
static unsigned int g_default_screen_height = 900;
static const char* const JN_VERSION = "0.1.3d"; 


}