#pragma once
#include "IController.h"
#include "../Axis/InteractiveObject.h"

class Camera;
// Exam task 1
namespace KT {

    class ThirdPersonController : public IController {
    public:
        const float SPEED = 2.5f;
        const float SENSETIVITY = 0.05f;
        std::shared_ptr<Camera> mCamera{};
        InteractiveObject* mTargetPlayer{};
        float speed;
        float sensetivity;
        bool mLocked = false;
        ThirdPersonController(std::shared_ptr<Camera> camera, InteractiveObject* targetPlayer);

        void ProcessKeyboard(glm::vec3 input, float deltaTime) override;

        void ProcessMouseScroll(float yoffset, bool bFOV) override;

        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch, GLboolean bInvertY) override;
    };
}
