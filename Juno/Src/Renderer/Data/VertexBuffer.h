#pragma once
#include "Utils/Memory.h"
#include <vector>
#include <glad/glad.h>
namespace Juno
{
enum class VertexBufferDataType : unsigned char
{
    ARRAY_BUFFER,  
    ELEMENT_BUFFER
};

enum class VertexBufferUsageType : unsigned char
{
    STATIC_DRAW, DYNAMIC_DRAW, STREAM_DRAW,
    STATIC_COPY, DYNAMIC_COPY, STREAM_COPY,
    STATIC_READ, DYNAMIC_READ, STREAM_READ
};


class VertexBuffer
{
public:
    static VertexBuffer* Create(VertexBufferDataType bufferType, VertexBufferUsageType bufferUsage);

    virtual ~VertexBuffer(){}

    unsigned int getVboID();
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void StoreDataInAttribute(unsigned int attribNum, unsigned int dim, std::vector<float>& data) = 0;

protected:
    unsigned int vboID;
};

class GLVertexBuffer : public VertexBuffer
{
public:
    GLVertexBuffer(VertexBufferDataType bufferType, VertexBufferUsageType bufferUsage);
    ~GLVertexBuffer() override;
    void Bind();
    void Unbind();
    void StoreDataInAttribute(unsigned int attribNum, unsigned int dim, std::vector<float>& data);
    GLenum bufferDataType;
    GLenum bufferUsageType;
};

class VKVertexBuffer : public VertexBuffer
{
public:
    VKVertexBuffer(VertexBufferDataType bufferType, VertexBufferUsageType bufferUsage);
    ~VKVertexBuffer() override;
    void Bind();
    void Unbind();
    void StoreDataInAttribute(unsigned int attribNum, unsigned int dim, std::vector<float>& data);
};
}