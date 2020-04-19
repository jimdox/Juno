#pragma once
#include <memory>
namespace Juno
{

class VertexArray
{
public:
    static std::shared_ptr<VertexArray> Generate();
    virtual ~VertexArray();
    unsigned int GetVaoID();
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    

protected:
    unsigned int vaoID;
};


class GLVertexArray : public VertexArray
{
public:
    GLVertexArray();
    ~GLVertexArray();
    void Bind();
    void Unbind();
};


class VKVertexArray : public VertexArray
{
public:
    VKVertexArray();
    ~VKVertexArray();
    void Bind();
    void Unbind();
};
}


