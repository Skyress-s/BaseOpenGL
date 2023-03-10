#pragma once
#include <functional>

#include "VisualObject.h"

namespace KT {
    class InteractiveObject : public VisualObject {
    public:
        InteractiveObject();
        ~InteractiveObject() override;
            
        void move(float x, float y, std::function<float(float,float)>);
        void move(float x, float y, float z);

    private:
    };
}
