#pragma once
#include "../Axis/InteractiveObject.h"
#include "../Axis/VisualObject.h"

namespace KT {
    class Cube : public InteractiveObject {
    public:
        Cube();
        void init(GLint matrixUniform) override;
        void draw() override;
    };
}
