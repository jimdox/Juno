#pragma once

namespace Juno
{
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