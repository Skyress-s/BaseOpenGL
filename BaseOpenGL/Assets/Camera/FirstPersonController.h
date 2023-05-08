#pragma once
#include "Camera.h"
#include "IController.h"
#include "../Axis/InteractiveObject.h"

namespace KT {
    class FirstPersonController : public IController {
    public:
    public:
        const float SPEED = 2.5f;
        const float SENSETIVITY = 0.05f;
        std::shared_ptr<Camera> mCamera{};
        InteractiveObject* mTargetPlayer{};
        float speed;
        float sensetivity;
        bool mLocked = false;
        FirstPersonController(std::shared_ptr<Camera> camera, InteractiveObject* targetPlayer);

        void ProcessKeyboard(glm::vec3 input, float deltaTime) override;

        void ProcessMouseScroll(float yoffset, bool bFOV) override;

        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch, GLboolean bInvertY) override;
    };
};
