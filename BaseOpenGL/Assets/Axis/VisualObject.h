#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../Structure/Vertex.h"

class VisualObject {
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init(GLint matrixUniform) = 0;
    virtual void draw() = 0;
    std::string name{"N/A"};
    
    glm::vec3 GetPosition() {
        return glm::vec3(x,y,z);
        glm::vec3 pos{};
        
        pos.x = mModelMatrix[3][0];
        pos.y = mModelMatrix[3][1];
        pos.z = mModelMatrix[3][2];

        return pos;
    }

    void SetPosition(glm::vec3 newPosition) {
        x = newPosition.x;
        y = newPosition.y;
        z = newPosition.z;
    }

    void SetScale(glm::vec3 newScale) {
        sx = newScale[0];
        sy = newScale[1];
        sz = newScale[2];
    }

    glm::mat4 GetRotationEulerAngles() {
                
    }

    void SetRotation(glm::vec3 rot) {
        _rotation = glm::mat4(1);

        _rotation = glm::rotate(_rotation, glm::length(rot), normalize(rot));

        return;
        _rotation = glm::rotate(_rotation, rot.x, glm::vec3(1,0,0));
        _rotation = glm::rotate(_rotation, rot.y, glm::vec3(0,1,0));
        _rotation = glm::rotate(_rotation, rot.z, glm::vec3(0,0,1));
    }
    

protected:
    std::vector<MM::Vertex> mVertices;
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mMatrixUniform{0};
    glm::mat4x4 mModelMatrix;

    glm::mat4 GetModelMatrix() const {
        glm::mat4 mat = glm::mat4(1.f);
        mat = glm::translate(mat, glm::vec3(x, y, z));
        mat *= _rotation; 
        mat = glm::scale(mat, glm::vec3(sx,sy,sz));
        return mat;
    }

    float x{};
    float y{};
    float z{};

    glm::mat4 _rotation = glm::mat4(1.f);
    
    // float rx{};
    // float ry{};
    // float rz{};
    //
    float sx = 1.f;
    float sy = 1.f;
    float sz = 1.f;
};
