#pragma once

namespace Juno
{
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
}