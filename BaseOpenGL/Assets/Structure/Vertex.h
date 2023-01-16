#pragma once
#include <iostream>

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
    private:
        float m_xyz[3];       
        float m_normal[3];  
        float m_st[2];


        friend std::ostream& operator<<(std::ostream&, const Vertex&);
        friend std::istream& operator>>(std::istream&, Vertex&);
    };
    
}
