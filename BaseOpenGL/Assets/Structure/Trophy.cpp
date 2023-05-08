#include "pch.h"
#include "Trophy.h"

void KT::Trophy::draw() {
    if (!_collected) {
        KT::VisualObject::DrawVertices(GL_TRIANGLES, GetModelMatrix());
    }
}

void KT::Trophy::init(GLint matrixUniform) {
    VisualObject::init(matrixUniform);
}

void KT::Trophy::Update(float deltaTime) {
    VisualObject::Update(deltaTime);
    if (IsInRange() && !_collected) {
        _collected = true;
        mCurrentPlayerScore++;
        std::cout << "Trophy was collected! Current score: " << mCurrentPlayerScore << std::endl;
    }
}
