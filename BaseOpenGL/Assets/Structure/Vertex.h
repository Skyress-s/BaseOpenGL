#pragma once
#include <iostream>
#include <glm/vec3.hpp>

namespace KT {
    class Vertex {
    public:
        /*
        float x;
        float y;
        float z;
        float r;
        float g;
        float b;
        float u;
        float v;
        */
        Vertex();
        Vertex(float x, float y, float z, float r, float g, float b);
        Vertex(float x, float y, float z, float r, float g, float b, float u, float v);
        Vertex(glm::vec3 xyz, glm::vec3 rgb, glm::vec2 uv);
        Vertex(float x, float y, float z, glm::vec3 nor);
        Vertex(glm::vec3 pos, glm::vec3 nor);
        float m_xyz[3]{};
        float m_normal[3]{};
        float m_st[2]{};

        // converions
        const glm::vec3 posToVec3();

    private:
        friend std::ostream& operator<<(std::ostream&, const Vertex&);
        friend std::istream& operator>>(std::istream&, Vertex&);
    };
}
