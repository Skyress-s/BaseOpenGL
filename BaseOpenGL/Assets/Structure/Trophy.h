#pragma once
#include "OctahedronBall.h"
#include "../Axis/InteractiveObject.h"

namespace KT {
    class Trophy : public VisualObject {
    private:
        InteractiveObject* _target;
        float _range;
    public:
        Trophy(InteractiveObject* player, const float& range) {
            _target = player;
            _range = range;
            mVertices = std::vector<Vertex>();
            mVertices = OctahedronBall::makeUnitBall(2); // skateboard trick
            for (int i = 0; i < mVertices.size(); ++i) {
                mVertices[i].m_xyz[0] *= 0.1f;
                mVertices[i].m_xyz[1] *= 0.1f;
                mVertices[i].m_xyz[2] *= 0.1f;
                
            }
            for (auto vert : mVertices) {
            }
        }

        bool IsInRange() {
            if (glm::distance(_target->GetPosition(), GetPosition())) 
                return true;
            return false;
        }
        
        
        void draw() override;
        void init(GLint matrixUniform) override;
        void Update(float deltaTime) override;
    };
}
