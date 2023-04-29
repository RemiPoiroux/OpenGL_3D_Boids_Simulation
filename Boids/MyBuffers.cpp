#include "MyBuffers.hpp"
#include "ObjLoad.hpp"
#include "img/src/Image.h"

std::vector<glimac::ShapeVertex> cubeVbo()
{
    std::string cubeFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/background.obj");
    return loadVboFromObj(cubeFile);
}
std::vector<uint32_t> cubeIbo()
{
    std::string cubeFile("/home/rempoir/Documents/IMAC/Annee2/Semestre 2/Synthèse d'image/OpenGL_3D_Boids_Simulation/assets/models/background.obj");
    return loadIboFromObj(cubeFile);
}

void initializesBuffers(MyBuffers& vbos, MyBuffers& ibos, MyBuffers& vaos, MyBuffers& textures)
{
    auto initializeVbo = [](const auto& function, GLuint& vbo) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        std::vector<glimac::ShapeVertex> vertices = function();
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    };
    auto initializeIbo = [](const auto& function, GLuint& ibo) {
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        std::vector<uint32_t> indices = function();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    };
    auto initializeVao = [&](GLuint& vao, const GLuint& vbo, const GLuint& ibo) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        static constexpr GLuint VERTEX_ATTR_POSITION = 0;
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        static constexpr GLuint VERTEX_ATTR_NORMAL = 1;
        glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
        static constexpr GLuint VERTEX_ATTR_TEXTURE = 2;
        glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), nullptr);
        glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
        glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    };
    auto initializeTexture = [](const img::Image& image, GLuint& texture) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    };

    initializeVbo(cubeVbo, vbos.m_["background"]);
    initializeIbo(cubeIbo, ibos.m_["background"]);
    initializeVao(vaos.m_["background"], vbos.m_["background"], ibos.m_["background"]);

    img::Image backgroundImage = p6::load_image_buffer("assets/textures/starry-night-sky.jpg");

    initializeTexture(backgroundImage, textures.m_["background"]);
}

void releasesRessources(MyBuffers& vbos, MyBuffers& ibos, MyBuffers& vaos, MyBuffers& textures)
{
    auto ReleaseBuffers = [](MyBuffers& buffers) {
        for (auto& [name, buffer] : buffers.m_)
        {
            glDeleteBuffers(1, &buffer);
        }
        buffers.m_.clear();
    };

    ReleaseBuffers(vbos);
    ReleaseBuffers(ibos);
    ReleaseBuffers(vaos);
    ReleaseBuffers(textures);
}