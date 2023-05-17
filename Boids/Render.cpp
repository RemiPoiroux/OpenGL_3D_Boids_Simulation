#include "Render.hpp"
#include <vector>
#include "CharacterCamera.hpp"
#include "LightsAndMaterials.hpp"
#include "Programs.hpp"
#include "ShadowMapFBO.hpp"
#include "glm/gtc/type_ptr.hpp"

glm::mat4 shadowPass(glm::vec3 lightPosition, p6::Context& ctx, const CharacterCamera& camera, std::vector<Obstacle>& obstacles, std::vector<Boid>& boids, bool lowQuality, MyBuffers& vaos);
void      renderWithoutTexture(const glm::mat4& ViewMatrix, const HaloProgram& program, glm::mat4 MVMatrix, const glm::mat4& ProjMatrix, const GLuint& vao);
void      renderWithOneTexture(const glm::mat4& ViewMatrix, const OneTextureProgram& program, glm::mat4 MVMatrix, const GLuint& texture, const glm::mat4& ProjMatrix, const GLuint& vao);
void      renderWithOneTextureAndLighting(const glm::mat4& ViewMatrix, const OneTextureAndLightsProgram& program, glm::mat4 MVMatrix, const GLuint& texture, const glm::mat4& ProjMatrix, const GLuint& vao, const DirectionalLight& dirLight, const std::vector<PointLight>& pointLights, const Material& material, const glm::mat4& DepthMatrix, ShadowMapFBO& shadowMapFBO);

void render(p6::Context& ctx, std::vector<Boid>& boids, std::vector<Obstacle>& obstacles, std::vector<Laser>& lasers, bool lowQuality, MyBuffers& vaos, const CharacterCamera& camera, MyBuffers& textures, const glm::mat4& ProjMatrix, bool& spotLight)
{
    DirectionalLight globalLight        = DirectionalLight({1, 1, 1}, {0.6, 0.6, 0.6});
    auto             createPointsLights = [&]() -> std::vector<PointLight> {
        float     characterLightOffset    = 0.14;
        float     characterLightIntensity = 0.1;
        glm::vec3 reactorsColor           = {1, 0, 0.2};
        float     reactorsIntensity       = 0.00005;
        glm::vec3 lasersColor             = {1, 0, 0};
        float     lasersIntensity         = 0.0005;

        std::vector<PointLight> pointsLights = {
            PointLight(camera.getPosition() + characterLightOffset * camera.getFrontVector(), 0, {1, 1, 1}),
            PointLight(camera.getTopLReactorPosition(), reactorsIntensity, reactorsColor),
            PointLight(camera.getTopRReactorPosition(), reactorsIntensity, reactorsColor),
            PointLight(camera.getBotLReactorPosition(), reactorsIntensity, reactorsColor),
            PointLight(camera.getBotRReactorPosition(), reactorsIntensity, reactorsColor)};

        // Character light Manager
        if (ctx.key_is_pressed(GLFW_KEY_L))
        {
            spotLight = !spotLight;
        }
        if (spotLight)
        {
            pointsLights[0].setIntensity(characterLightIntensity);
        }

        for (Laser laser : lasers)
        {
            pointsLights.emplace_back(laser.getPosition(), lasersIntensity, lasersColor);
        }

        return pointsLights;
    };
    std::vector<PointLight> pointsLights = createPointsLights();

    // Shadow pass
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLsizei      shadowQuality = 6;
    ShadowMapFBO shadowMapFBO(shadowQuality * ctx.main_canvas_width(), shadowQuality * ctx.main_canvas_height());
    shadowMapFBO.Init();
    shadowMapFBO.BindForWriting();
    glm::mat4 DepthMatrix = shadowPass(globalLight.getDirection(), ctx, camera, obstacles, boids, lowQuality, vaos);

    // Render pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, ctx.main_canvas_width(), ctx.main_canvas_height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    glm::mat4 ViewMatrix = camera.getViewMatrix();

    OneTextureAndLightsProgram oneTextureAndLightsProgram = OneTextureAndLightsProgram();
    Material                   material                   = Material({1, 1, 1}, {0, 0, 0}, 1);
    Material                   boidMaterial               = Material({1, 1, 1}, {0.4, 0.4, 0.4}, 1);

    auto renderBackground = [&]() {
        glm::mat4 MVMatrix = glm::mat4(1);
        renderWithOneTexture(ViewMatrix, OneTextureProgram(), MVMatrix, textures.m_["background"], ProjMatrix, vaos.m_["background"]);
    };
    renderBackground();

    auto renderCharacter = [&]() {
        glm::mat4 MVMatrix = glm::translate(glm::mat4(1), camera.getPosition());
        if (ctx.key_is_pressed(GLFW_KEY_Q) || ctx.key_is_pressed(GLFW_KEY_A) || ctx.key_is_pressed(GLFW_KEY_LEFT))
        {
            MVMatrix = glm::rotate(MVMatrix, -.3f, camera.getFrontVector());
        }
        if (ctx.key_is_pressed(GLFW_KEY_E) || ctx.key_is_pressed(GLFW_KEY_D) || ctx.key_is_pressed(GLFW_KEY_RIGHT))
        {
            MVMatrix = glm::rotate(MVMatrix, .3f, camera.getFrontVector());
        }
        MVMatrix *= camera.getRotationMatrix();

        renderWithOneTextureAndLighting(ViewMatrix, oneTextureAndLightsProgram, MVMatrix, textures.m_["character"], ProjMatrix, vaos.m_["character"], globalLight, pointsLights, material, DepthMatrix, shadowMapFBO);
        renderWithoutTexture(ViewMatrix, HaloProgram(), MVMatrix, ProjMatrix, vaos.m_["characterReactors"]);
    };
    renderCharacter();

    auto renderBoids = [&]() {
        for (Boid& boid : boids)
        {
            glm::mat4 MVMatrix  = glm::translate(glm::mat4(1.0f), boid.getPosition());
            glm::mat4 rotMatrix = boid.getRotationMatrix();
            MVMatrix            = MVMatrix * rotMatrix;
            if (lowQuality)
            {
                renderWithOneTextureAndLighting(ViewMatrix, oneTextureAndLightsProgram, MVMatrix, textures.m_["boidLD"], ProjMatrix, vaos.m_["boidLD"], globalLight, pointsLights, boidMaterial, DepthMatrix, shadowMapFBO);
            }
            else
            {
                renderWithOneTextureAndLighting(ViewMatrix, oneTextureAndLightsProgram, MVMatrix, textures.m_["boidHD"], ProjMatrix, vaos.m_["boidHD"], globalLight, pointsLights, boidMaterial, DepthMatrix, shadowMapFBO);
            }
        }
    };
    renderBoids();

    auto renderObstacles = [&]() {
        for (Obstacle& obstacle : obstacles)
        {
            glm::mat4 MVMatrix = glm::translate(glm::mat4(1.0f), obstacle.getPosition());
            MVMatrix           = glm::rotate(MVMatrix, ctx.time() / obstacle.getSize() / std::sqrt(obstacle.getSize()) / 50, obstacle.getRotationAxis());
            MVMatrix           = glm::scale(MVMatrix, obstacle.getSize() * glm::vec3(1));

            if (lowQuality)
            {
                renderWithOneTextureAndLighting(ViewMatrix, oneTextureAndLightsProgram, MVMatrix, textures.m_["obstacleLD"], ProjMatrix, vaos.m_["obstacleLD"], globalLight, pointsLights, material, DepthMatrix, shadowMapFBO);
            }
            else
            {
                renderWithOneTextureAndLighting(ViewMatrix, oneTextureAndLightsProgram, MVMatrix, textures.m_["obstacleHD"], ProjMatrix, vaos.m_["obstacleHD"], globalLight, pointsLights, material, DepthMatrix, shadowMapFBO);
            }
        }
    };
    renderObstacles();
}

glm::mat4 shadowPass(const glm::vec3 lightPosition, p6::Context& ctx, const CharacterCamera& camera, std::vector<Obstacle>& obstacles, std::vector<Boid>& boids, bool lowQuality, MyBuffers& vaos)
{
    auto renderShadowCaster = [](const glm::mat4& ViewMatrix, const ShadowMapProgram& program, glm::mat4 MVMatrix, const glm::mat4& ProjMatrix, const GLuint& vao) {
        program.use();

        glm::mat4 MVPMatrix = ProjMatrix * ViewMatrix * MVMatrix;

        glUniformMatrix4fv(program.uMVPMatrix(), 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        glBindVertexArray(vao);
        GLint size{};
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    };

    float     farFactor   = 1.f;
    glm::mat4 ProjMatrix  = glm::ortho<float>(-(farFactor + 2), (farFactor + 2), -(farFactor + 2), (farFactor + 2), 0, 2 * (farFactor + 2));
    glm::mat4 ViewMatrix  = glm::lookAt(farFactor * lightPosition, glm::vec3(0), {0, 1, 0});
    glm::mat4 DepthMatrix = ProjMatrix * ViewMatrix;

    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);

    ShadowMapProgram program{};

    program.use();

    auto renderBoids = [&]() {
        for (Boid& boid : boids)
        {
            glm::mat4 MVMatrix  = glm::translate(glm::mat4(1.0f), boid.getPosition());
            glm::mat4 rotMatrix = boid.getRotationMatrix();
            MVMatrix            = MVMatrix * rotMatrix;
            if (lowQuality)
            {
                renderShadowCaster(ViewMatrix, program, MVMatrix, ProjMatrix, vaos.m_["boidLD"]);
            }
            else
            {
                renderShadowCaster(ViewMatrix, program, MVMatrix, ProjMatrix, vaos.m_["boidHD"]);
            }
        }
    };
    renderBoids();

    auto renderCharacter = [&]() {
        glm::mat4 MVMatrix = glm::translate(glm::mat4(1), camera.getPosition());
        if (ctx.key_is_pressed(GLFW_KEY_Q) || ctx.key_is_pressed(GLFW_KEY_A) || ctx.key_is_pressed(GLFW_KEY_LEFT))
        {
            MVMatrix = glm::rotate(MVMatrix, -.3f, camera.getFrontVector());
        }
        if (ctx.key_is_pressed(GLFW_KEY_E) || ctx.key_is_pressed(GLFW_KEY_D) || ctx.key_is_pressed(GLFW_KEY_RIGHT))
        {
            MVMatrix = glm::rotate(MVMatrix, .3f, camera.getFrontVector());
        }
        MVMatrix *= camera.getRotationMatrix();
        renderShadowCaster(ViewMatrix, program, MVMatrix, ProjMatrix, vaos.m_["character"]);
    };
    renderCharacter();

    auto renderObstacles = [&]() {
        for (const Obstacle& obstacle : obstacles)
        {
            glm::mat4 MVMatrix = glm::translate(glm::mat4(1.0f), obstacle.getPosition());
            MVMatrix           = glm::rotate(MVMatrix, ctx.time() / obstacle.getSize() / std::sqrt(obstacle.getSize()) / 50, obstacle.getRotationAxis());
            MVMatrix           = glm::scale(MVMatrix, obstacle.getSize() * glm::vec3(1));

            if (lowQuality)
            {
                renderShadowCaster(ViewMatrix, program, MVMatrix, ProjMatrix, vaos.m_["obstacleLD"]);
            }
            else
            {
                renderShadowCaster(ViewMatrix, program, MVMatrix, ProjMatrix, vaos.m_["obstacleHD"]);
            }
        }
    };
    renderObstacles();

    return DepthMatrix;
}

void renderWithoutTexture(const glm::mat4& ViewMatrix, const HaloProgram& program, glm::mat4 MVMatrix, const glm::mat4& ProjMatrix, const GLuint& vao)
{
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
}

void renderWithOneTexture(const glm::mat4& ViewMatrix, const OneTextureProgram& program, glm::mat4 MVMatrix, const GLuint& texture, const glm::mat4& ProjMatrix, const GLuint& vao)
{
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
}

void renderWithOneTextureAndLighting(const glm::mat4& ViewMatrix, const OneTextureAndLightsProgram& program, glm::mat4 MVMatrix, const GLuint& texture, const glm::mat4& ProjMatrix, const GLuint& vao, const DirectionalLight& dirLight, const std::vector<PointLight>& pointLights, const Material& material, const glm::mat4& DepthMatrix, ShadowMapFBO& shadowMapFBO)
{
    program.use();

    shadowMapFBO.BindForReading(GL_TEXTURE1);
    glUniform1i(program.uShadowMap(), 1);
    glUniformMatrix4fv(program.uDepthMVP(), 1, GL_FALSE, glm::value_ptr(DepthMatrix * MVMatrix));

    MVMatrix               = ViewMatrix * MVMatrix;
    glm::mat4 MVPMatrix    = ProjMatrix * MVMatrix;
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    glUniformMatrix4fv(program.uMVMatrix(), 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(program.uMVPMatrix(), 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    glUniformMatrix4fv(program.uNormalMatrix(), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glUniform1i(program.uTexture(), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUniform3fv(program.uDirLightDir_vs(), 1, glm::value_ptr(ViewMatrix * glm::vec4(dirLight.getDirection(), 0.f)));
    glUniform3fv(program.uDirLightColor(), 1, glm::value_ptr(dirLight.getColor()));

    const auto pointLightsNumber = static_cast<GLint>(pointLights.size());
    glUniform1i(program.uNumPointLights(), pointLightsNumber);

    std::vector<glm::vec3> pointLightsPositions(pointLightsNumber);
    std::vector<glm::vec3> pointLightsColors(pointLightsNumber);
    std::vector<GLfloat>   pointLightsIntensities(pointLightsNumber);
    for (int i = 0; i < pointLightsNumber; ++i)
    {
        pointLightsPositions[i]   = ViewMatrix * glm::vec4(pointLights[i].getPosition(), 1);
        pointLightsColors[i]      = pointLights[i].getColor();
        pointLightsIntensities[i] = pointLights[i].getIntensity();
    }

    glUniform3fv(program.uPointLightsPositions(), pointLightsNumber, glm::value_ptr(pointLightsPositions[0]));
    glUniform3fv(program.uPointLightsColors(), pointLightsNumber, glm::value_ptr(pointLightsColors[0]));
    glUniform1fv(program.uPointLightsIntensities(), pointLightsNumber, pointLightsIntensities.data());

    glUniform3fv(program.uKd(), 1, glm::value_ptr(material.getKd()));
    glUniform3fv(program.uKs(), 1, glm::value_ptr(material.getKs()));
    glUniform1f(program.uShininess(), material.getShininess());

    glBindVertexArray(vao);
    GLint size{};
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}