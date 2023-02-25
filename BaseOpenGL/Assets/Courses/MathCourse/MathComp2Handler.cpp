#include "pch.h"
#include "MathComp2Handler.h"
#include "../../Math/MathHelpers.h"
#include "../../Structure/OctahedronBall.h"
#include <Eigen/Dense>

namespace KT {
    MathComp2Handler::MathComp2Handler() {
        
    }

    MathComp2Handler::~MathComp2Handler() {
    }

    void MathComp2Handler::HandleTask1() {
        points = GenerateRandomPoints();

        Eigen::MatrixXd A = Eigen::MatrixXd(points.size(), 3);
        A.setZero();
        for (int i = 0; i < points.size(); ++i) {
            A(i, 0) = pow(points[i].x, 2.f);
            A(i, 1) = points[i].x;
            A(i, 2) = 1.f;
        }
        std::cout << A << std::endl;
        Eigen::MatrixXd y = Eigen::MatrixXd(points.size(), 1);
        for (int i = 0; i < points.size(); ++i) {
            y(i, 0) = points[i].y;
        }
        
        std::cout << y << std::endl;
        
        Eigen::MatrixXd B = Eigen::Transpose<Eigen::MatrixXd>(A) * A;
        std::cout << B << std::endl;
        Eigen::MatrixXd c = Eigen::Transpose<Eigen::MatrixXd>(A) * y;

        Eigen::MatrixXd x = Eigen::Inverse<Eigen::MatrixXd>(B)*c;
        std::cout << "result" << std::endl << x << std::endl;
        
        // writing mVerices
        mVertices.clear();
        float min = -4.f;
        float max = 3.f;
        int divisions = 10;
        float delta = (max - min)/divisions;
        
        for (float i = min; i <= max+0.001f; i+=delta) {
            float y =
                x(0,0)*i*i +
                x(1,0)*i +
                x(2,0);
            Vertex v = Vertex(glm::vec3(i, y,0), glm::vec3(0,0,0));
            mVertices.push_back(v);    
        }
        
    }

    std::vector<glm::vec3> MathComp2Handler::GenerateRandomPoints() const {
        std::vector<glm::vec3> ps{};

        ps.push_back(glm::vec3(-4, 7, 0));
        ps.push_back(glm::vec3(-2.5, 4, 0));
        ps.push_back(glm::vec3(-2.3, 1, 0));
        ps.push_back(glm::vec3(-1.5, 0, 0));
        ps.push_back(glm::vec3(0, 2, 0));
        ps.push_back(glm::vec3(1, 3, 0));
        ps.push_back(glm::vec3(2, 5, 0));
        ps.push_back(glm::vec3(3, 7, 0));

        return ps;
    }

    void MathComp2Handler::HandleTask2() {
        std::vector<glm::vec3> ps{};
        ps.push_back(glm::vec3(0,0,0));
        ps.push_back(glm::vec3(1,2,0));
        ps.push_back(glm::vec3(2,5,0));
        ps.push_back(glm::vec3(3,6,0));
        points = ps;

        
        Eigen::MatrixXd A = Eigen::MatrixXd(4, 4);
        for (int i = 0; i < ps.size(); ++i) {
            A(i,0) = ps[i].x * ps[i].x * ps[i].x;
            A(i,1) = ps[i].x * ps[i].x;
            A(i,2) = ps[i].x;
            A(i,3) = 1.0;
        }

        std::cout << A <<  std::endl;

        Eigen::Vector4d y{};
        for (int i = 0; i < ps.size(); ++i) {
            y(i) = ps[i].y;
        }
        
        std::cout << y << std::endl;
        Eigen::Vector4d x = A.inverse() * y;
        std::cout << x << std::endl;

        mVertices.clear();
        float min = 0.f;
        float max = 3.f;
        int divisions = 10;
        float delta = (max - min)/divisions;
        
        for (float i = 0; i <= max+0.001f; i+=delta) {
            float y =
                x(0)*i*i*i +
                x(1)*i*i +
                x(2)*i +
                x(3);
            Vertex v = Vertex(glm::vec3(i, y,0), glm::vec3(0,0,0));
            mVertices.push_back(v);    
        }
        
    }

    void MathComp2Handler::init(GLint matrixUniform) {
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

    void MathComp2Handler::draw() {
        for (int i = 0; i < points.size(); ++i) {
            mModelMatrix = GetModelMatrix();

            glm::vec3 point = mModelMatrix * glm::vec4(points[i], 1);

            mModelMatrix = MathHelpers::TRS(point, glm::quat(1, 0, 0, 0), glm::vec3(0.1f));


            glBindVertexArray(mVAO);

            glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &mModelMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
        }
    }

    void MathComp2Handler::Update(float deltaTime) {
        VisualObject::Update(deltaTime);
    }
}
