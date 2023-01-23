#pragma once
#include <vector>
#include <glad/glad.h>
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

protected:
    std::vector<MM::Vertex> mVertices;
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mMatrixUniform{0};
    glm::mat4x4 mModelMatrix;

    glm::mat4 GetModelMatrix() const {
        glm::mat4 mat = glm::mat4(1.f);
        return glm::translate(mat, glm::vec3(x, y, z));
    }

    float x{};
    float y{};
    float z{};
};
