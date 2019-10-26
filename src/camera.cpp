#include "camera.h"

#include <SDL2/SDL.h>

Camera::Camera(float movementScale, float rotationScale, float yFOV, float farClip, float nearClip)
    : movementScale(movementScale), rotationScale(rotationScale), yFOV(yFOV), farClip(farClip), nearClip(nearClip)
{
}

void Camera::setLook(const vec3 cameraPosition, const vec3 lookAt)
{
    vec3 normLookAt;
    vec3_dup(position, cameraPosition);
    vec3_sub(normLookAt, lookAt, position);
    vec3_norm(normLookAt, normLookAt);
    rotation[1] = asin(normLookAt[1]);
    int sign = (normLookAt[2] > 0) ? 1 : -1;
    rotation[0] = acos(normLookAt[0]/cos(rotation[1])) * sign;
}

void Camera::handleKeys(int scancode, bool isDown)
{
    if (scancode == SDL_SCANCODE_W || scancode == SDL_SCANCODE_UP) keyState[0] = isDown;
    else if (scancode == SDL_SCANCODE_S || scancode == SDL_SCANCODE_DOWN) keyState[1] = isDown;
    else if (scancode == SDL_SCANCODE_D || scancode == SDL_SCANCODE_RIGHT) keyState[2] = isDown;
    else if (scancode == SDL_SCANCODE_A || scancode == SDL_SCANCODE_LEFT) keyState[3] = isDown;
    else if (scancode == SDL_SCANCODE_E || scancode == SDL_SCANCODE_SPACE) keyState[4] = isDown;
    else if (scancode == SDL_SCANCODE_Q || scancode == SDL_SCANCODE_LSHIFT) keyState[5] = isDown;
    else return;

    deltaPosition[2] = movementScale * (keyState[0] - keyState[1]);
    deltaPosition[0] = movementScale * (keyState[2] - keyState[3]);
    deltaPosition[1] = movementScale * (keyState[4] - keyState[5]);
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
