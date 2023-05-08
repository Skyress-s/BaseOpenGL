#pragma once
#include "OctahedronBall.h"
#include "../Axis/InteractiveObject.h"

namespace KT {
    class Trophy : public GeneralVisualObject {
    private:
        InteractiveObject* _target;
        float _range;
        bool _collected = false;
        unsigned int& mCurrentPlayerScore;
    public:
        Trophy(InteractiveObject* player, const float& range, unsigned int& currentPlayerScore) : mCurrentPlayerScore(currentPlayerScore) {
            _target = player;
            _range = range;
            mVertices = std::vector<Vertex>();
            mVertices = OctahedronBall::makeUnitBall(2); // skateboard trick
            for (int i = 0; i < mVertices.size(); ++i) {
                mVertices[i].m_xyz[0] *= 0.01f;
                mVertices[i].m_xyz[1] *= 0.01f;
                mVertices[i].m_xyz[2] *= 0.01f;
                
            }
            for (auto vert : mVertices) {
            }
        }

        bool IsInRange() {
            if (glm::distance(_target->GetPosition(), GetPosition()) < _range) 
                return true;
            return false;
        }
        
        
        void draw() override;
        void init(GLint matrixUniform) override;
        void Update(float deltaTime) override;
    };
}
