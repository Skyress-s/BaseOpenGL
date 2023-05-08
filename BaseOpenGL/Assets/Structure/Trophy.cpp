#include "pch.h"
#include "Trophy.h"

void KT::Trophy::draw() {
    if (!_collected) {
        GeneralVisualObject::draw();
        // KT::VisualObject::DrawVertices(GL_TRIANGLES, GetModelMatrix());
    }
}

void KT::Trophy::init(GLint matrixUniform) {
    VisualObject::init(matrixUniform);
}
// Exam task 5 // Exam task 7 
void KT::Trophy::Update(float deltaTime) {
    VisualObject::Update(deltaTime);
    if (IsInRange() && !_collected) {
        _collected = true;
        mCurrentPlayerScore++;
        std::cout << "Trophy was collected! Current score: " << mCurrentPlayerScore << std::endl;
    }
}
