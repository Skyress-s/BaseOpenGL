#include "MathComp2Handler.h"
#include "../Math/MathHelpers.h"
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

        ps.push_back(glm::vec3(0, 0, 0));
        ps.push_back(glm::vec3(1, 3, 0));
        ps.push_back(glm::vec3(2, 4, 0));
        ps.push_back(glm::vec3(3, 6, 0));
        ps.push_back(glm::vec3(4, 4, 0));
        ps.push_back(glm::vec3(5, 3, 0));
        ps.push_back(glm::vec3(6, 5, 0));
        ps.push_back(glm::vec3(7, 7, 0));
        ps.push_back(glm::vec3(8, 10, 0));

        return ps;
    }

    void MathComp2Handler::init(GLint matrixUniform)
    {
        std::vector<KT::Vertex> list = KT::OctahedronBall::makeUnitBall(1);

        // glm::mat4 test{1.f};
        // test = glm::translate(test, glm::vec3(1,2,3));
        // MathHelpers::printt(test);
        
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
        for (int i = 0; i < points.size(); ++i)
        {
            mModelMatrix = GetModelMatrix();

            glm::vec3 point =  mModelMatrix*glm::vec4(points[i],1);
            
            mModelMatrix = MathHelpers::TRS(point, glm::quat(1,0,0,0), glm::vec3(0.1f));

            
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
