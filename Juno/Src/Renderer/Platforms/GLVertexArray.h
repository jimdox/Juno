#pragma once

namespace Juno
{
class GLVertexArray : public VertexArray
{
public:
    GLVertexArray();
    ~GLVertexArray();
    void Bind();
    void Unbind();
    void EnableAttribute(unsigned int attribNum);
    void DisableAttribute(unsigned int attribNum);

private:

};
}