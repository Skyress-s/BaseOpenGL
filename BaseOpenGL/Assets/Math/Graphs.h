﻿#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace KT {
    namespace Graph {
    float Franke(float x, float z) {
        const float e = glm::e<float>();
        float f1 = -(pow(9.f * x - 2.f, 2.f) + pow(9.f * z - 2.f, 2.f)) / 4.f;
        float f2 = -(pow(9.f * x + 1.f, 2.f) / 49.f + (9.f * z + 1.f) / 10.f);
        float f3 = -(pow(9.f * x - 7.f, 2.f) + pow(9.f * z - 3.f, 2.f)) / 4.f;
        float f4 = -(pow(9.f * x - 4.f, 2.f) + pow(9.f * z - 7.f, 2.f));
        return 3.f / 4.f * pow(e, f1) +
            3.f / 4.f * pow(e, f2) +
            0.5f * pow(e, f3) -
            1.f / 5.f * pow(e, f4);
    }
    }
}
