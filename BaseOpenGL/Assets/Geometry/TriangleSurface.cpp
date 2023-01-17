#include "TriangleSurface.h"

#include <fstream>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>

namespace MM {
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

        mMatrix = glm::mat4x4(1.f);
    }

    TriangleSurface::TriangleSurface(std::string fileName) {
        readFile(fileName);
        mMatrix = glm::mat4x4(1.f);
    }


    TriangleSurface::~TriangleSurface() {
    }

    void TriangleSurface::readFile(std::string fileName) {
        std::ifstream inn;
        inn.open(fileName.c_str());
        mVertices.clear();
        if (inn.is_open()) {
            int n;
            MM::Vertex vertex;
            inn >> n;
            mVertices.reserve(n);
            for (int i = 0; i < n; ++i) {
                inn >> vertex;
                mVertices.push_back(vertex);
                std::cout << vertex << std::endl;
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
        glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(MM::Vertex), mVertices.data(), GL_STATIC_DRAW);

        // tells the GPU shader program the structure and size of the data we pass
        // XYZ
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MM::Vertex), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // RGB
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MM::Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // Good practice to unbind vertex arrays
        glBindVertexArray(0);
    }

    void TriangleSurface::draw() {
        glBindVertexArray(mVAO);
        glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &mMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    }

    void TriangleSurface::construct() {
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
            float f2 = -(pow(9.f * x + 1.f, 2.f) / 49.f + (9.f * y + 1.f)/10.f); 
            float f3 = -(pow(9.f * x - 7.f, 2.f) + pow(9.f * y - 3.f, 2.f)) / 4.f;
            float f4 = -(pow(9.f * x - 4.f, 2.f) + pow(9.f * y - 7.f, 2.f));
            return 3.f / 4.f * pow(e,f1) +
                3.f / 4.f * pow(e,f2) +
                0.5f * pow(e,f3) -
                1.f / 5.f * pow(e,f4);
        };

        float xmin = -0.f, xmax = 1.0f, ymin = -0.f, ymax = 1.0f;
        float h = 1.f / (2 << 3);
        for (float x = xmin; x < xmax; x += h)
            for (float y = ymin; y < ymax; y += h) {
                float z = franke(x, y);
                mVertices.push_back(Vertex{x, y, z, x, y, z});
                
                z = franke(x + h, y);
                mVertices.push_back(Vertex{x + h, y, z, x, y, z});
                z = franke(x, y + h);
                mVertices.push_back(Vertex{x, y + h, z, x, y, z});


                mVertices.push_back(Vertex{x, y + h, z, x, y, z});
                z = franke(x + h, y);
                mVertices.push_back(Vertex{x + h, y, z, x, y, z});
                z = franke(x + h, y + h);
                mVertices.push_back(Vertex{x + h, y + h, z, x, y, z});
            }
    }
}
