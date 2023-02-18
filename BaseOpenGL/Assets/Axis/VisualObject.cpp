#include "VisualObject.h"

VisualObject::VisualObject() {
}

VisualObject::~VisualObject() {
    if (mVAO != NULL) {
        glDeleteVertexArrays(1, &mVAO);
    }
    if (mVBO != NULL) {
        glDeleteBuffers(1, &mVBO);
    }
}

void VisualObject::Update(float deltaTime) {
}
