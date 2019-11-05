#include "render/shaders/SkyBoxShader.h"
#include "core/AssetLoader.h"

using namespace bx;

SkyBoxShader::SkyBoxShader()
{
    loadShader(DEFAULT_PATH);
}

SkyBoxShader::SkyBoxShader(const std::string& filepath)
{
    loadShader(filepath);
}

SkyBoxShader::~SkyBoxShader()
{

}

bool SkyBoxShader::loadShader(const std::string& filepath)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	bxImport::loadShader(filepath, vertexShader, fragmentShader);

	BX_INFO("Linking shaders");
	this->progID = glCreateProgram();

	glAttachShader(progID, vertexShader);
	glAttachShader(progID, fragmentShader);
	bindAllAttribs();
	glLinkProgram(progID);

	int errorLength;
	GLint linkStatus;
	glGetProgramiv(progID, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0)
	{
		std::vector<char> errorMsg(errorLength + 1);
		glGetShaderInfoLog(fragmentShader, errorLength, NULL, &errorMsg[0]);
		std::string message(errorMsg.begin(), errorMsg.end());
		BX_CRIT("Shader linking failed!");
		BX_CRIT(message);
	}
	cacheUniformLocations();

	return true;
}

void SkyBoxShader::bindAllAttribs()
{
    bindAttrib(0, "position");
}

void SkyBoxShader::cacheUniformLocations()
{
	loc_projectionMatrix = glGetUniformLocation(progID, "projection");
	loc_viewMatrix = glGetUniformLocation(progID, "view");
}
