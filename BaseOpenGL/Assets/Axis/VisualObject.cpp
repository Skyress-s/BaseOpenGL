﻿#include "pch.h"
#include "VisualObject.h"

KT::VisualObject::VisualObject()
{
    
}

KT::VisualObject::~VisualObject()
{
    if (mVAO != NULL)
    {
        glDeleteVertexArrays(1, &mVAO);
    }
    if (mVBO != NULL)
    {
        glDeleteBuffers(1, &mVBO);
    }
}

void KT::VisualObject::Update(float deltaTime)
{
}

void KT::VisualObject::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    // vertex array
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // Vertex Buffer
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);
    
    // alt 1: Bind Element Buffer
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO); // binds EBO to VBO!
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mIndices.size(), mIndices.data(), GL_STATIC_DRAW);// sets data

    // attrib pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(2*3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void KT::VisualObject::forceDrawElements() {
    if (mIndices.size() != 0 && mVertices.size() != 0) {
        DrawElements(GL_TRIANGLES, GetModelMatrix());
    }
    else {
        DrawVertices(GL_TRIANGLES, GetModelMatrix());
    }
                
}
