#include "ObjLoad.hpp"
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <iostream>

std::vector<glimac::ShapeVertex> loadVboFromObj(const std::string& objFilePath)
{
    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(objFilePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        // Failed to load model file
        std::cerr << "Error: " << importer.GetErrorString() << std::endl;
        return {};
    }

    std::vector<glimac::ShapeVertex> vertices;

    // Iterate over all meshes in the scene
    for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
    {
        const aiMesh* mesh = scene->mMeshes[meshIndex];

        if (!mesh)
        {
            // Invalid mesh pointer
            continue;
        }

        // Process vertex positions, normals and texture coordinates
        for (unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex)
        {
            glimac::ShapeVertex vertex{};

            // Convert position
            const aiVector3D& pos = mesh->mVertices[vertexIndex];
            vertex.position       = glm::vec3(pos.x, pos.y, pos.z);

            // Convert normal
            const aiVector3D& normal = mesh->mNormals[vertexIndex];
            vertex.normal            = glm::vec3(normal.x, normal.y, normal.z);

            // Convert texture coordinates
            if (mesh->HasTextureCoords(0))
            {
                const aiVector3D& texCoord = mesh->mTextureCoords[0][vertexIndex];
                vertex.texCoords           = glm::vec2(texCoord.x, texCoord.y);
            }
            else
            {
                vertex.texCoords = glm::vec2(0.f);
            }

            vertices.push_back(vertex);
        }
    }

    return vertices;
}

std::vector<unsigned int> loadIboFromObj(const std::string& filepath)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        // Failed to load model file
        std::cerr << "Error: " << importer.GetErrorString() << std::endl;
        return {};
    }

    std::vector<unsigned int> indices;
    for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
    {
        const aiMesh* mesh = scene->mMeshes[meshIndex];

        // Process face indices and add them to the IBO
        for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
        {
            const aiFace& face = mesh->mFaces[faceIndex];
            for (unsigned int i = 0; i < face.mNumIndices; ++i)
            {
                indices.push_back(face.mIndices[i]);
            }
        }
    }

    return indices;
}