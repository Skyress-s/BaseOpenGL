#pragma once
#include "VisualObject.h"

namespace MM {
    class InteractiveObject : public VisualObject {
    public:
        InteractiveObject();
        ~InteractiveObject() override;
            

        void move(float x, float y, float z);

    private:
    };
}
