#include "pch.h"
#include "TriangleSurface.h"

#include <fstream>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>

namespace KT {
    TriangleSurface::TriangleSurface() {
        //                x    y     z   r g b
        Vertex v0{0.0, 0.0, 0.0, 1, 0, 0};
        mVertices.push_back(v0);
        Vertex v1(0.5, 0.0, 0.0, 0, 1, 0);
        mVertices.push_back(v1);
        Vertex v2{0.5, 0.5, 0.0, 0, 0, 1};
        mVertices.push_back(v2);
        Vertex v3{0.0, 0.0, 0.0, 0, 0, 1};
        mVertices.push_back(v3);
        Vertex v4{0.5, 0.5, 0.0, 0, 1, 0};
        mVertices.push_back(v4);
        Vertex v5{0.0, 0.5, 0.0, 1, 0, 0};
        mVertices.push_back(v5);

        mModelMatrix = glm::mat4x4(1.f);
    }

    TriangleSurface::TriangleSurface(std::string fileName) {
        readFile(fileName);
        mModelMatrix = glm::mat4x4(1.f);
    }

    TriangleSurface::~TriangleSurface() {
    }

    void TriangleSurface::readFile(std::string fileName) {
        std::ifstream inn;
        inn.open(fileName.c_str());
        mVertices.clear();
        if (inn.is_open()) {
            int n;
            Vertex vertex;
            inn >> n;
            mVertices.reserve(n);
            for (int i = 0; i < n; ++i) {
                inn >> vertex;
                mVertices.push_back(vertex);
                // std::cout << vertex << std::endl;
            }
        }
    }

    void TriangleSurface::toFile(std::string fileName) {
        std::fstream o;
        o.open(fileName.c_str(), std::ios::out);
        o << mVertices.size() << std::endl;
        for (int i = 0; i < mVertices.size(); ++i) {
            o << mVertices[i] << std::endl;
        }
        o.close();
    }

    void TriangleSurface::init(GLint matrixUniform) {
        mMatrixUniform = matrixUniform;

        // Create and bind vertex arrays, the object that will hold the Vertex Buffer Object
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);

        // Create, bind and populate Vertex Buffer Object,
        glGenBuffers(1, &mVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

        // tells the GPU shader program the structure and size of the data we pass
        // XYZ
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // RGB / NORMAL >:)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // Good practice to unbind vertex arrays
        glBindVertexArray(0);
    }

    void TriangleSurface::draw() {
        mModelMatrix = GetModelMatrix();

        glBindVertexArray(mVAO);
        glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &mModelMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
        
    }


    void TriangleSurface::construct() {

        glm::mat<4,4, glm::f32, glm::defaultp> a;
        
        mVertices.clear();
        const float k_pi = glm::pi<float>();
        const float e = glm::e<float>();
        auto aaa = [k_pi](float x, float y)
        {
            return sin(x * k_pi) * sin(y * k_pi);
        };

        auto franke = [e](float x, float y)
        {
            float f1 = -(pow(9.f * x - 2.f, 2.f) + pow(9.f * y - 2.f, 2.f)) / 4.f;
            float f2 = -(pow(9.f * x + 1.f, 2.f) / 49.f + (9.f * y + 1.f) / 10.f);
            float f3 = -(pow(9.f * x - 7.f, 2.f) + pow(9.f * y - 3.f, 2.f)) / 4.f;
            float f4 = -(pow(9.f * x - 4.f, 2.f) + pow(9.f * y - 7.f, 2.f));
            return 3.f / 4.f * pow(e, f1) +
                3.f / 4.f * pow(e, f2) +
                0.5f * pow(e, f3) -
                1.f / 5.f * pow(e, f4);
        };

        // auto frankeDerived = [franke](float x, float y)
        // {
        // float diff = 0.05f;
        // float x1 = franke(x);
        // };

        auto myFunc = [](float x, float y)
        {
            return x * x * y;
        };

        auto funcX = [](float x, float y)
        {
            return 2.f * x * y;
        };

        auto funcY = [](float x, float y)
        {
            return x * x;
        };

        auto myFuncNormal = [funcX, funcY](float x, float y)
        {
            float xx = funcX(x, y);
            float yy = funcY(x, y);

            // new Anders amazing way, see proof in camera roll
            return glm::normalize(glm::vec3(-funcX(x, y),  1,-funcY(x, y)));
            // old stinky stinky method >:(
            return normalize(glm::cross(glm::vec3(1, 0, xx), glm::vec3(0, 1, yy)));
        };

        float xmin = 0.f, xmax = 1.0f, zmin = 0.f, zmax = 1.0f;
        float h = 1.f / (2 << 2);
        for (float x = xmin; x < xmax; x += h)
            for (float z = zmin; z < zmax; z += h) {
                float y;
                glm::vec3 n;
                y = myFunc(x, z + h);
                n = myFuncNormal(x, z + h);
                mVertices.push_back(Vertex{x, y, z+h, n.x, n.y, n.z});

                y = myFunc(x + h, z);
                n = myFuncNormal(x + h, z);
                mVertices.push_back(Vertex{x + h, y, z, n.x, n.y, n.z});

                y = myFunc(x, z);
                n = myFuncNormal(x, z);
                mVertices.push_back(Vertex{x, y, z, n.x, n.y, n.z});


                // second triangle
                // ----------------------------------------
                y = myFunc(x + h, z + h);
                n = myFuncNormal(x + h, z + h);
                mVertices.push_back(Vertex{x + h, y, z + h, n.x, n.y, n.z});

                y = myFunc(x + h, z);
                n = myFuncNormal(x + h, z);
                mVertices.push_back(Vertex{x + h, y, z, n.x, n.y, n.z});
                
                y = myFunc(x, z + h);
                n = myFuncNormal(x, z + h);
                mVertices.push_back(Vertex{x, y, z+h, n.x, n.y, n.z});

            }
    }

    void TriangleSurface::constructWithLambda(std::function<float(float, float)> func) {

        
        auto FindNormal = [](float x, float z, std::function<float(float,float)> funcc)
        {
            const float step = 0.01f;
            float xx = funcc(x+step, z) - funcc(x-step,z);
            xx /= (step*2.f);
            float yy = funcc(x, z + step) - funcc(x, z-step);
            yy /= (step*2.f);

            return glm::normalize(glm::vec3(-xx,  1,-yy));
        };
        mVertices.clear();
        float xmin = -1.f, xmax = 1.0f, zmin = -1.f, zmax = 1.0f;
        float h = 1.f / (2 << 4);
        for (float x = xmin; x < xmax; x += h)
            for (float z = zmin; z < zmax; z += h) {
                float y;
                glm::vec3 n = glm::vec3(1,1,1);
                y = func(x, z + h);
                n = FindNormal(x, z + h,func);
                mVertices.push_back(Vertex{x, y, z+h, n.x, n.y, n.z});

                y = func(x + h, z);
                n = FindNormal(x + h, z,func);
                mVertices.push_back(Vertex{x + h, y, z, n.x, n.y, n.z});

                y = func(x, z);
                n = FindNormal(x, z,func);
                mVertices.push_back(Vertex{x, y, z, n.x, n.y, n.z});


                // second triangle
                // ----------------------------------------
                y = func(x + h, z + h);
                n = FindNormal(x + h, z + h,func);
                mVertices.push_back(Vertex{x + h, y, z + h, n.x, n.y, n.z});

                y = func(x + h, z);
                n = FindNormal(x + h, z,func);
                mVertices.push_back(Vertex{x + h, y, z, n.x, n.y, n.z});
                
                y = func(x, z + h);
                n = FindNormal(x, z + h,func);
                mVertices.push_back(Vertex{x, y, z+h, n.x, n.y, n.z});

            }
    }
}
