#pragma once
#include <functional>
#include <iostream>
#include "../Axis/VisualObject.h"

namespace MM
{
    class Graph2D : public VisualObject
    {
    public:
        Graph2D(std::function<float(float)> f, int n, float from, float to);
        Graph2D(std::vector<Vertex> verts);
        void init(GLint matrixUniform) override;
        void draw() override;

        void readFile(std::string fileName);
        void toFile(std::string fileName);
    };
    }

