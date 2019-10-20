#pragma once

#include <linmath.h>

///
/// \brief Camera and camera controls
/// Assumes y+ is up
///
class Camera {
public:
    Camera(float movementScale, float rotationScale, float yFOV, float farClip, float nearClip);

    void setLook(const vec3 cameraPosition, const vec3 lookAt);

    void handleKeys(int scancode, bool isDown);
    void handleMouse(int dx, int dy);

    void updatePosition(double dt);

    mat4x4 &calcViewProjection(int windowWidth, int windowHeight);

private:
    void calcLook();

    vec3 deltaPosition = { 0, 0, 0 };
    vec3 position = { 0, 0, 0 };
    vec2 rotation = { 0, 0 };
    const vec3 up = { 0, 1, 0 };
    vec3 look = { 1, 0, 0 };
    mat4x4 vp;
    float movementScale, rotationScale;
    float yFOV;
    float farClip, nearClip;
};
