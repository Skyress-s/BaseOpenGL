#pragma once
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
