#include "pch.h"
#include "ThirdPersonController.h"
#include "Camera.h"

KT::ThirdPersonController::ThirdPersonController(std::shared_ptr<Camera> camera, InteractiveObject* targetPlayer)
    : mCamera(camera), mTargetPlayer(targetPlayer), speed(SPEED), sensetivity(SENSETIVITY) {
}

void KT::ThirdPersonController::ProcessKeyboard(glm::vec3 input, float deltaTime) {
    if (mLocked)
        return;
    glm::vec3 offset = glm::vec3(0, 1, -1) * 0.2f;
    float velocity = deltaTime * speed;
    // mCamera->position = mTargetPlayer->GetPosition() + offset;

    glm::vec3 forward = mCamera->forward;
    forward.y = 0.f;
    if (glm::length(forward) > 0.1)
        forward = glm::normalize(forward) * input.z;
    
    glm::vec3 left = -mCamera->right;
    left.y = 0.f;
    if (glm::length(left) > 0.1)
        left = glm::normalize(left) * input.x;

    glm::vec3 combined = glm::length(forward + left) > 0.1 ? glm::normalize(forward + left) : glm::vec3(0, 0, 0);
    combined *= velocity;


    mTargetPlayer->move(combined.x, 0, combined.z);
    // mTargetPlayer->move(input.x * deltaTime, 0, input.z * deltaTime);
    // mCamera->position += mCamera->right * input.x * velocity;
    // mCamera->position += mCamera->forward * input.z * velocity;
    // mCamera->position += mCamera->up * input.y * velocity;
}

void KT::ThirdPersonController::ProcessMouseScroll(float yoffset, bool bFOV) {
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

void KT::ThirdPersonController::ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch,
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
    glm::vec3 pos = mTargetPlayer->GetPosition() - mCamera->forward * 0.15f;
    mCamera->position = pos;
}
