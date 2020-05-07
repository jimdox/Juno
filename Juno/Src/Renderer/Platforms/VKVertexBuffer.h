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
    virtual void StoreElements(std::vector<uint32_t>& data);
    void StoreDataInAttribute(unsigned int attribNum, unsigned int dim, std::vector<float>& data);

    
};
}