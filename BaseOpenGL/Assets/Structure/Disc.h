#pragma once
#include "../Axis/VisualObject.h"

namespace MM
{
    class Disc : public VisualObject
    {
    public:
        GLuint mEBO;
        std::vector<unsigned int> mIndices;

        void draw() override;
        void init(GLint matrixUniform) override;
        void Update(float deltaTime) override;
        void construct(int segments);
    };
}
