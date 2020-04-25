#pragma once

namespace Juno
{
class VKVertexArray : public VertexArray
{
public:
    VKVertexArray();
    ~VKVertexArray();
    void Bind();
    void Unbind();
    void EnableAttribute(unsigned int attribNum);
    void DisableAttribute(unsigned int attribNum);
};
}