#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Structure/Vertex.h"

class VisualObject {
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init(GLint matrixUniform) = 0;
    virtual void draw() = 0;
    virtual void Update(float deltaTime);
    std::string name{"N/A"};
    
    glm::vec3 GetPosition() {
        return glm::vec3(x,y,z);
        glm::vec3 pos{};
        
        pos.x = mModelMatrix[3][0];
        pos.y = mModelMatrix[3][1];
        pos.z = mModelMatrix[3][2];

        return pos;
    }

    void SetPosition(const float& x, const float& y, const float& z) {
        SetPosition(glm::vec3(x,y,z));
    }

    void SetPosition(glm::vec3 newPosition) {
        x = newPosition.x;
        y = newPosition.y;
        z = newPosition.z;
    }

    glm::vec3 GetScale() const
    {
        return glm::vec3(sx,sy,sz);
    }
    
    void SetScale(glm::vec3 newScale) {
        sx = newScale[0];
        sy = newScale[1];
        sz = newScale[2];
    }

    void SetScale(const float& x, const float& y, const float& z)
    {
        sx =x;
        sy = y;
        sz = z;
    }

    // glm::mat4 GetRotationEulerAngles() {
        
    // }

    void SetRotation(glm::vec3 rot) {
        _rotation = glm::quat(rot);
    }

    void SetRotation(const float& x, const float& y,const float& z)
    {
        SetRotation(glm::vec3(x,y,z));
    }
    

protected:
    std::vector<KT::Vertex> mVertices;
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mMatrixUniform{0};
    glm::mat4x4 mModelMatrix;

    glm::mat4 GetModelMatrix() const {
        glm::mat4 mat = glm::mat4(1.f);
        mat = glm::translate(mat, glm::vec3(x, y, z));
        mat *=  glm::mat4_cast(_rotation); 
        mat = glm::scale(mat, glm::vec3(sx,sy,sz));
        return mat;
    }

    float x{};
    float y{};
    float z{};

    glm::quat _rotation = glm::quat(1.f,0,0,0);
    
    // float rx{};
    // float ry{};
    // float rz{};
    //
    float sx = 1.f;
    float sy = 1.f;
    float sz = 1.f;
};
