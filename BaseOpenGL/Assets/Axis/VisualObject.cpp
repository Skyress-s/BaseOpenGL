#include "VisualObject.h"

VisualObject::VisualObject() {
    
}

VisualObject::~VisualObject() {
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}

void VisualObject::Update(float deltaTime)
{
}

