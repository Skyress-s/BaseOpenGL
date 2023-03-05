#pragma once
#include "Camera.h"

class FollowCamera : public Camera {
private:
    KT::VisualObject* mTarget;
    glm::vec3 offset = glm::vec3(1,3,-1);

public:
    FollowCamera(KT::VisualObject* target) : Camera() {
        mTarget = target;
    }

    void ProcessKeyboard(glm::vec3 input, float deltaTime) override {
        position = mTarget->GetPosition() + offset;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch, GLboolean bInvertY) override {
    }

    glm::mat4 GetViewMatrix() override {
        
        glm::vec3 lookDirection = glm::normalize(mTarget->GetPosition() - mTarget->GetPosition() + offset);
        glm::quat q = glm::quatLookAtRH(lookDirection, glm::vec3(0, 1, 0));
        return glm::lookAt(mTarget->GetPosition() + offset, mTarget->GetPosition(), glm::vec3(0,1,0));
        return mat4_cast(q);
    }
};
