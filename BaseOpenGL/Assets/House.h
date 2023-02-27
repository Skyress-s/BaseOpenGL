#pragma once
#include <assimp/anim.h>
#include <assimp/Importer.hpp>

// #include "Geometry/Model.h"
#include "Math/MathHelpers.h"
#include "Structure/Primitives.h"

namespace KT {
    class House : public VisualObject {
    public:
        House(const std::string& modelPath) {
            
        }

        void init(GLint matrixUniform) override {
        }

        void draw() override {
        }
    };
}
