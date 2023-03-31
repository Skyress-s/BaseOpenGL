#pragma once
#include <functional>

#include "VisualObject.h"

namespace KT {
    class InteractiveObject : public VisualObject {
    public:
        InteractiveObject();
        ~InteractiveObject() override;
            
        virtual void move(float x, float y, std::function<float(float,float)>);
        virtual void move(float x, float y, float z);

    private:
    };
}
