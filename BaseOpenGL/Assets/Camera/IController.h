#pragma once

class IController {
public:
    
    virtual void ProcessKeyboard(glm::vec3 input, float deltaTime) =0;
    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch = true, GLboolean bInvertY = true) =0;
    virtual void ProcessMouseScroll(float yoffset, bool bFOV = false) = 0;
};
