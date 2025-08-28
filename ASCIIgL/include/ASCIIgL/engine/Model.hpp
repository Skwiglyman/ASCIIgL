#pragma once

#include "Mesh.hpp"

#include <../../libs/assimp/include/assimp/Importer.hpp>
#include <../../libs/assimp/include/assimp/scene.h>
#include <../../libs/assimp/include/assimp/postprocess.h>

#include "Texture.hpp"
#include "../renderer/Vertex.hpp"

class Model
{
public:
    Model(std::string path) { loadModel(path); }
    Model(std::vector<VERTEX> vertices, std::vector<Texture*> textures) { meshes.push_back(new Mesh(vertices, textures)); };
    ~Model();

    std::vector<Mesh*> meshes;

private:
    // model data
    std::string directory;
    std::vector<Texture*> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};