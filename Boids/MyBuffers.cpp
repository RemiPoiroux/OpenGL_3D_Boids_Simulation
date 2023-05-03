#include "MyBuffers.hpp"
#include "Programs.hpp"
#include "Vbos&Ibos.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/matrix.hpp"
#include "img/src/Image.h"

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

void render(p6::Context& ctx, std::vector<Boid>& boids, std::vector<Obstacle>& obstacles, bool lowQuality, MyBuffers& vaos, const glm::mat4& ViewMatrix, MyBuffers& textures, const glm::mat4& ProjMatrix)
{
    auto renderWithoutTexture = [](const glm::mat4& ViewMatrix, const auto& program, glm::mat4& MVMatrix, const glm::mat4& ProjMatrix, const GLuint& vao) {
        program.use();

        MVMatrix               = ViewMatrix * MVMatrix;
        glm::mat4 MVPMatrix    = ProjMatrix * MVMatrix;
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(program.uMVMatrix(), 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(program.uMVPMatrix(), 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(program.uNormalMatrix(), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glBindVertexArray(vao);
        GLint size{};
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    };
    auto renderWithOneTexture = [](const glm::mat4& ViewMatrix, const auto& program, glm::mat4& MVMatrix, const GLuint& texture, const glm::mat4& ProjMatrix, const GLuint& vao) {
        program.use();
        glUniform1i(program.uTexture(), 0);

        MVMatrix               = ViewMatrix * MVMatrix;
        glm::mat4 MVPMatrix    = ProjMatrix * MVMatrix;
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(program.uMVMatrix(), 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(program.uMVPMatrix(), 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(program.uNormalMatrix(), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(vao);
        GLint size{};
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    OneTextureProgram oneTextureProgram = OneTextureProgram();

    // Background Render
    glm::mat4 MVMatrix = glm::mat4(1);
    renderWithOneTexture(ViewMatrix, oneTextureProgram, MVMatrix, textures.m_["background"], ProjMatrix, vaos.m_["background"]);

    // Character Render
    MVMatrix = glm::translate(glm::mat4(1), {0, -0.02, -0.08});
    if (ctx.key_is_pressed(GLFW_KEY_Q) || ctx.key_is_pressed(GLFW_KEY_A) || ctx.key_is_pressed(GLFW_KEY_LEFT))
    {
        MVMatrix = glm::rotate(MVMatrix, -.3f, {0, 0, -1});
    }
    if (ctx.key_is_pressed(GLFW_KEY_E) || ctx.key_is_pressed(GLFW_KEY_D) || ctx.key_is_pressed(GLFW_KEY_RIGHT))
    {
        MVMatrix = glm::rotate(MVMatrix, .3f, {0, 0, -1});
    }
    renderWithOneTexture(glm::mat4{1}, oneTextureProgram, MVMatrix, textures.m_["character"], ProjMatrix, vaos.m_["character"]);
    renderWithoutTexture(glm::mat4{1}, HaloProgram(), MVMatrix, ProjMatrix, vaos.m_["characterReactors"]);

    // Boids Render
    for (Boid& boid : boids)
    {
        MVMatrix            = glm::translate(glm::mat4(1.0f), boid.pos());
        glm::mat4 rotMatrix = boid.getRotationMatrix();
        MVMatrix            = MVMatrix * rotMatrix;
        if (lowQuality)
        {
            renderWithOneTexture(ViewMatrix, oneTextureProgram, MVMatrix, textures.m_["boidLD"], ProjMatrix, vaos.m_["boidLD"]);
        }
        else
        {
            renderWithOneTexture(ViewMatrix, oneTextureProgram, MVMatrix, textures.m_["boidHD"], ProjMatrix, vaos.m_["boidHD"]);
        }
    }

    // Obstacles Render
    for (Obstacle& obstacle : obstacles)
    {
        MVMatrix = glm::translate(glm::mat4(1.0f), obstacle.pos());
        MVMatrix = glm::rotate(MVMatrix, ctx.time() / obstacle.size() / std::sqrt(obstacle.size()) / 50, obstacle.rotationAxis());
        MVMatrix = glm::scale(MVMatrix, obstacle.size() * glm::vec3(1));
        if (lowQuality)
        {
            renderWithOneTexture(ViewMatrix, oneTextureProgram, MVMatrix, textures.m_["obstacleLD"], ProjMatrix, vaos.m_["obstacleLD"]);
        }
        else
        {
            renderWithOneTexture(ViewMatrix, oneTextureProgram, MVMatrix, textures.m_["obstacleHD"], ProjMatrix, vaos.m_["obstacleHD"]);
        }
    }
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