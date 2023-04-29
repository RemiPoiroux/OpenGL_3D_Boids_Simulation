#include "Inputs.hpp"

void inputsEvents(const p6::Context& ctx, const CameraParameters& cameraParameters, FreeflyCamera& camera)
{
    // camera move
    float factor = 1;
    if (ctx.shift())
    {
        factor = cameraParameters.fastSpeedFactor;
    };

    if (ctx.key_is_pressed(GLFW_KEY_A))
    {
        camera.moveLeft(factor * cameraParameters.cameraTranslationForce);
    }
    if (ctx.key_is_pressed(GLFW_KEY_D))
    {
        camera.moveLeft(-factor * cameraParameters.cameraTranslationForce);
    }
    if (ctx.key_is_pressed(GLFW_KEY_W))
    {
        camera.moveFront(factor * cameraParameters.cameraTranslationForce);
    }
    if (ctx.key_is_pressed(GLFW_KEY_S))
    {
        camera.moveFront(-factor * cameraParameters.cameraTranslationForce);
    }

    // camera rotation
    if (ctx.key_is_pressed(GLFW_KEY_Q))
    {
        camera.rotateLeft(cameraParameters.cameraRotationForce);
    }
    if (ctx.key_is_pressed(GLFW_KEY_E))
    {
        camera.rotateLeft(-cameraParameters.cameraRotationForce);
    }
    if (ctx.key_is_pressed(GLFW_KEY_LEFT))
    {
        camera.rotateLeft(cameraParameters.cameraRotationForce);
    }
    if (ctx.key_is_pressed(GLFW_KEY_RIGHT))
    {
        camera.rotateLeft(-cameraParameters.cameraRotationForce);
    }
    if (ctx.key_is_pressed(GLFW_KEY_UP))
    {
        camera.rotateUp(cameraParameters.cameraRotationForce);
    }
    if (ctx.key_is_pressed(GLFW_KEY_DOWN))
    {
        camera.rotateUp(-cameraParameters.cameraRotationForce);
    }
    if (ctx.mouse_moved)
    {
        if (ctx.alt() || ctx.mouse_button_is_pressed(p6::Button::Right))
        {
            camera.rotateLeft(-ctx.mouse_delta().x * cameraParameters.mouseRotationForce);
            camera.rotateUp(ctx.mouse_delta().y * cameraParameters.mouseRotationForce);
        }
    }
}
