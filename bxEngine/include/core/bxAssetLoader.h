#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <string>
#include <map>
#include <vector>
#include <istream>
#include <fstream>
#include <sstream>
#include <iostream>
#include "entity/Entity.h"
#include "core/Log.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdlib.h>


/* */
namespace bxImport {
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
	    glVertexAttribPointer(attribNum, dimensions, GL_FLOAT, GL_FALSE, 0, (void*)0);           /* may need to be (float) stride */
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }


    static void bindIBO(std::vector<unsigned int> &indices)
    {
        unsigned int iboID;
        glGenBuffers(1, &iboID);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    }

   
    static bbx::VAO_Data loadToVAO(std::vector<float> &positions, std::vector<float> texCoordinates, std::vector<unsigned int> &indices)
    {
        bbx::VAO_Data vaoData;
        vaoData.VAO_ID = generateVAO();
        bindIBO(indices);
        vaoData.numIndices = indices.size();
        
        storeInAttribList(0, 3, positions);
        storeInAttribList(1, 2, texCoordinates);


        glBindVertexArray(0);
        return vaoData;
    }





    

    


    /* ------------------------------------ */


    static void processFace(int faceVertices[3][3], std::vector<unsigned int> &indices, std::vector<glm::vec2> &textures, 
                                std::vector<glm::vec3> &normals, std::vector<float> &texturesData, std::vector<float> &normalsData )
    {
        for(int i = 0; i < 3; i++)
        {
            int currentVertPtr = faceVertices[i][0] - 1;
            indices.push_back(currentVertPtr);

            glm::vec2 currentTexCoord = textures[faceVertices[i][1]-1];
            texturesData[currentVertPtr*2] = currentTexCoord.x;
            texturesData[currentVertPtr*2 +1] = 1 - currentTexCoord.y;



            glm::vec3 currentNormal = normals[faceVertices[i][2]-1];
            normalsData[currentVertPtr * 3] = currentNormal.x;
            normalsData[currentVertPtr * 3 +1] = currentNormal.y;
            normalsData[currentVertPtr * 3 +2] = currentNormal.z;
        }
    }



    /* custom obj importer */
    static bbx::Mesh loadOBJ(std::string& filepath)
    {
        std::string line;                            /* per line .obj info */
        std::ifstream openFile(filepath.c_str());
        if(!openFile.is_open())
        {
            BBX_CLI_ERR(("Error loading file: " + filepath).c_str());
        }
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textures;
        std::vector<unsigned int> indices;
        
        std::vector<float> verticesData;
        std::vector<float> texturesData;
        std::vector<float> normalsData;

        std::vector<bbx::Vertex> meshData;


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
        /* load face data */
        verticesData.reserve(vertices.size() * 3);
        normalsData.reserve(vertices.size() * 3);
        texturesData.reserve(vertices.size() * 2);

        for(unsigned int i = 0; i < vertices.size() * 2; i++)
        {
            texturesData.push_back(0.0f);
        }

        for(unsigned int i = 0; i < vertices.size() * 3; i++)
        {
            normalsData.push_back(0.0f);
        }

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

       




        return bbx::Mesh(verticesData, texturesData, normalsData, indices);
    }

    
   
	
}
