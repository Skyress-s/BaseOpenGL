#include "pch.h"
#include "Primitives.h"

void KT::Primitives::Cube(std::vector<Vertex>& outVerts, std::vector<int>& indices)
{
    
    outVerts = std::vector<Vertex>();
    indices = std::vector<int>();

    const float u = 0.5f;
    // vertices
    {
        glm::vec3 a;
        a = glm::vec3(u,-0.5f,u);
        outVerts.push_back(Vertex(a,a));
        a = glm::vec3(-u,-0.5f,u);
        outVerts.push_back(Vertex(a,a));
        a = glm::vec3(-u,-0.5f,-u);
        outVerts.push_back(Vertex(a,a));
        a = glm::vec3(u,-0.5f,-u);
        outVerts.push_back(Vertex(a,a));

        a = glm::vec3(u,0.5f,u);
        outVerts.push_back(Vertex(a,a));
        a = glm::vec3(-u,0.5f,u);
        outVerts.push_back(Vertex(a,a));
        a = glm::vec3(-u,0.5f,-u);
        outVerts.push_back(Vertex(a,a));
        a = glm::vec3(u,0.5f,-u);
        outVerts.push_back(Vertex(a,a));
    }

    // indices
    {
        // TOP
        // -----------------------------------------------------------------------------------------------------------------
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
    
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);

        // LEFT
        // -----------------------------------------------------------------------------------------------------------------
        indices.push_back(0);
        indices.push_back(3);
        indices.push_back(4);
    
        indices.push_back(3);
        indices.push_back(7);
        indices.push_back(4);

        // RIGHT
        // -----------------------------------------------------------------------------------------------------------------
        indices.push_back(5);
        indices.push_back(2);
        indices.push_back(1);
    
        indices.push_back(2);
        indices.push_back(5);
        indices.push_back(6);
    
        // BACK
        // -----------------------------------------------------------------------------------------------------------------
        indices.push_back(4);
        indices.push_back(1);
        indices.push_back(0);
    
        indices.push_back(1);
        indices.push_back(4);
        indices.push_back(5);
    
        // FRONT
        // -----------------------------------------------------------------------------------------------------------------
        indices.push_back(6);
        indices.push_back(3);
        indices.push_back(2);
    
        indices.push_back(3);
        indices.push_back(6);
        indices.push_back(7);
    
        // TOP
        // -----------------------------------------------------------------------------------------------------------------
        indices.push_back(6);
        indices.push_back(5);
        indices.push_back(4);
    
        indices.push_back(4);
        indices.push_back(7);
        indices.push_back(6);
    }
}
