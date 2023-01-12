#pragma once

#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(std::string path) { loadModel(path); }
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