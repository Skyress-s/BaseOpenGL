#pragma once
#include "../Axis/InteractiveObject.h"
#include "../Axis/VisualObject.h"
#include "../Geometry/TriangleSurface.h"

namespace KT {
    class playerObject : public InteractiveObject {
    private:
        TriangleSurface* mTarget{};
        unsigned int mCurrentTriangle = 0;
    public:
        bool mDead = false;
        playerObject(TriangleSurface* target, std::vector<Vertex> vertices, std::vector<int> indices, Shader* shader);
        void init(GLint matrixUniform) override;
        void draw() override;

        void ResetToTriangle(int triangleIndex) {
            glm::vec3 pos[3];
            mTarget->GetTrianglePositions(triangleIndex, pos);
            mCurrentTriangle = triangleIndex;
            SetPosition(pos[0] * 0.333f + pos[1] * 0.333f + pos[2] * 0.333f);
        }

        
        // Exam task 4
        void move(float x, float y, float z) override {
            this->x += x * 0.1f;
            this->y += y * 0.1f;
            this->z += z * 0.1f;
            glm::vec3 pos = mTarget->SearchCurrentAndNearest(mCurrentTriangle, GetPosition());
            SetPosition(pos);
        }
        
    };
}
