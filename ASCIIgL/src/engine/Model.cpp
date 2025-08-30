#include <ASCIIgL/engine/Model.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>

// PIMPL Implementation class that contains all Assimp-related code
class Model::Impl
{
public:
    std::string directory;
    std::vector<Texture*> textures_loaded;

    void loadModel(std::string path, std::vector<Mesh*>& meshes);
    void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

void Model::Impl::loadModel(std::string path, std::vector<Mesh*>& meshes)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    
    processNode(scene->mRootNode, scene, meshes);
}

void Model::Impl::processNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, meshes);
    }
}

Mesh* Model::Impl::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<VERTEX> vertices;
    std::vector<Texture*> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        VERTEX vertex;
        vertex.SetXYZW(glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f));

        // normals
        if (mesh->HasNormals())
        {
            vertex.SetNXYZ(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vertex.SetUV(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
        }

        else
        {
            vertex.SetUV(glm::vec2(0.0f, 0.0f));
        }

        vertices.push_back(vertex);
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // return a mesh object created from the extracted mesh data

    Mesh* returnMesh = new Mesh(vertices, textures);
    return returnMesh;
}

std::vector<Texture*> Model::Impl::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture*> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j]->GetFilePath().data(), str.C_Str()) == 0) // comparing textures to not repeat them
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture* texture = new Texture(str.C_Str(), typeName);
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }

    return textures;
}

// Model public interface implementation
Model::Model(std::string path) : pImpl(std::make_unique<Impl>())
{
    pImpl->loadModel(path, meshes);
}

Model::Model(std::vector<VERTEX> vertices, std::vector<Texture*> textures) : pImpl(std::make_unique<Impl>())
{
    meshes.push_back(new Mesh(vertices, textures));
}

Model::~Model() = default;

Model::Model(Model&& other) noexcept : meshes(std::move(other.meshes)), pImpl(std::move(other.pImpl))
{
}

Model& Model::operator=(Model&& other) noexcept
{
    if (this != &other)
    {
        meshes = std::move(other.meshes);
        pImpl = std::move(other.pImpl);
    }
    return *this;
}