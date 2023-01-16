#include "TriangleSurface.h"

#include <fstream>

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

        if (inn.is_open()) {
            int n;
            MM::Vertex vertex;
            inn >> n;
            mVertices.reserve(n);
            for (int i = 0; i < n; ++i) {
                inn >> vertex;
                mVertices.push_back(vertex);
            }
        }
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
}
