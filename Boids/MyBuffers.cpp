#include "MyBuffers.hpp"
#include "Vbos&Ibos.hpp"

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

    // Background Buffers Init
    initializeVbo(cubeVbo, vbos.m_["background"]);
    initializeIbo(cubeIbo, ibos.m_["background"]);
    initializeVao(vaos.m_["background"], vbos.m_["background"], ibos.m_["background"]);
    img::Image backgroundImage = p6::load_image_buffer("assets/textures/Solarsystemscope_texture_8k_stars.jpg");
    initializeTexture(backgroundImage, textures.m_["background"]);

    // Character's Buffers Init
    initializeVbo(xwingVbo, vbos.m_["character"]);
    initializeIbo(xwingIbo, ibos.m_["character"]);
    initializeVao(vaos.m_["character"], vbos.m_["character"], ibos.m_["character"]);
    img::Image characterImage = p6::load_image_buffer("assets/textures/xwingTexture.png");
    initializeTexture(characterImage, textures.m_["character"]);

    initializeVbo(xwingReactorsVbo, vbos.m_["characterReactors"]);
    initializeIbo(xwingReactorsIbo, ibos.m_["characterReactors"]);
    initializeVao(vaos.m_["characterReactors"], vbos.m_["characterReactors"], ibos.m_["characterReactors"]);

    // Boid's buffers Init
    initializeVbo(tieHDVbo, vbos.m_["boidHD"]);
    initializeIbo(tieHDIbo, ibos.m_["boidHD"]);
    initializeVao(vaos.m_["boidHD"], vbos.m_["boidHD"], ibos.m_["boidHD"]);
    img::Image boidImage = p6::load_image_buffer("assets/textures/TF_diffuse.png");
    initializeTexture(boidImage, textures.m_["boidHD"]);

    initializeVbo(tieLDVbo, vbos.m_["boidLD"]);
    initializeIbo(tieLDIbo, ibos.m_["boidLD"]);
    initializeVao(vaos.m_["boidLD"], vbos.m_["boidLD"], ibos.m_["boidLD"]);
    initializeTexture(boidImage, textures.m_["boidLD"]);

    // Obstacle's buffers Init
    initializeVbo(asteroidHDVbo, vbos.m_["obstacleHD"]);
    initializeIbo(asteroidHDIbo, ibos.m_["obstacleHD"]);
    initializeVao(vaos.m_["obstacleHD"], vbos.m_["obstacleHD"], ibos.m_["obstacleHD"]);
    img::Image obstacleImage = p6::load_image_buffer("assets/textures/asteroidTexture.jpg");
    initializeTexture(obstacleImage, textures.m_["obstacleHD"]);

    initializeVbo(asteroidLDVbo, vbos.m_["obstacleLD"]);
    initializeIbo(asteroidLDIbo, ibos.m_["obstacleLD"]);
    initializeVao(vaos.m_["obstacleLD"], vbos.m_["obstacleLD"], ibos.m_["obstacleLD"]);
    initializeTexture(obstacleImage, textures.m_["obstacleLD"]);
}
void releasesBuffers(MyBuffers& vbos, MyBuffers& ibos, MyBuffers& vaos, MyBuffers& textures)
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