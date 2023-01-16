#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "../Structure/Vertex.h"

class VisualObject
{
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init( GLint matrixUniform ) = 0;
    virtual void draw() = 0;
protected:
    std::vector<MM::Vertex> mVertices;
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mMatrixUniform{0};
    glm::mat4x4 mMatrix;
};
