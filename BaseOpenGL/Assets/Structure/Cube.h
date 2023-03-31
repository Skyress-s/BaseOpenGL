#pragma once
#include "../Axis/InteractiveObject.h"
#include "../Axis/VisualObject.h"
#include "../Geometry/TriangleSurface.h"

namespace KT {
    class Cube : public InteractiveObject {
    private:
        TriangleSurface* mTarget{};
        unsigned int mCurrentTriangle = 0;
    public:
        Cube(TriangleSurface* target);
        void init(GLint matrixUniform) override;
        void draw() override;

        void move(float x, float y, float z) override {
            this->x += x * 0.1f;
            this->y += y * 0.1f;
            this->z += z * 0.1f;
            glm::vec3 pos = mTarget->SearchCurrentAndNearest(mCurrentTriangle, GetPosition());
            SetPosition(pos);
        }
        
    };
}
