#pragma once
#include <functional>

#include "../../Axis/VisualObject.h"
#include "../../Math/MathHelpers.h"
#include "../../Structure/OctahedronBall.h"
#include "../../Structure/Range.h"

class GraphNPCWalker : public KT::VisualObject {
private:
    std::function<float(float)> _graph1;
    std::function<float(float)> _graph2;

    Range _range;

    float currX = 0.f;
    float currY = 0.f;
public:
    bool toggle{};
    GraphNPCWalker(std::function<float(float)> graph1, std::function<float(float)> graph2,
        Range range) {
        _graph1 = graph1;
        _graph2 = graph2;
        _range = range;
        
        mVertices = KT::OctahedronBall::makeUnitBall(2 << 2, 0.1f);
    }

    void draw() override {
        
        VisualObject::Draw(GL_TRIANGLES,
            KT::MathHelpers::TRS(GetPosition() + glm::vec3(currX, currY,0), GetRotation(), GetScale())
            );
    }
    void Update(float deltaTime) override{
        currX += deltaTime;
        currX = currX % _range;

        if (toggle)
            currY = _graph1(currX);
        else
            currY = _graph2(currX);
        
    }
    void init(GLint matrixUniform) override {
        VisualObject::init(matrixUniform);
    }
};
