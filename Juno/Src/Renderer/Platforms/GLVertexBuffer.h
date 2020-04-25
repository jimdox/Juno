#pragma once

#include <glad/glad.h>
namespace Juno
{
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
}