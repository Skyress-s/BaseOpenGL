#pragma once
#include "../Axis/VisualObject.h"

namespace MM {
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
    };
}
