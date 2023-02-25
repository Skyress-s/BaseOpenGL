#include "pch.h"
#include "OctahedronBall.h"

namespace KT
{
    OctahedronBall::OctahedronBall()
    {
    }

    OctahedronBall::OctahedronBall(int recurtions) : _recurtions(recurtions), index(0), VisualObject()
    {
        mVertices.reserve(3 * 8 * pow(4, recurtions));
        makeUnitBall();
    }

    OctahedronBall::~OctahedronBall()
    {
    }

    void OctahedronBall::draw()
    {
        mModelMatrix = GetModelMatrix();

        glBindVertexArray(mVAO);

        glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &mModelMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    }

    void OctahedronBall::init(GLint matrixUniform)
    {
        mMatrixUniform = matrixUniform;

        // vertex array
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);

        // Vertex Buffer
        glGenBuffers(1, &mVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    std::vector<Vertex> OctahedronBall::makeUnitBall(int _recurtions, float scale)
    {
        glm::vec3 v0{0, 0, 1};
        glm::vec3 v1{1, 0, 0};
        glm::vec3 v2{0, 1, 0};
        glm::vec3 v3{-1, 0, 0};
        glm::vec3 v4{0, -1, 0};
        glm::vec3 v5{0, 0, -1};

        std::vector<KT::Vertex> list = std::vector<Vertex>(0);


        
        Subdivide(v0, v1, v2, _recurtions, list);
        Subdivide(v0, v2, v3, _recurtions, list);
        Subdivide(v0, v3, v4, _recurtions, list);
        Subdivide(v0, v4, v1, _recurtions, list);
        Subdivide(v5, v2, v1, _recurtions, list);
        Subdivide(v5, v3, v2, _recurtions, list);
        Subdivide(v5, v4, v3, _recurtions, list);
        Subdivide(v5, v1, v4, _recurtions, list);
        
        for (int i = 0; i < list.size(); ++i) {
            list[i].m_xyz[0] *= scale;   
            list[i].m_xyz[1] *= scale;   
            list[i].m_xyz[2] *= scale;   
        }
        return list;
    }

    void OctahedronBall::makeUnitBall()
    {
        glm::vec3 v0{0, 0, 1};
        glm::vec3 v1{1, 0, 0};
        glm::vec3 v2{0, 1, 0};
        glm::vec3 v3{-1, 0, 0};
        glm::vec3 v4{0, -1, 0};
        glm::vec3 v5{0, 0, -1};


        Subdivide(v0, v1, v2, _recurtions);
        Subdivide(v0, v2, v3, _recurtions);
        Subdivide(v0, v3, v4, _recurtions);
        Subdivide(v0, v4, v1, _recurtions);
        Subdivide(v5, v2, v1, _recurtions);
        Subdivide(v5, v3, v2, _recurtions);
        Subdivide(v5, v4, v3, _recurtions);
        Subdivide(v5, v1, v4, _recurtions);
    }

    void OctahedronBall::lagTriangel(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
    {
        glm::vec3 center = (v1 + v2 + v3) / 3.f;
        center = center / glm::length(center);
        Vertex v{v1.x, v1.y, v1.z, v1.x, v1.y, v1.z};
        mVertices.push_back(v);
        v = Vertex{v2.x, v2.y, v2.z, v2.x, v2.y, v2.z};
        mVertices.push_back(v);
        v = Vertex{v3.x, v3.y, v3.z, v3.x, v3.y, v3.z};
        mVertices.push_back(v);
    }

    void OctahedronBall::lagTriangel(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
                                     std::vector<KT::Vertex>& list)
    {
        glm::vec3 center = (v1 + v2 + v3) / 3.f;
        center = center / glm::length(center);
        Vertex v{v1.x, v1.y, v1.z, v1.x, v1.y, v1.z};
        list.push_back(v);
        v = Vertex{v2.x, v2.y, v2.z, v2.x, v2.y, v2.z};
        list.push_back(v);
        v = Vertex{v3.x, v3.y, v3.z, v3.x, v3.y, v3.z};
        list.push_back(v);
    }

    void OctahedronBall::Subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, int n)
    {
        if (n > 0)
        {
            glm::vec3 v1 = a + b;
            v1 = normalize(v1);
            glm::vec3 v2 = a + c;
            v2 = normalize(v2);
            glm::vec3 v3 = b + c;
            v3 = normalize(v3);

            Subdivide(a, v1, v2, n - 1);
            Subdivide(c, v2, v3, n - 1);
            Subdivide(b, v3, v1, n - 1);
            Subdivide(v3, v2, v1, n - 1);
        }
        else
        {
            lagTriangel(a, b, c);
        }
    }

    void OctahedronBall::Subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, int n,
                                   std::vector<KT::Vertex>& list)
    {
        if (n > 0)
        {
            glm::vec3 v1 = a + b;
            v1 = normalize(v1);
            glm::vec3 v2 = a + c;
            v2 = normalize(v2);
            glm::vec3 v3 = b + c;
            v3 = normalize(v3);

            Subdivide(a, v1, v2, n - 1, list);
            Subdivide(c, v2, v3, n - 1, list);
            Subdivide(b, v3, v1, n - 1, list);
            Subdivide(v3, v2, v1, n - 1, list);
        }
        else
        {
            lagTriangel(a, b, c, list);
        }
    }
}
