#pragma once
#include "../Axis/VisualObject.h"

namespace KT {
    class Tetrahedron : public VisualObject {
    public:
        Tetrahedron();
        ~Tetrahedron();
        void init(GLint matrixUniform) override;
        void draw() override;
    };
}
