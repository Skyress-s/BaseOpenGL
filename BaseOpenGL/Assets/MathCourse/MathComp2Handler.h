#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "../Axis/VisualObject.h"

namespace KT
{
    class MathComp2Handler : public VisualObject
    {
    private:
        std::vector<glm::vec3> points;

    public:
        MathComp2Handler();
        std::vector<glm::vec3> GenerateRandomPoints() const;
        void init(GLint matrixUniform) override;
        void draw() override;
        void Update(float deltaTime) override;
    };
}
