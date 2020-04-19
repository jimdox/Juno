#include "jnpch.h"
#include "VertexBuffer.h"
#include "Renderer/Renderer.h"

using namespace Juno;


std::unique_ptr<VertexBuffer> VertexBuffer::Create(VertexBufferDataType bufferType, VertexBufferUsageType usageType)
{
    if(Renderer::Get().GetCurrentRendererAPI() == RendererAPI::Vulkan)
    {
        return std::make_unique<VKVertexBuffer>(bufferType, usageType);
    } else {
        return std::make_unique<GLVertexBuffer>(bufferType, usageType);
    }
}

GLVertexBuffer::GLVertexBuffer(VertexBufferDataType bufferType, VertexBufferUsageType bufferUsage)
{
    if(bufferType != VertexBufferDataType::ELEMENT_BUFFER)
    {
        this->bufferDataType = GL_ARRAY_BUFFER;
    } else {
        this->bufferDataType = GL_ELEMENT_ARRAY_BUFFER;
    }
    switch(bufferUsage)
    {
    case VertexBufferUsageType::STATIC_DRAW:
        this->bufferUsageType = GL_STATIC_DRAW;
        break;
    case VertexBufferUsageType::DYNAMIC_DRAW:
        this->bufferUsageType = GL_DYNAMIC_DRAW;
        break;
    case VertexBufferUsageType::STREAM_DRAW:
        this->bufferUsageType = GL_STREAM_DRAW;
        break;
    case VertexBufferUsageType::STATIC_COPY:
        this->bufferUsageType = GL_STATIC_COPY;
        break;
    case VertexBufferUsageType::DYNAMIC_COPY:
        this->bufferUsageType = GL_DYNAMIC_COPY;
        break;
    case VertexBufferUsageType::STREAM_COPY:
        this->bufferUsageType = GL_STREAM_COPY;
        break;
    case VertexBufferUsageType::STATIC_READ:
        this->bufferUsageType = GL_STATIC_READ;
        break;
    case VertexBufferUsageType::DYNAMIC_READ:
        this->bufferUsageType = GL_DYNAMIC_READ;
        break;
    case VertexBufferUsageType::STREAM_READ:
        this->bufferUsageType = GL_STREAM_READ;
        break;  
    };

    glGenBuffers(1, &vboID);
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &vboID);
}

void GLVertexBuffer::Bind()
{
    glBindBuffer(bufferDataType, vboID);
}
void GLVertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVertexBuffer::StoreData(unsigned int attribNum, unsigned int dim, std::vector<float>& data)
{
    glBufferData(bufferDataType, data.size() * sizeof(float), &data[0], bufferUsageType);
    glVertexAttribPointer(attribNum, dim, GL_FLOAT, GL_FALSE, 0, (void*)0);                 /* will need to accomodate more types of data in future */
}



/* -------- */

VKVertexBuffer::VKVertexBuffer(VertexBufferDataType bufferType, VertexBufferUsageType bufferUsage)
{
    // this->bufferType = bufferType;
    // this->bu

}

VKVertexBuffer::~VKVertexBuffer()
{

}
void VKVertexBuffer::Bind()
{

}
void VKVertexBuffer::Unbind()
{

}
void VKVertexBuffer::StoreData(unsigned int attribNum, unsigned int dim, std::vector<float>& data)
{

}