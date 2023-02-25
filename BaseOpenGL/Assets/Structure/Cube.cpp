#include "pch.h"
#include "Cube.h"

namespace KT {
    Cube::Cube() {
        // front
        mVertices.push_back(Vertex(-1,-1,1    ,1,1,1));
        mVertices.push_back(Vertex(1,-1,1    ,1,1,1));
        mVertices.push_back(Vertex(1,1,1    ,1,1,1));
        
        mVertices.push_back(Vertex(1,1,1    ,1,1,1));
        mVertices.push_back(Vertex(-1,1,1    ,1,1,1));
        mVertices.push_back(Vertex(-1,-1,1    ,1,1,1));

        // bottom
        mVertices.push_back(Vertex(1,-1,-1    ,1,1,1));
        mVertices.push_back(Vertex(1,-1,1    ,1,1,1));
        mVertices.push_back(Vertex(-1,-1,1    ,1,1,1));
        
        mVertices.push_back(Vertex(-1,-1,1    ,1,1,1));
        mVertices.push_back(Vertex(-1,-1,-1    ,1,1,1));
        mVertices.push_back(Vertex(1,-1,-1    ,1,1,1));

        // side R
        Vertex s1 = Vertex(1, -1,-1     , 1,1,1);
        Vertex s2 = Vertex(1, 1,-1     , 1,1,1);
        Vertex s3 = Vertex(1, 1,1     , 1,1,1);
        Vertex s4 = Vertex(1, -1,1     , 1,1,1);

        mVertices.push_back(s1);
        mVertices.push_back(s2);
        mVertices.push_back(s3);

        mVertices.push_back(s3);
        mVertices.push_back(s4);
        mVertices.push_back(s1);

        // size L
        s1 = Vertex(-1, -1,-1     , 1,1,1);
        s2 = Vertex(-1, 1,-1     , 1,1,1);
        s3 = Vertex(-1, 1,1     , 1,1,1);
        s4 = Vertex(-1, -1,1     , 1,1,1);

        mVertices.push_back(s3);
        mVertices.push_back(s2);
        mVertices.push_back(s1);

        mVertices.push_back(s1);
        mVertices.push_back(s4);
        mVertices.push_back(s3);

        // back
        mVertices.push_back(Vertex(1,1,-1    ,1,1,1));
        mVertices.push_back(Vertex(1,-1,-1    ,1,1,1));
        mVertices.push_back(Vertex(-1,-1,-1    ,1,1,1));
        
        mVertices.push_back(Vertex(-1,-1,-1    ,1,1,1));
        mVertices.push_back(Vertex(-1,1,-1    ,1,1,1));
        mVertices.push_back(Vertex(1,1,-1    ,1,1,1));


        // top
        mVertices.push_back(Vertex(-1,1,1    ,1,1,1));
        mVertices.push_back(Vertex(1,1,1    ,1,1,1));
        mVertices.push_back(Vertex(1,1,-1    ,1,1,1));
        
        mVertices.push_back(Vertex(1,1,-1    ,1,1,1));
        mVertices.push_back(Vertex(-1,1,-1    ,1,1,1));
        mVertices.push_back(Vertex(-1,1,1    ,1,1,1));
        
        mModelMatrix = glm::mat4x4(1.f); // creates identity matrix
    }

    void Cube::init(GLint matrixUniform) {
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

    void Cube::draw() {
        mModelMatrix = GetModelMatrix();
        
        glBindVertexArray(mVAO);
        glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &mModelMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());


        // mMatrix = glm::rotate(mMatrix, 0.0005f, glm::vec3(0.2,1,0));
    }
}
