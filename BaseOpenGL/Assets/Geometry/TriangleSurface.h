#pragma once
#include <functional>

#include "../Axis/VisualObject.h"

namespace KT {
    class TriangleSurface : public VisualObject {
    public:
        TriangleSurface();
        TriangleSurface(std::string fileName);
        ~TriangleSurface();

        void readFile(std::string fileName);
        void toFile(std::string fileName);
        void init(GLint matrixUniform) override;
        void draw() override;
        void construct();
        void constructWithLambda(std::function<float(float, float)> func);
    };
}
