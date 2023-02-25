#include "pch.h"
#include "Trophy.h"

void KT::Trophy::draw() {
    if (!_collected) {
        KT::VisualObject::Draw(GL_TRIANGLES, GetModelMatrix());
    }
}

void KT::Trophy::init(GLint matrixUniform) {
    VisualObject::init(matrixUniform);
}

void KT::Trophy::Update(float deltaTime) {
    VisualObject::Update(deltaTime);
    if (IsInRange()) {
        _collected = true;
    }
}
