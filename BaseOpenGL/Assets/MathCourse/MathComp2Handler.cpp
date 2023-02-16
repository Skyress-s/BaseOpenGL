#include "MathComp2Handler.h"

#include "../Structure/OctahedronBall.h"

namespace KT
{
    MathComp2Handler::MathComp2Handler()
    {
        points = GenerateRandomPoints();
    }

    std::vector<glm::vec3> MathComp2Handler::GenerateRandomPoints() const
    {
        std::vector<glm::vec3> ps{};

        ps.push_back(glm::vec3(0,0,0));
        ps.push_back(glm::vec3(0,3,0));
        ps.push_back(glm::vec3(0,3,2));
        ps.push_back(glm::vec3(4,3,0));


        
        return ps;
        
    }

    void MathComp2Handler::init(GLint matrixUniform)
    {
        std::vector<KT::Vertex> list = KT::OctahedronBall::makeUnitBall(1);
        std::cout << "verts : " << list.size() << std::endl;
        // std::vector<KT::Vertex> list{};
        // list.push_back(Vertex(0,0,0,1,1,1));
        // list.push_back(Vertex(0,2,0,1,1,1));
        // list.push_back(Vertex(0,2,5,1,1,1));

        for (int i = 0; i < list.size(); ++i)
        {
            list[i].m_xyz[0] *= 0.1f;
            list[i].m_xyz[1] *= 0.1f;
            list[i].m_xyz[2] *= 0.1f;
        }
        
        for (auto vertex : list)
        {
        }

        
        mVertices = list;
        
        
        
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

    void MathComp2Handler::draw()
    {
        mModelMatrix = glm::mat4(1.f);
        for (int i = 0; i < points.size(); ++i)
        {
            std::cout << i << " ";
            mModelMatrix = glm::translate(mModelMatrix, points[i]);

            glBindVertexArray(mVAO);

            glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &mModelMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
        }
    }

    void MathComp2Handler::Update(float deltaTime)
    {
        VisualObject::Update(deltaTime);
    }
}
