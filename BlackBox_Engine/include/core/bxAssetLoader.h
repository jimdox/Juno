#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <ios>
#include <sstream>
#include "render/Mesh.h"
#include "core/Log.h"
/* */
namespace bbx {

	struct ModelData
	{
		//float* vertices[] = nullptr;
		//float* textureCoords[] = nullptr;
		//float* normals[] = nullptr;
		unsigned int* indices = nullptr;
	};

	static void loadOBJ(const std::string& filepath)
	{
		std::vector<glm::vec3> vertexData;
		std::vector<glm::vec3> normalData;
		std::vector<glm::vec2> textureData;
		std::vector<unsigned int> indexData;
		ModelData data;


		std::string obj;
		std::ifstream sourceStream(filepath, std::ios::in);
		if (sourceStream.is_open())
		{
			std::stringstream src;
			src << sourceStream.rdbuf();
			obj = src.str();

			sourceStream.close();
		}
		else {
			BBX_CLI_ERR("Error: cannot access file: " + filepath);
		}
		//BBX_CLI_CRIT(obj);

		std::string temp;
		unsigned int i;
		while (obj.length() != 0)
		{
			i = obj.find_first_of('\n');
			temp = obj.substr(0, i);
			BBX_WARN(temp);
			obj = obj.substr(i, obj.length());
		}


	}

	Mesh* loadMesh(ModelData data)
	{

	}
}
