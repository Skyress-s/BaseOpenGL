#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <glm/ext/matrix_transform.hpp>

//defualt values
const glm::vec3 POSITION = glm::vec3(0,0,0);
const glm::vec3 FRONT = glm::vec3(-1, 0 ,0);
const glm::vec3 UP = glm::vec3(0,1,0);

const float YAW = 90.f;
const float PITCH = 0.f;
const float SPEED = 5.5f;
const float SENSETIVITY = 0.05f;
const float FOV = 60.f;

class Camera {
public:
    //transform
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    bool mLocked = false;

    
    float pitch;
    float yaw;
    float roll = 0.f;

    
    //movement
    float speed;
    float sensetivity;
    //other
    float fov = 70.f;


    Camera(glm::vec3 _position = POSITION, glm::vec3 _up = UP, float _yaw = YAW, float _pitch = PITCH): forward(FRONT), speed(SPEED), sensetivity(SENSETIVITY), fov(FOV) {
        position = _position;
        worldUp = _up;
        yaw = _yaw;
        pitch = _pitch;
        UpdateCameraVectors();
    }
    
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() {
        return SelfLookAtImplementation(position, position + forward, up);
        // return SelfLookAtImplementation(position, position + forward, up);
    }

    void ProcessKeyboard(glm::vec3 input, float deltaTime) {
        if (mLocked)
            return;
            
        float velocity = deltaTime * speed;
        position += right * input.x * velocity;
        position += forward * input.z * velocity;
        position += up * input.y * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch = true, GLboolean bInvertY = true) {
        if (mLocked)
            return;
            
        xoffset *= sensetivity;
        yoffset *= sensetivity;
        
        if (bInvertY)
            yoffset *= -1.f;
        
        yaw     += xoffset;
        pitch   += yoffset;
        //clamps
        if (bConstrainPitch) {
            //clamps
            float maxPitch = 89.5f;
            pitch = pitch > maxPitch ? maxPitch : pitch;
            pitch = pitch < -maxPitch ? -maxPitch : pitch;
        }
        UpdateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset, bool bFOV = false) {
        if (mLocked)
            return;
        
        if (bFOV) {
            fov += -yoffset * 5.f;
            fov = fov < 1.f ? 1.f : fov;
            fov = fov > 80.f ? 80.f : fov;
        }
        else {
            speed += yoffset;
            speed = speed > 145.f ? 145.f : speed;
            speed = speed < 1.f ? 1.f : speed;
        }
    }

    
    
    void UpdateCameraVectors() {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward = glm::normalize(front);
        // also re-calculate the Right and Up vector
        right   = glm::normalize(glm::cross(forward, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up      = glm::normalize(glm::cross(right, forward));
    }
private:

    glm::mat4 SelfLookAtImplementation(glm::vec3 _position, glm::vec3 _target, glm::vec3 _up) const {
        glm::vec3 forwardAxis = glm::normalize(_position - _target);
        glm::vec3 rightAxis = glm::normalize(glm::cross(_up, forwardAxis));
        glm::vec3 upAxis = glm::normalize(glm::cross(forwardAxis, rightAxis));
        
        glm::mat4 rotation(1.f);
        rotation[0][0] = rightAxis.x;
        rotation[1][0] = rightAxis.y;
        rotation[2][0] = rightAxis.z;
        rotation[0][1] = upAxis.x;
        rotation[1][1] = upAxis.y;
        rotation[2][1] = upAxis.z;
        rotation[0][2] = forwardAxis.x;
        rotation[1][2] = forwardAxis.y;
        rotation[2][2] = forwardAxis.z;
        
        glm::mat4 translation(1.f);
        translation[3][0] = -position.x;
        translation[3][1] = -position.y;
        translation[3][2] = -position.z;

        return rotation * translation;
    }

    
};


#endif