#include "pch.h"
#include "FlyCameraController.h"
#include "Camera.h"
void FlyCameraController::ProcessKeyboard(glm::vec3 input, float deltaTime) {
    if (mLocked)
    return;

    float velocity = deltaTime * speed;
    mCamera->position += mCamera->right * input.x * velocity;
    mCamera->position += mCamera->forward * input.z * velocity;
    mCamera->position += mCamera->up * input.y * velocity;
}

void FlyCameraController::ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch,
    GLboolean bInvertY) {
    if (mLocked)
        return;

    xoffset *= sensetivity;
    yoffset *= sensetivity;

    if (bInvertY)
        yoffset *= -1.f;

    mCamera->yaw += xoffset;
    mCamera->pitch += yoffset;
    //clamps
    if (bConstrainPitch) {
        //clamps
        float maxPitch = 89.5f;
        mCamera->pitch = mCamera->pitch > maxPitch ? maxPitch : mCamera->pitch;
        mCamera->pitch = mCamera->pitch < -maxPitch ? -maxPitch : mCamera->pitch;
    }
    mCamera->UpdateCameraVectors();
}

void FlyCameraController::ProcessMouseScroll(float yoffset, bool bFOV) {
    if (mLocked)
        return;

    if (bFOV) {
        mCamera->fov += -yoffset * 5.f;
        mCamera->fov = mCamera->fov < 1.f ? 1.f : mCamera->fov;
        mCamera->fov = mCamera->fov > 80.f ? 80.f : mCamera->fov;
    }
    else {
        speed += yoffset;
        speed = speed > 145.f ? 145.f : speed;
        speed = speed < 1.f ? 1.f : speed;
    }
}
