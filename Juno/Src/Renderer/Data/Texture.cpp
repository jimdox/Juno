#include "jnpch.h"
#include "Renderer/Data/Texture.h"

#ifdef JN_RENDERER_OPENGL
    #include "Renderer/Platforms/GLTexture.h"
#else   
    #include "Renderer/Platforms/VKTexture.h"
#endif
#include "Renderer/Platforms/VKTexture.h"


using namespace Juno;

SPtr<Texture> Texture::Create(TextureType texType, bool transparency)
{
    // #ifdef JN_RENDERER_OPENGL
    //     return std::make_shared<GLTexture>(texType, transparency);
    // #else   
        return std::make_shared<VKTexture>(texType, transparency);
    // #endif
}



/* ---------- */


