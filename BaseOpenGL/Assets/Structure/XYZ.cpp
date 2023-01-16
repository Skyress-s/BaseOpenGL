#include "XYZ.h"

namespace MM {
    XYZ::XYZ() {
        mVertices.push_back(Vertex{0, 0, 0, 1, 0, 0});
        mVertices.push_back(Vertex{1, 0, 0, 1, 0, 0});
        mVertices.push_back(Vertex{0, 0, 0, 0, 1, 0});
        mVertices.push_back(Vertex{0, 1, 0, 0, 1, 0});
        mVertices.push_back(Vertex{0, 0, 0, 0, 0, 1});
        mVertices.push_back(Vertex{0, 0, 1, 0, 0, 1});
        mMatrix = glm::mat4x4(1.f); // creates identity matrix
    }

    XYZ::~XYZ() {
    }

    void XYZ::init(GLint matrixUniform) {
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

        // RGB
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // Good practice to unbind vertex arrays
        glBindVertexArray(0);
    }

    void XYZ::draw() {
        glBindVertexArray(mVAO);
        glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE,&mMatrix[0][0]);
        glDrawArrays(GL_LINES, 0, mVertices.size());
    }
}
