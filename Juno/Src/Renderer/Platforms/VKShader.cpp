#include "jnpch.h"
#include "Renderer/Data/Shader.h"
#include "Renderer/Platforms/VKShader.h"

using namespace Juno;

VKShader::VKShader(std::array<ShaderComponentType, 3>& components, const std::string& fp)
{

}

VKShader::~VKShader()
{

}

void VKShader::Bind()
{

}

void VKShader::Unbind()
{

}

void VKShader::Compile()
{

}

void VKShader::BindAttribute(unsigned int attribID, const std::string& varName)
{

}

unsigned int VKShader::GetUniformLocation(const std::string& varName)
{
    return 0;
}


void VKShader::LoadInt(unsigned int loc, int value)
{

}

void VKShader::LoadBool(unsigned int loc, bool flag)
{

}

void VKShader::LoadFloat(unsigned int loc, float value)
{

}

void VKShader::LoadFloat2(unsigned int loc, const Vec2& vec)
{

}	

void VKShader::LoadFloat3(unsigned int loc, const Vec3& vec)
{

}

void VKShader::LoadFloat4(unsigned int loc, const Vec4& vec)
{

}

void VKShader::LoadMat2(unsigned int loc, const Mat2& mat)
{

}

void VKShader::LoadMat3(unsigned int loc, const Mat3& mat)
{

}

void VKShader::LoadMat4(unsigned int loc, const Mat4& mat)
{

}


bool VKShader::LinkErrorCheck(unsigned int id, ShaderComponentType componentType)
{
    return false;
}