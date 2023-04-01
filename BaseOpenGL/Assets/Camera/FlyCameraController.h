#pragma once
#include "IController.h"

class Camera;

const float SPEED = 5.5f;
const float SENSETIVITY = 0.05f;
class FlyCameraController : public IController {
public:
    
    float speed;
    float sensetivity;
    bool mLocked = false;
    
    std::shared_ptr<Camera> mCamera{};
    FlyCameraController(std::shared_ptr<Camera> camera) : mCamera(camera), speed(SPEED), sensetivity(SENSETIVITY) {
        
    }
    
    void ProcessKeyboard(glm::vec3 input, float deltaTime) override;

    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch = true,
                                      GLboolean bInvertY = true);

    void ProcessMouseScroll(float yoffset, bool bFOV = false);
};
