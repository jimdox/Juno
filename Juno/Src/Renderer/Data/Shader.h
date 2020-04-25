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

class GLShader : public Shader
{
public:
	GLShader(std::array<ShaderComponentType, 3>& components, const std::string& fp);
	~GLShader();

    void Bind();
    void Unbind();
    void Compile();
    void BindAttribute(unsigned int attribID, const std::string& varName);
    unsigned int GetUniformLocation(const std::string& varName);



	void LoadInt(unsigned int loc, int value);
	void LoadBool(unsigned int loc, bool flag);
	void LoadFloat(unsigned int loc, float value);
	void LoadFloat2(unsigned int loc, const Vec2& vec);	
	void LoadFloat3(unsigned int loc, const Vec3& vec);
	void LoadFloat4(unsigned int loc, const Vec4& vec);
	void LoadMat2(unsigned int loc, const Mat2& mat);
	void LoadMat3(unsigned int loc, const Mat3& mat);
	void LoadMat4(unsigned int loc, const Mat4& mat);

protected:
	unsigned int shaderID;
	unsigned int BuildComponent(const std::string& fp, ShaderComponentType componentType);
    bool LinkErrorCheck(unsigned int id, ShaderComponentType componentType);
	
};

class VKShader : public Shader
{
public:
	VKShader(std::array<ShaderComponentType, 3>& components, const std::string& fp);
	~VKShader();
    
	void Bind();
    void Unbind();
    void Compile();
    void BindAttribute(unsigned int attribID, const std::string& varName);
    unsigned int GetUniformLocation(const std::string& varName);

	void LoadInt(unsigned int loc, int value);
	void LoadBool(unsigned int loc, bool flag);
	void LoadFloat(unsigned int loc, float value);
	void LoadFloat2(unsigned int loc, const Vec2& vec);	
	void LoadFloat3(unsigned int loc, const Vec3& vec);
	void LoadFloat4(unsigned int loc, const Vec4& vec);
	void LoadMat2(unsigned int loc, const Mat2& mat);
	void LoadMat3(unsigned int loc, const Mat3& mat);
	void LoadMat4(unsigned int loc, const Mat4& mat);

protected:
    bool LinkErrorCheck(unsigned int id, ShaderComponentType componentType);


};
}