#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "entity/Entity.h"
#include "core/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


/* */
namespace bxImport {

    static void loadModel(std::string& filepath, std::vector<bbx::Mesh>& meshList)
    {
        Assimp::Importer aimport;
        const aiScene *scene = aimport.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
        {
            BBX_CLI_ERR(("Error Loading Model: " + filepath).c_str());
            return;
        }
        std::string directory = filepath.substr(0, filepath.find_last_of("/"));
        assimp_processNode(scene->mRootNode, scene);
    }

    static void assimp_processNode(aiNode *node, const aiScene *scene)
    {
        for(unsigned int i = 0; i <node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }   
        /* repeat for descendent meshes */
        for(unsigned int j = 0; j < node->mNumChildren; j++)
        {
            assimp_processNode(node->mChildren[j], scene);
        }
    }

    bbx::Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::vector<bbx::Vertex> vertices;
        std::vector<unsigned int> indices;
        bbx::TextureList textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            bbx::Vertex v; 

            vertices.push_back(v);
        }

        if(mesh->mMaterialIndex >= 0)
        {

        }
        return bbx::Mesh(vertices, indices, textures);
    }

    bbx::TextureList& loadTextures(aiMaterial *mat, aiTextureType type, std::string typeStr)

   
	
}
