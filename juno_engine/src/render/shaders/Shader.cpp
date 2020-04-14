#include "render/shaders/Shader.h"
#include "core/AssetManager.h"
#include "utils/MathUtils.h"

using namespace juno;

Shader::Shader()
{

}

Shader::Shader(const std::string& filepath)
{
	this->filepath = filepath;
	compileShader();
	cacheUniformLocations();
}

Shader::~Shader()
{

}

void Shader::compileShader()
{
	GLuint vertShader = AssetManager::get().loadShaderComponentFile(filepath, GL_VERTEX_SHADER);
	GLuint fragShader = AssetManager::get().loadShaderComponentFile(filepath, GL_FRAGMENT_SHADER);

	// GLint *shaderIDs = AssetManager::get().loadShaderFile(filepath);
	// GLuint vertShader = shaderIDs[0];
	// GLuint fragShader = shaderIDs[1];

	//JN_INFO("Linking shaders");
	this->progID = glCreateProgram();

	glAttachShader(progID, vertShader);
	glAttachShader(progID, fragShader);
	bindAllAttribs();
	glLinkProgram(progID);

	linkErrorCheck(vertShader, GL_VERTEX_SHADER);
	linkErrorCheck(fragShader, GL_FRAGMENT_SHADER);
}

bool Shader::linkErrorCheck(GLuint id, GLenum type)
{
	int errorLength;
	GLint linkStatus;
	glGetProgramiv(progID, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0)
	{
		std::vector<char> errorMsg(errorLength + 1);
		glGetShaderInfoLog(id, errorLength, NULL, &errorMsg[0]);
		std::string message(errorMsg.begin(), errorMsg.end());

		std::string shaderTypeName;
		if(type == GL_VERTEX_SHADER)
		{
			shaderTypeName = "Vertex";
		} else if(type == GL_FRAGMENT_SHADER)
		{
			shaderTypeName = "Fragment";
		} else if(type == GL_COMPUTE_SHADER)
		{
			shaderTypeName = "Compute";
		} else if(type == GL_GEOMETRY_SHADER)
		{
			shaderTypeName = "Geometry";
		}

		JN_CLI_CRIT("Error trying to link {} Shader!", shaderTypeName);
		JN_CLI_TRACE(message);
		//std::cerr << message;
		return true;
	}
	return false;
}

GLuint Shader::getID()
{
	const GLuint p = progID;
	return p;
}

std::string& Shader::getFilepath()
{
	return filepath;
}

/* binds shader */
void Shader::setActive()
{
	glUseProgram(progID);
}

void Shader::unbindProgram()
{
	glUseProgram(0);
}

void Shader::bindAllAttribs()
{
	bindAttrib(0, "position");
	bindAttrib(1, "normal");
	bindAttrib(2, "texCoordinates");
	
}

void Shader::bindAttrib(unsigned int attrib, const std::string& var)
{
	glBindAttribLocation(this->progID, attrib, var.c_str());
}

/* caches all uniform variables' locations in memory */
void Shader::cacheUniformLocations()
{
	loc_transformationMatrix = glGetUniformLocation(progID, "model");
	loc_projectionMatrix = glGetUniformLocation(progID, "projection");
	loc_viewMatrix = glGetUniformLocation(progID, "view");
	loc_reflectivity = glGetUniformLocation(progID, "reflectivity");
	loc_shineDamper = glGetUniformLocation(progID, "shineDamper");
	loc_baseColor = glGetUniformLocation(progID, "in_baseColor");

	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		std::string location("lightPosition["+std::to_string(i)+"]");
		loc_lightPositions[i] = glGetUniformLocation(progID, location.c_str());
		
		location = std::string("lightColor["+std::to_string(i)+"]");
		loc_lightColors[i] = glGetUniformLocation(progID, location.c_str());
		
		location = std::string("attenuations["+std::to_string(i)+"]");
		loc_attenuations[i] = glGetUniformLocation(progID, location.c_str());
	}
}

void Shader::loadTransformMatrix(glm::mat4& transform)
{
	loadMat4(loc_transformationMatrix, transform);
}

void Shader::loadProjectionMatrix(glm::mat4& projection)
{
	loadMat4(loc_projectionMatrix, projection);
}

void Shader::loadViewMatrix(Camera* camera)
{
	glm::mat4 viewMat = juno::generateViewMatrix(camera);
	loadMat4(loc_viewMatrix, viewMat);

}

/* lighting variables */
void Shader::loadLightUniforms(std::vector<Light> &lights)
{
	for(unsigned int i = 0; i < NUM_LIGHTS; i++)
	{
		if(i < lights.size())
		{
			loadFloat3(loc_lightPositions[i], lights[i].getPosition());
			loadFloat3(loc_lightColors[i], lights[i].getColor());
			loadFloat3(loc_attenuations[i], lights[i].getAttenuation());
		} else {
			glm::vec3 empty_light(0.0, 0.0, 0.0);
			loadFloat3(loc_lightPositions[i], empty_light);
			loadFloat3(loc_lightColors[i], empty_light);
			loadFloat3(loc_attenuations[i], empty_light);
		}
	}
}

void Shader::loadMaterialVars(Material material)
{
	loadFloat(loc_reflectivity, material.getReflectivity());
	loadFloat(loc_shineDamper, material.getShineDamper());
	loadFloat3(loc_baseColor, material.getBaseColor());
}

/* GLSL uniform loaders */
void Shader::loadInt(unsigned int loc, int value) const
{
	glUniform1i(loc, (int)value);
}

void Shader::loadBool(unsigned int loc, bool flag) const
{
	glUniform1i(loc, flag);
}

void Shader::loadFloat(unsigned int loc, float value) const
{
	glUniform1f(loc, value);
}

void Shader::loadFloat2(unsigned int loc, glm::vec2& vec) const
{
	glUniform2fv(loc, 1, &vec[0]);
}

void Shader::loadFloat3(unsigned int loc, glm::vec3 &vec) const
{
	glUniform3fv(loc, 1, &vec[0]);
}

void Shader::loadFloat4(unsigned int loc, glm::vec4& vec) const
{
	glUniform4fv(loc, 1, &vec[0]);
}

void Shader::loadMat2(unsigned int loc, glm::mat2& mat) const
{
	glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::loadMat3(unsigned int loc, glm::mat3& mat) const
{
	glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::loadMat4(unsigned int loc, glm::mat4& mat) const
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}