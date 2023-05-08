#pragma once
#include "playerObject.h"
#include "OctahedronBall.h"

namespace KT {
    class Enemy : public KT::VisualObject {
    public:
        playerObject* _target;
        float _range;

        Enemy(playerObject* player, const float& range) {
            _target = player;
            _range = range;
            mVertices = OctahedronBall::makeUnitBall(1); // skateboard trick
            for (int i = 0; i < mVertices.size(); ++i) {
                mVertices[i].m_xyz[0] *= 0.01f;
                mVertices[i].m_xyz[1] *= 0.01f;
                mVertices[i].m_xyz[2] *= 0.01f;
            }
            for (int i = 0; i < mVertices.size(); ++i) {
                mVertices[i].set_normal(glm::vec3(1,0,0));
            }
            
        }

        bool IsInRange() {
            if (glm::distance(_target->GetPosition(), GetPosition()) < _range)
                return true;
            return false;
        }
        // Exam task 6
        void Update(float deltaTime) override {
            if (IsInRange()) {
                _target->mDead = true;
            }
        }
        void draw() override {
            DrawVertices(GL_TRIANGLES, GetModelMatrix());
        }
    };
}
