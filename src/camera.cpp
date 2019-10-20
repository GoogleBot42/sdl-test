#include "camera.h"

#include <SDL2/SDL.h>

Camera::Camera(float movementScale, float rotationScale, float yFOV, float farClip, float nearClip)
    : movementScale(movementScale), rotationScale(rotationScale), yFOV(yFOV), farClip(farClip), nearClip(nearClip)
{
}

void Camera::handleKeys(int scancode, bool isDown)
{
    if (scancode == SDL_SCANCODE_W) deltaPosition[2] = movementScale * isDown;
    if (scancode == SDL_SCANCODE_S) deltaPosition[2] = -1 * movementScale * isDown;
    if (scancode == SDL_SCANCODE_A) deltaPosition[0] = -1 * movementScale * isDown;
    if (scancode == SDL_SCANCODE_D) deltaPosition[0] = movementScale * isDown;
    if (scancode == SDL_SCANCODE_Q || scancode == SDL_SCANCODE_LSHIFT) deltaPosition[1] = -1 * movementScale * isDown;
    if (scancode == SDL_SCANCODE_E || scancode == SDL_SCANCODE_SPACE) deltaPosition[1] = movementScale * isDown;
}

void Camera::handleMouse(int dx, int dy)
{
    rotation[0] += rotationScale * dx;
    rotation[1] -= rotationScale * dy;
    if (rotation[1] > toRadians(80) ) rotation[1] = toRadians(80);
    if (rotation[1] < toRadians(-80) ) rotation[1] = toRadians(-80);
}

void Camera::updatePosition(double dt)
{
    vec3 delta;
    vec3_scale(delta, deltaPosition, dt);
    calcLook();
    vec3 right, look_without_elevation;
    vec3_mul_cross(right, look, up);
    vec3_norm(right, right);
    vec3_mul_cross(look_without_elevation, up, right);
    vec3_norm(look_without_elevation, look_without_elevation);
    vec3 deltaUp, deltaRight, deltaForward;
    vec3_scale(deltaRight, right, delta[0]);
    vec3_scale(deltaUp, up, delta[1]);
    vec3_scale(deltaForward, look_without_elevation, delta[2]);
    vec3_add(position, position, deltaUp);
    vec3_add(position, position, deltaRight);
    vec3_add(position, position, deltaForward);
}

mat4x4 &Camera::calcViewProjection(int windowWidth, int windowHeight)
{
    mat4x4 view, projection;
    mat4x4_perspective(projection, toRadians(yFOV), (float)windowWidth/(float)windowHeight, nearClip, farClip);
    calcLook();
    vec3 lookat;
    vec3_add(lookat, position, look);
    mat4x4_look_at(view, position, lookat, up);
    mat4x4_mul(vp, projection, view);
    return vp;
}

void Camera::calcLook()
{
    look[0] = cos(rotation[0])*cos(rotation[1]);
    look[1] = sin(rotation[1]);
    look[2] = sin(rotation[0])*cos(rotation[1]);
}
