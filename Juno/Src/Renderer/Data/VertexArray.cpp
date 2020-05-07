#include "jnpch.h"
#include "VertexArray.h"

// #ifdef JN_RENDERER_OPENGL
//     #include "Renderer/Platforms/GLVertexArray.h"
// #else 
    #include "Renderer/Platforms/VKVertexArray.h"
//#endif

using namespace Juno;


VertexArray* VertexArray::Generate()
{

    // #ifdef JN_RENDERER_OPENGL
    //     return new GLVertexArray();
    // #else 
        return new VKVertexArray();
//    #endif
}

unsigned int VertexArray::GetVaoID()
{
    return this->vaoID;
}

