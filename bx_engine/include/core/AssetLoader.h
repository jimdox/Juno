#pragma once
#include "pch.h"
#include <stb_image.h>

#include "entity/Entity.h"
#include "core/Log.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



namespace bxImport {
    /* references for garbage collection later */
    static std::vector<unsigned int> VAO_refs;
    static std::vector<unsigned int> VBO_refs;

    
    static unsigned int generateVAO()
    {
        unsigned int vaoID;
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);
        return vaoID;
    }


    static void storeInAttribList(unsigned int attribNum, unsigned int dimensions, std::vector<float> data)
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

   
    static std::tuple<unsigned int, unsigned int> loadToVAO(std::vector<float> &positions, std::vector<float> texCoordinates, std::vector<float> normals, std::vector<unsigned int> &indices)
    {
        unsigned int vaoID = generateVAO();

        unsigned int iboID;
        glGenBuffers(1, &iboID);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        unsigned int numIndices = indices.size();
        
        storeInAttribList(0, 3, positions);
        storeInAttribList(1, 3, normals);
        storeInAttribList(2, 2, texCoordinates);


        glBindVertexArray(0);
        return {vaoID, numIndices};
    }


    /* ------------------------------------ */

    static void processFace(int faceVertices[3][3], std::vector<unsigned int> &indices, std::vector<glm::vec2> &textures, 
                                std::vector<glm::vec3> &normals, std::vector<float> &texturesData, std::vector<float> &normalsData )
    {
        for(int i = 0; i < 3; i++)
        {
            // BX_CLI_WARN(faceVertices[i][0]);

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



    /* custom obj importer */
    static bx::Mesh loadOBJFile(const std::string& filepath)
    {
        std::string line;                            /* per line .obj info */
        std::ifstream openFile(filepath.c_str());
        if(!openFile.is_open())
        {
            BX_CLI_ERR(("Error loading file: " + filepath).c_str());
        }
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textures;
        std::vector<unsigned int> indices;
        
        std::vector<float> verticesData;
        std::vector<float> texturesData;
        std::vector<float> normalsData;



        std::vector<std::string> lineData;
        
        while(!openFile.eof())
        {
            std::getline(openFile, line);
            std::stringstream ss(line);
            std::string token;
            while(std::getline(ss, token, ' '))
            {
                lineData.push_back(token);                              /* split line by ' '  */
            }

            if(lineData[0] == "v")
            {
                glm::vec3 vert(atof(lineData[1].c_str()), atof(lineData[2].c_str()), atof(lineData[3].c_str()));
                vertices.push_back(vert);
            }
            else if (lineData[0] == "vt")
            {
                glm::vec2 texCoord(atof(lineData[1].c_str()), atof(lineData[2].c_str()));
                textures.push_back(texCoord);
            }
            else if(lineData[0] == "vn")
            {
                glm::vec3 norm(atof(lineData[1].c_str()), atof(lineData[2].c_str()), atof(lineData[3].c_str()));
                normals.push_back(norm);
            }
            if(lineData[0] == "f"){ break; }
            lineData.clear();
        }
        /* Next Phase of OBJ loading: faces */
        
        verticesData.reserve(vertices.size() * 3);
        normalsData.resize(vertices.size() * 3);
        texturesData.resize(vertices.size() * 2);

        lineData.clear();

        while(!openFile.eof())    
        {
            std::getline(openFile, line);
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
                        std::getline(ssVert, tokVert, '/');             /* split face vertex by '/'  */
                        faceVertices[i][j] = atoi(tokVert.c_str());
                    }

                                    

                }
                processFace(faceVertices, indices, textures, normals, texturesData, normalsData);

            }
            lineData.clear();
        }
        openFile.close();
        /* done parsing obj */

        for(glm::vec3 vertex : vertices)
        {
            verticesData.push_back(vertex.x);
            verticesData.push_back(vertex.y);
            verticesData.push_back(vertex.z);
        }

        bx::Mesh mesh(verticesData, texturesData, normalsData, indices);

        auto [id, numIndices] = loadToVAO(verticesData, texturesData, normalsData, indices);
        mesh.assignVAO(id, numIndices);
        return mesh;
    }

static bx::Mesh loadModel(const std::string& filepath)
{
    if(filepath.find(".obj") != std::string::npos)
    {
    return loadOBJFile(filepath);
    }
    return loadOBJFile(filepath);
}

static unsigned int loadTexture(const std::string& filepath, bx::TextureType tx_type)
{
    int width, height, channels;
    unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	unsigned char* imageData = stbi_load((filepath.c_str()), &width, &height, &channels, 0);

	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		BX_CLI_INFO(("Loaded Texture from location: " + filepath).c_str());
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if(tx_type == bx::TX_DIFFUSE || tx_type == bx::TX_SPECULAR)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
            if(glewIsSupported("GL_EXT_texture_filter_anisotropic") || GLEW_EXT_texture_filter_anisotropic)
            {
                
            } else {
                BX_WARN("anisotropic filtering is not available!");
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);
            }
        }
    }
	else
	{
		BX_CLI_INFO("Failed to load image: " + filepath);
	}
	stbi_image_free(imageData);
    return id;
}


static void loadShader(const std::string& filepath, GLuint &vertexShader, GLuint &fragmentShader)
{
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
		BX_ERR("Error: cannot access file: " + vertFilePath);
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
		BX_ERR("Error: cannot access file: " + fragFilePath);
		return;
	}

	GLint compileFlag = GL_FALSE;
	int errorLength;
	BX_WARN("Compiling shaders");

	char const* vertSrcPtr = vertSource.c_str();
	char const* fragSrcPtr = fragSource.c_str();

	glShaderSource(vertexShader, 1, &vertSrcPtr, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileFlag);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0)
	{

		std::vector<char> errorMsg(errorLength + 1);
		glGetShaderInfoLog(vertexShader, errorLength, NULL, &errorMsg[0]);
		std::string message(errorMsg.begin(), errorMsg.end());
		BX_CRIT(message);
	}

	glShaderSource(fragmentShader, 1, &fragSrcPtr, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileFlag);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &errorLength);

	if (errorLength > 0)
	{
		std::vector<char> errorMsg(errorLength + 1);
		glGetShaderInfoLog(fragmentShader, errorLength, NULL, &errorMsg[0]);
		std::string message(errorMsg.begin(), errorMsg.end());
		BX_CRIT(message);
	}
}

}


































// class Vertex
// {
// public:
//     Vertex(unsigned int index, glm::vec3 pos) : position(pos)
//     {
//         this->index = index;
//         textureIndex = -1;
//         normalIndex = -1;
//         duplicate = nullptr;
//     }

//     unsigned int getIndex()
//     {
//         return index;
//     }

//     int getLength()
//     {
//         return position.length();
//     }

//     bool inUse()
//     {
//         return textureIndex != -1 && normalIndex != -1;
//     }

//     bool containsSameVTVN(unsigned int texIndex, unsigned int normIndex)
//     {
//         return textureIndex == texIndex && normalIndex == normIndex;
//     }

//     void setTextureIndex(unsigned int i)
//     {
//         textureIndex = i;
//     }

//     void setNormalIndex(unsigned int i)
//     {
//         normalIndex = i;
//     }

//     glm::vec3& getPosition()
//     {
//         return position;
//     }

//     unsigned int getTextureIndex()
//     {
//         return textureIndex;
//     }

//     unsigned int getNormalIndex()
//     {
//         return normalIndex;
//     }

//     Vertex* getDuplicate()
//     {
//         return duplicate;
//     }

//     void setDuplicate(Vertex* dup)
//     {
//         duplicate = dup;
//     }

// private:
//     glm::vec3 position;
//     unsigned int index;
//     unsigned int textureIndex;
//     unsigned int normalIndex;
//     Vertex* duplicate;
// };

// /* */
// namespace bxImport {
//     /* references for garbage collection later */
//     static std::vector<unsigned int> VAO_refs;
//     static std::vector<unsigned int> VBO_refs;

    
//     static unsigned int generate_VAO()
//     {
//         unsigned int vaoID;
//         glGenVertexArrays(1, &vaoID);
//         glBindVertexArray(vaoID);
//         return vaoID;
//     }


//     static void storeInAttribList(unsigned int attribNum, unsigned int dimensions, std::vector<float> data)
//     {
//         unsigned int VBO_ID;
//         glGenBuffers(1, &VBO_ID);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
// 	    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
// 	    glVertexAttribPointer(attribNum, dimensions, GL_FLOAT, GL_FALSE, 0, (void*)0);           /* check correct stride value for float array */
//         glBindBuffer(GL_ARRAY_BUFFER, 0);

//     }


//     // static void bindIBO(std::vector<unsigned int> &indices)
//     // {
//     //     unsigned int iboID;
//     //     glGenBuffers(1, &iboID);
// 	//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
//     //     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

//     // }

   
//     static std::tuple<unsigned int, unsigned int> load_to_VAO(std::vector<float> &positions, std::vector<float> tx_coordinates, std::vector<float> normals, std::vector<unsigned int> &indices)
//     {
//         unsigned int vaoID = generate_VAO();

//         unsigned int iboID;
//         glGenBuffers(1, &iboID);
// 	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

//         unsigned int numIndices = indices.size();
        
//         storeInAttribList(0, 3, positions);
//         storeInAttribList(1, 3, normals);
//         storeInAttribList(2, 2, tx_coordinates);


//         glBindVertexArray(0);
//         return {vaoID, numIndices};
//     }

//     static void removeNonSetVertices(std::vector<Vertex> &vertices)
//     {
//         for(int i = 0; i < vertices.size(); i++)
//         {
//             if(!vertices[i].inUse())
//             {
//                 vertices[i].setNormalIndex(0);
//                 vertices[i].setTextureIndex(0);
//             }
//         }
//     }


//     static void handleProcessedVertex(Vertex &prev_vert, unsigned int tex_index_next, unsigned int norm_index_next, std::vector<unsigned int> &indices, std::vector<Vertex> &vertices)
//     {
//         if(prev_vert.getTextureIndex() == tex_index_next && prev_vert.getNormalIndex() == norm_index_next)
//         {
//             indices.push_back(prev_vert.getIndex());
//         } else {
//             if(prev_vert.getDuplicate() != nullptr)  
//             {
//                 handleProcessedVertex(*prev_vert.getDuplicate(), tex_index_next, norm_index_next, indices, vertices);
//             } else {
//                 Vertex dup(vertices.size(), prev_vert.getPosition());
//                 dup.setTextureIndex(tex_index_next);
//                 dup.setNormalIndex(norm_index_next);
//                 prev_vert.setDuplicate(&dup);
//                 vertices.push_back(dup);
//                 indices.push_back(dup.getIndex());
//             }

//         }

//     }   

//     /* ------------------------------------ */

//     static void processFace(int faceVertices[3][3], std::vector<Vertex> &vertices, std::vector<unsigned int> &indices)
//     {
//         for(int i = 0; i < 3; i++)
//         {
//             // BX_CLI_WARN(faceVertices[i][0]);
//             unsigned int index = faceVertices[i][0] - 1;
//             Vertex vertex_current = vertices[index];
//             unsigned int tex_index = faceVertices[i][1] - 1;
//             unsigned int norm_index = faceVertices[i][2] - 1;
//             if(!vertex_current.inUse())
//             {
//                 indices.push_back(index);
//                 vertex_current.setTextureIndex(tex_index);
//                 vertex_current.setNormalIndex(norm_index);
//             } else {
//                 handleProcessedVertex(vertex_current, tex_index, norm_index, indices, vertices);
//             }
//         }
//     }

//     static float convertToObjectData(std::vector<Vertex> &vertices, std::vector<glm::vec2> &tex_coords, std::vector<glm::vec3> &normals, 
//                                                                     std::vector<float> &vertices_data, std::vector<float> &textures_data, std::vector<float> &normals_data)
//     {
//         float furthest = 0;
//         for(int i = 0; i < vertices.size(); i++)
//         {
//             Vertex vertex_current = vertices[i];
//             if(vertex_current.getLength() > furthest)
//             {
//                 furthest = vertex_current.getLength();
//             }
//             glm::vec3 position = vertex_current.getPosition();
//             glm::vec3 normal = normals[vertex_current.getNormalIndex()];
//             glm::vec2 tx_coord = tex_coords[vertex_current.getTextureIndex()];
            
//             vertices_data[i*3] = position.x;
//             vertices_data[i*3 +1] = position.y;
//             vertices_data[i*3 +2] = position.z;
//             textures_data[i*2] = tx_coord.x;
//             textures_data[i*2 +1] = tx_coord.y; /* 1 - y ? */
//             normals_data[i*3] = normal.x;
//             normals_data[i*3 +1] = normal.y;
//             normals_data[i*3 +2] = normal.z;
//         }
//         return furthest;
//     }

//     /* custom obj importer */
//     static bx::Mesh loadOBJFileFile(std::string& filepath)
//     {
//         std::string line;                            /* per line .obj info */
//         std::ifstream open_file(filepath.c_str());
//         if(!open_file.is_open())
//         {
//             BX_CLI_ERR(("Error loading file: " + filepath).c_str());
//         }
//         std::vector<Vertex> vertices;
//         std::vector<glm::vec3> normals;
//         std::vector<glm::vec2> textures;
//         std::vector<unsigned int> indices;
        
//         std::vector<float> vertices_data;
//         std::vector<float> textures_data;
//         std::vector<float> normals_data;

//         std::vector<std::string> line_data;
        
//         while(!open_file.eof())
//         {
//             std::getline(open_file, line);
//             std::stringstream ss(line);
//             std::string token;
//             while(std::getline(ss, token, ' '))
//             {
//                 line_data.push_back(token);                              /* split line by ' '  */
//             }

//             if(line_data[0] == "v")
//             {
//                 glm::vec3 vert(atof(line_data[1].c_str()), atof(line_data[2].c_str()), atof(line_data[3].c_str()));
//                 vertices.push_back(Vertex(vertices.size(), vert));
//             }
//             else if (line_data[0] == "vt")
//             {
//                 glm::vec2 texCoord(atof(line_data[1].c_str()), atof(line_data[2].c_str()));
//                 textures.push_back(texCoord);
//             }
//             else if(line_data[0] == "vn")
//             {
//                 glm::vec3 norm(atof(line_data[1].c_str()), atof(line_data[2].c_str()), atof(line_data[3].c_str()));
//                 normals.push_back(norm);
//             }
//             if(line_data[0] == "f"){ break; }
//             line_data.clear();
//         }
//         /* Next Phase of OBJ loading: faces */
//         vertices_data.resize(vertices.size() * 3);
//         normals_data.resize(vertices.size() * 3);
//         textures_data.resize(vertices.size() * 2);

//         line_data.clear();

//         while(!open_file.eof())    
//         {
//             std::getline(open_file, line);
//             std::stringstream ss(line);
//             std::string token;
//             while(std::getline(ss, token, ' '))
//             {
//                 line_data.push_back(token);                              /* split line by ' '  */
//             }

//             if(line_data[0] == "f")
//             {
            
//                 int faceVertices[3][3];

//                 for(unsigned int i = 0; i < 3; i++)
//                 {
//                     std::stringstream ssVert(line_data[i+1]);
//                     std::string tokVert;

//                     for(int j = 0; j < 3; j++)
//                     {
//                         std::getline(ssVert, tokVert, '/');             /* split face vertex by '/'  */
//                         faceVertices[i][j] = atoi(tokVert.c_str());
//                     }

                                    

//                 }
//                 processFace(faceVertices, vertices, indices);
//             }
//             line_data.clear();
//         }
//         open_file.close();

//         removeNonSetVertices(vertices);
//         float ptr_furthest = convertToObjectData(vertices, textures, normals, vertices_data, textures_data, normals_data);

//         /* done parsing obj */

//         // for(glm::vec3 vertex : vertices)
//         // {
//         //     vertices_data.push_back(vertex.x);
//         //     vertices_data.push_back(vertex.y);
//         //     vertices_data.push_back(vertex.z);
//         // }

//         bx::Mesh mesh(vertices_data, textures_data, normals_data, indices, ptr_furthest);

//         auto [id, numIndices] = load_to_VAO(vertices_data, textures_data, normals_data, indices);
//         mesh.assignVAO(id, numIndices);
//         return mesh;
//     }
    
//     static bx::Mesh loadModel(std::string& filepath)
//     {
//         if(filepath.find(".obj") != std::string::npos)
//         {
//         return loadOBJFileFile(filepath);
//         }
//         return loadOBJFileFile(filepath);

//     }
// }