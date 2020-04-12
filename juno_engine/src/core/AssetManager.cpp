#include "core/AssetManager.h"
#include "core/Log.h"
using namespace juno;

/* */

unsigned int AssetManager::genAssetID()
{
    return ++currentIDPtr;                                           /* id = 0 is reserved for an ID search failure */
}

unsigned int AssetManager::findAssetID(std::map<const std::string, unsigned int>& fileMap, const std::string& filepath)
{
    std::map<const std::string, unsigned int>::iterator iter;
    unsigned int assetID;

    iter = fileMap.find(filepath);
    if(iter != fileMap.end())                                       /* asset has already been loaded */
    {
        return iter->second;
    }
    return 0;                                                       /* asset has not been loaded */
}

Mesh& AssetManager::loadMesh(const std::string& filepath)
{
    unsigned int assetID = findAssetID(meshFilepaths, filepath);

    if(assetID != 0)
    {
        meshRefs[assetID];
    }
    assetID = genAssetID();
    meshRefs.insert(std::pair<unsigned int, std::unique_ptr<Mesh>>(assetID, std::make_unique<Mesh>(filepath)));
    return *meshRefs[assetID].get();
}

Shader& AssetManager::loadShader(const std::string& filepath)
{
    unsigned int assetID = findAssetID(shaderFilepaths, filepath);

    if(assetID != 0)
    {
        return *shaderRefs[assetID].get();
    }
    assetID = genAssetID();
    shaderRefs.insert(std::pair<unsigned int, std::unique_ptr<Shader>>(assetID, std::make_unique<Shader>(filepath)));
    return *shaderRefs[assetID].get();
}

Shader& AssetManager::getDefaultShader()
{
    return loadShader("./juno_engine/res/shaders/basic");
}

Texture& AssetManager::loadTexture(const std::string& filepath, TextureType texType)
{
    unsigned int assetID = findAssetID(textureFilepaths, filepath);

    if(assetID != 0)
    {
        return *textureRefs[assetID].get();
    }
    assetID = genAssetID();
    textureRefs.insert(std::pair<unsigned int, std::unique_ptr<Texture>>(assetID, std::make_unique<Texture>(filepath, texType)));
    return *textureRefs[assetID].get();
}

CubeMap& AssetManager::loadCubeMap(const std::string& filepath, juno::TextureType texType)
{
   unsigned int assetID = findAssetID(cubeMapFilepaths, filepath);
    if(assetID != 0)
    {
        return *cubeMapRefs[assetID].get();
    }

    std::array<std::string, 6> mapFaceFilepaths;
    mapFaceFilepaths[0] = filepath + "/right.png";
    mapFaceFilepaths[1] = filepath + "/left.png";
    mapFaceFilepaths[2] = filepath + "/top.png";
    mapFaceFilepaths[3] = filepath + "/bottom.png";
    mapFaceFilepaths[4] = filepath + "/front.png";
    mapFaceFilepaths[5] = filepath + "/back.png";
    
    assetID = genAssetID();
    cubeMapRefs.insert(std::pair<unsigned int, std::unique_ptr<CubeMap>>(assetID, std::make_unique<CubeMap>(mapFaceFilepaths, texType)));
    return *cubeMapRefs[assetID];
}

unsigned int AssetManager::generateVAO()
{
    unsigned int vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    return vaoID;
}

void AssetManager::storeDataInAttribList(unsigned int attribNum, unsigned int dimensions, std::vector<float> data)
{
    unsigned int VBO_ID;
    glGenBuffers(1, &VBO_ID);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(attribNum, dimensions, GL_FLOAT, GL_FALSE, 0, (void*)0);           /* check correct stride value for float array */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// static void bindIBO(std::vector<unsigned int> &indices)
// {
//     unsigned int iboID;
//     glGenBuffers(1, &iboID);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

// }

/* loads a single vertex buffer object to VAO */
std::pair<unsigned int, unsigned int> AssetManager::loadToVAO(std::vector<float>& positions, unsigned int dim)
{
    unsigned int vaoID = generateVAO();
    storeDataInAttribList(0, dim, positions);
    glBindVertexArray(0);
    return {vaoID, positions.size()/dim};
}

/* used for .obj file loading */
std::pair<unsigned int, unsigned int> AssetManager::loadToVAO(std::vector<float>& positions, std::vector<float>& texCoordinates,
                                                               std::vector<float>& normals, std::vector<unsigned int>& indices)
{
    unsigned int vaoID = generateVAO();

    unsigned int iboID;
    glGenBuffers(1, &iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    unsigned int numIndices = indices.size();
    
    storeDataInAttribList(0, 3, positions);
    storeDataInAttribList(1, 3, normals);
    storeDataInAttribList(2, 2, texCoordinates);

    glBindVertexArray(0);
    return {vaoID, numIndices};
}   


std::tuple<GLuint, GLuint> AssetManager::loadShaderFiles(const std::string& filepath)
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER); 
    GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertFilePath = filepath + ".vert";
	std::string fragFilePath = filepath + ".frag";
    std::string vertSource;
	std::string fragSource;

	std::ifstream sourceStream(vertFilePath, std::ios::in);
	if (sourceStream.is_open())
	{
		std::stringstream src;
		src << sourceStream.rdbuf();
		vertSource = src.str();
		sourceStream.close();
	}
	else {
		JN_CLI_ERR("Error: cannot access file: " + vertFilePath);
	}

	std::ifstream sourceStream2(fragFilePath, std::ios::in);
	if (sourceStream2.is_open())
	{

		std::stringstream src;
		src << sourceStream2.rdbuf();
		fragSource = src.str();
		sourceStream2.close();
	}
	else {
		JN_CLI_ERR("Error: cannot access file: " + fragFilePath);
		return{0,0};
	}

	GLint compileFlag = GL_FALSE;
	int errorLength;
	JN_WARN("Compiling shaders");

	char const* vertSrcPtr = vertSource.c_str();
	char const* fragSrcPtr = fragSource.c_str();

	glShaderSource(vertexShaderID, 1, &vertSrcPtr, NULL);
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compileFlag);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0)
	{
		std::vector<char> errorMsg(errorLength + 1);
		glGetShaderInfoLog(vertexShaderID, errorLength, NULL, &errorMsg[0]);
		std::string message(errorMsg.begin(), errorMsg.end());
		JN_CRIT(message);
	}

	glShaderSource(fragShaderID, 1, &fragSrcPtr, NULL);
	glCompileShader(fragShaderID);
	glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &compileFlag);
	glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0)
	{
		std::vector<char> errorMsg(errorLength + 1);
		glGetShaderInfoLog(fragShaderID, errorLength, NULL, &errorMsg[0]);
		std::string message(errorMsg.begin(), errorMsg.end());
		JN_CRIT(message);
	}
    return {vertexShaderID, fragShaderID};
}

unsigned int AssetManager::loadTextureFile(const std::string& filepath, GLenum format, juno::TextureType texType)
{
    int width, height, channels;
    unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(format, id);

	unsigned char* imageData = stbi_load((filepath.c_str()), &width, &height, &channels, 0);

	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		JN_CLI_INFO(("Loaded Texture from location: " + filepath).c_str());
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if(texType == juno::TX_DIFFUSE || texType == juno::TX_SPECULAR)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
            if(glewIsSupported("GL_EXT_texture_filter_anisotropic") || GLEW_EXT_texture_filter_anisotropic)
            {
                
            } else {
                JN_WARN("anisotropic filtering is not available!");
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);
            }
        }
    }
	else
	{
		JN_CLI_ERR("Failed to load image: " + filepath);
	    return loadTextureFile("./juno_engine/res/red.png", format, texType);
	}
	stbi_image_free(imageData);
    return id;
}

unsigned int AssetManager::loadCubeMapFiles(const std::array<std::string, 6>& filepaths, juno::TextureType tx_type)
{
    int width, height, channels;
    unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    unsigned char* imageData;
    for(unsigned int i = 0; i < 6; i++)
    {
        imageData = stbi_load((filepaths[i].c_str()), &width, &height, &channels, 0);

        if (imageData)
        {   
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);       /* GL_RGB or RGBA? */
        }
        else
        {
            JN_CLI_INFO("Failed to load image: " + filepaths[i]);
            stbi_image_free(imageData);
            return 0;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

    stbi_image_free(imageData);
    return id;
}


/* 
    .obj wavefront importer : treats each face as having one normal (in blender: smooth surface), 
    where the mesh requires double vertices along any given texture coordinate seam for correct indices 
*/

void AssetManager::processFace(int faceVertices[3][3], std::vector<unsigned int> &indices, std::vector<glm::vec2> &textures, 
                               std::vector<glm::vec3> &normals, std::vector<float> &texturesData, std::vector<float> &normalsData )
{
    for(int i = 0; i < 3; i++)
    {
        // JN_CLI_WARN(faceVertices[i][0]);

        int currentVertPtr = faceVertices[i][0] - 1;
        indices.push_back(currentVertPtr);
        
        glm::vec2 currentTexCoord = textures[faceVertices[i][1]-1];
        texturesData[currentVertPtr*2] = currentTexCoord.x;
        texturesData[currentVertPtr*2 +1] = currentTexCoord.y;



        glm::vec3 currentNormal = normals[faceVertices[i][2]-1];
        normalsData[currentVertPtr * 3] = currentNormal.x;
        normalsData[currentVertPtr * 3 +1] = currentNormal.y;
        normalsData[currentVertPtr * 3 +2] = currentNormal.z;
    }
}

std::tuple<unsigned int, unsigned int, unsigned int> AssetManager::loadOBJFile(const std::string& filepath)
{
    std::string line;                            /* per line .obj info */
    std::ifstream openFile(filepath.c_str());
    if(!openFile.is_open())
    {
        JN_CLI_ERR(("Error loading file: " + filepath).c_str());
        return {0,0,0};
    }
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;
    std::vector<unsigned int> indices;
    
    std::vector<float> verticesData;
    std::vector<float> texturesData;
    std::vector<float> normalsData;
    
    vertices.reserve(1000);
    normals.reserve(1000);
    textures.reserve(1000);
    indices.reserve(300);

    std::vector<std::string> lineData;
    
    while(!openFile.eof())
    {
        std::getline(openFile, line);
        std::stringstream ss(line);
        std::string token;
        while(std::getline(ss, token, ' '))
        {
            lineData.emplace_back(token);                              
        }
        
        if(lineData[0] == "v")
        {
            glm::vec3 vert(atof(lineData[1].c_str()), atof(lineData[2].c_str()), atof(lineData[3].c_str()));
            vertices.emplace_back(vert);
        }
        else if (lineData[0] == "vt")
        {
            glm::vec2 texCoord(atof(lineData[1].c_str()), atof(lineData[2].c_str()));
            textures.emplace_back(texCoord);
        }
        else if(lineData[0] == "vn")                
        {
            glm::vec3 norm(atof(lineData[1].c_str()), atof(lineData[2].c_str()), atof(lineData[3].c_str()));
            normals.emplace_back(norm);
        }
        else if(lineData[0] == "f")
        {
            /* current lineData needs to be read in the next step */
            break;
        }
        lineData.clear();
    }
    /* Next Phase -> faces */
    
    verticesData.reserve(vertices.size() * 3);
    normalsData.resize(vertices.size() * 3);
    texturesData.resize(vertices.size() * 2);

    lineData.clear();

    while(!openFile.eof())    
    {
        std::stringstream ss(line);
        std::string token;
        while(std::getline(ss, token, ' '))
        {
            lineData.push_back(token);                              /* split line by ' '  */
        }

        if(lineData[0] == "f")
        {
            int faceVertices[3][3];

            for(unsigned int i = 0; i < 3; i++)
            {
                std::stringstream ssVert(lineData[i+1]);
                std::string tokVert;

                for(int j = 0; j < 3; j++)
                {
                    std::getline(ssVert, tokVert, '/');             /* split face vertices by '/'  */
                    faceVertices[i][j] = atoi(tokVert.c_str());
                }
            }

            processFace(faceVertices, indices, textures, normals, texturesData, normalsData);
        }
        lineData.clear();
        std::getline(openFile, line);
    }
    openFile.close();
    /* done parsing .obj */

    for(glm::vec3 vertex : vertices)
    {
        verticesData.push_back(vertex.x);
        verticesData.push_back(vertex.y);
        verticesData.push_back(vertex.z);
    }

    auto [vaoID, numIndices] = loadToVAO(verticesData, texturesData, normalsData, indices);
    return { vaoID, numIndices, vertices.size() };
}