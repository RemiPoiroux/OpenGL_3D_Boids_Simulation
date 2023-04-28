#include "Vbos&Ibos.hpp"

std::vector<glimac::ShapeVertex> cubeVbo()
{
    // Coordonnées des sommets du cube
    const float vertexCoords[] = {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f};

    // Normales des sommets du cube
    const float vertexNormals[] = {
        -1.f, -1.f, -1.f,
        -1.f, -1.f, 1.f,
        -1.f, 1.f, -1.f,
        -1.f, 1.f, 1.f,
        1.f, -1.f, -1.f,
        1.f, -1.f, 1.f,
        1.f, 1.f, -1.f,
        1.f, 1.f, 1.f};

    // Coordonnées de texture des sommets du cube
    const float vertexTexCoords[] = {
        0.f, 0.f,
        0.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        0.f, 0.f,
        0.f, 1.f,
        1.f, 0.f,
        1.f, 1.f};

    std::vector<glimac::ShapeVertex> vertices;

    // Remplissage du vecteur de sommets
    vertices.reserve(8);
    for (int i = 0; i < 8; ++i)
    {
        vertices.push_back({glm::vec3{vertexCoords[3 * i], vertexCoords[3 * i + 1], vertexCoords[3 * i + 2]}, glm::vec3{vertexNormals[3 * i], vertexNormals[3 * i + 1], vertexNormals[3 * i + 2]}, glm::vec2{vertexTexCoords[2 * i], vertexTexCoords[2 * i + 1]}});
    }

    return vertices;
}

std::vector<uint32_t> cubeIbo()
{
    // Indices des sommets des triangles formant le cube
    std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 1, 3,
        2, 3, 6,
        6, 3, 7,
        5, 4, 7,
        7, 4, 6,
        1, 0, 5,
        5, 0, 4,
        4, 0, 2,
        2, 6, 4,
        1, 5, 3,
        3, 5, 7};
    return indices;
}
