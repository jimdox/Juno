#pragma once
#include "Utils/Memory.h"
#include "Utils/Math.h"
namespace Juno
{
enum class ShaderComponentType { Empty, Vertex, Fragment, Geometry, Compute };

class Shader
{
public:
    static SPtr<Shader> Create(std::array<ShaderComponentType, 3>& components, const std::string& fp);
    virtual ~Shader(){}

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void Compile() = 0;
    virtual void BindAttribute(unsigned int attribID, const std::string& varName) = 0;
    virtual unsigned int GetUniformLocation(const std::string& varName) = 0;



	virtual void LoadInt(unsigned int loc, int value) = 0;
	virtual void LoadBool(unsigned int loc, bool flag) = 0;
	virtual void LoadFloat(unsigned int loc, float value) = 0;
	virtual void LoadFloat2(unsigned int loc, const Vec2& vec) = 0;	
	virtual void LoadFloat3(unsigned int loc, const Vec3& vec) = 0;
	virtual void LoadFloat4(unsigned int loc, const Vec4& vec) = 0;
	virtual void LoadMat2(unsigned int loc, const Mat2& mat) = 0;
	virtual void LoadMat3(unsigned int loc, const Mat3& mat) = 0;
	virtual void LoadMat4(unsigned int loc, const Mat4& mat) = 0;

protected:
    virtual bool LinkErrorCheck(unsigned int id, ShaderComponentType componentType) = 0;

};
}