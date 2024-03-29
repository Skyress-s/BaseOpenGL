﻿#include "pch.h"
#include "Tetrahedron.h"

#include <glm/ext/matrix_transform.hpp>

KT::Tetrahedron::Tetrahedron() {
    
    mVertices.push_back(Vertex{1, 0, 0, 1, 0, 0});
    mVertices.push_back(Vertex{-1, 0, 0, 1, 0, 0});
    mVertices.push_back(Vertex{0, 1, 0, 1, 0, 0});
    
    mVertices.push_back(Vertex{0, 0, 1, 0, 1, 0});
    mVertices.push_back(Vertex{1, 0, 0, 0, 1, 0});
    mVertices.push_back(Vertex{-1, 0, 0, 0, 1, 0});
    
    mVertices.push_back(Vertex{0, 0, 1, 0, 0, 1});
    mVertices.push_back(Vertex{0, 1, 0, 0, 0, 1});
    mVertices.push_back(Vertex{-1, 0, 0, 0, 0, 1});
    
    mVertices.push_back(Vertex{0, 0, 1, 1, 1, 0});
    mVertices.push_back(Vertex{0, 1, 0, 1, 1, 0});
    mVertices.push_back(Vertex{1, 0, 0, 1, 1, 0});
    mModelMatrix = glm::mat4x4(1.f); // creates identity matrix
}

KT::Tetrahedron::~Tetrahedron() {
}

void KT::Tetrahedron::init(GLint matrixUniform) {
    mMatrixUniform = matrixUniform;

    // Create and bind vertex arrays, the object that will hold the Vertex Buffer Object
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // Create, bind and populate Vertex Buffer Object,
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    // tells the GPU shader program the structure and size of the data we pass
    // XYZ
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // RGB
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Good practice to unbind vertex arrays
    glBindVertexArray(0);
}

void KT::Tetrahedron::draw() {
    mModelMatrix = GetModelMatrix();
    
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &mModelMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());


    // mMatrix = glm::rotate(mMatrix, 0.0005f, glm::vec3(0.2,1,0));
}
