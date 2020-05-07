#include "jnpch.h"
#include "Renderer/Data/Shader.h"
#include "Core/AssetManager.h"
#include <glad/glad.h>

// #ifdef JN_RENDERER_OPENGL
//     #include "Renderer/Platforms/GLShader.h"
// #else 
    #include "Renderer/Platforms/VKShader.h"
//#endif

using namespace Juno;

SPtr<Shader> Shader::Create(std::array<ShaderComponentType, 3>& components, const std::string& fp)
{
    // #ifdef JN_RENDERER_OPENGL
    //     return std::make_shared<GLShader>(components, fp);
    // #else   
        return std::make_shared<VKShader>(components, fp);
//    #endif
}


