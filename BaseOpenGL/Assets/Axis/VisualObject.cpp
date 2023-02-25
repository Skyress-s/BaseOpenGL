#include "pch.h"
#include "VisualObject.h"

KT::VisualObject::VisualObject() {
}

KT::VisualObject::~VisualObject() {
    if (mVAO != NULL) {
        glDeleteVertexArrays(1, &mVAO);
    }
    if (mVBO != NULL) {
        glDeleteBuffers(1, &mVBO);
    }
}

void KT::VisualObject::Update(float deltaTime) {
}

void KT::VisualObject::init(GLint matrixUniform) {
    mMatrixUniform = matrixUniform;

    // vertex array
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // Vertex Buffer
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
