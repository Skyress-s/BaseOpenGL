#pragma once
#include <iostream>
#include <glm/vec3.hpp>

namespace MM {
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
        Vertex(float x, float y, float z, float r,float g,float b);
        Vertex(glm::vec3 pos, glm::vec3 nor);
    private:
        float m_xyz[3];       
        float m_normal[3];  
        float m_st[2];


        friend std::ostream& operator<<(std::ostream&, const Vertex&);
        friend std::istream& operator>>(std::istream&, Vertex&);
    };
    
}
