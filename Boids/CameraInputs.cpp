#include "CameraInputs.hpp"

void cameraInputsEvents(const p6::Context& ctx, const CameraParameters& cameraParameters, CharacterCamera& camera)
{
    // camera move
    float factor = 1;
    if (ctx.shift())
    {
        factor = cameraParameters.fastSpeedFactor;
    };

    camera.characterNotRotating();

    if (ctx.key_is_pressed(GLFW_KEY_A))
    {
        camera.moveLeft(factor * cameraParameters.cameraTranslationForce, ctx.delta_time());
        camera.characterRotatingLeft();
    }
    if (ctx.key_is_pressed(GLFW_KEY_D))
    {
        camera.moveLeft(-factor * cameraParameters.cameraTranslationForce, ctx.delta_time());
        camera.characterRotatingRight();
    }

    if (ctx.key_is_pressed(GLFW_KEY_W))
    {
        camera.moveFront(factor * cameraParameters.cameraTranslationForce, ctx.delta_time());
    }
    if (ctx.key_is_pressed(GLFW_KEY_S))
    {
        camera.moveFront(-factor * cameraParameters.cameraTranslationForce, ctx.delta_time());
    }

    // camera rotation
    if (ctx.key_is_pressed(GLFW_KEY_Q))
    {
        camera.rotateLeft(cameraParameters.cameraRotationForce, ctx.delta_time());
        camera.characterRotatingLeft();
    }
    if (ctx.key_is_pressed(GLFW_KEY_E))
    {
        camera.rotateLeft(-cameraParameters.cameraRotationForce, ctx.delta_time());
        camera.characterRotatingRight();
    }
    if (ctx.key_is_pressed(GLFW_KEY_LEFT))
    {
        camera.rotateLeft(cameraParameters.cameraRotationForce, ctx.delta_time());
        camera.characterRotatingLeft();
    }
    if (ctx.key_is_pressed(GLFW_KEY_RIGHT))
    {
        camera.rotateLeft(-cameraParameters.cameraRotationForce, ctx.delta_time());
        camera.characterRotatingRight();
    }
    if (ctx.key_is_pressed(GLFW_KEY_UP))
    {
        camera.rotateUp(cameraParameters.cameraRotationForce, ctx.delta_time());
    }
    if (ctx.key_is_pressed(GLFW_KEY_DOWN))
    {
        camera.rotateUp(-cameraParameters.cameraRotationForce, ctx.delta_time());
    }
}