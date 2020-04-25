#pragma once

namespace Juno
{
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