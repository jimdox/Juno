#pragma once
#include "Utils/Memory.h"
#include "Renderer/Data/VertexBuffer.h"
namespace Juno
{
class VertexArray
{
public:
    static VertexArray* Generate();
    virtual ~VertexArray(){}
    unsigned int GetVaoID();
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void EnableAttribute(unsigned int attribNum) = 0;
    virtual void DisableAttribute(unsigned int attribNum) = 0;

protected:
    unsigned int vaoID;
    unsigned int numAttributes;
};
}


