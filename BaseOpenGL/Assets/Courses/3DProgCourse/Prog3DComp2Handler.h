#pragma once
#include "../../Axis/VisualObject.h"

namespace KT {
    class Prog3DCom2Handler {
    public:
        static float Graph1(const float& x) {
            return 0.4975 * x*x + 0.6513 * x + 1.2513;
        }

        static float Graph2(const float& x) {
            return -0.5 * x*x*x + 2 * x*x + 0.5 * x;
        }
    public:

        
    };
}
