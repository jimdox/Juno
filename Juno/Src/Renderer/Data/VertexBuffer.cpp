#include "jnpch.h"
#include "VertexBuffer.h"

#ifdef JN_RENDERER_OPENGL
    #include "Renderer/Platforms/GLVertexBuffer.h"
#else 
    #include "Renderer/Platforms/VKVertexBuffer.h"
#endif


using namespace Juno;


VertexBuffer* VertexBuffer::Create(VertexBufferDataType bufferType, VertexBufferUsageType usageType)
{
    #ifdef JN_RENDERER_OPENGL
        return new GLVertexBuffer(bufferType, usageType);
    #else   
        return new VKVertexBuffer(bufferType, usageType);
    #endif
}





/* -------- */

