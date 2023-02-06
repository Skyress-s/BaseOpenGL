#pragma once
#include "../Axis/VisualObject.h"

namespace MM {
    class Disc : public VisualObject {
    public:
        GLuint mEBO;
        std::vector<unsigned int> mIndices;
        
        void draw() override;
        void init(GLint matrixUniform) override;

        void construct(int segments) {
            const float pi = glm::pi<float>();
            mVertices.clear();
            // central point
            mVertices.push_back(MM::Vertex(0, 0, 0, 0, 0, 0));
            
            for (int i = 0; i < segments; ++i) {
                float t = (float)i / ((float)segments - 1);
                float x = cos(t * pi * 2.f);
                float y = sin(t * pi * 2.f);
                glm::vec3 pos = glm::vec3(x, y, 0.f);

                mVertices.push_back(MM::Vertex(pos, pos));
            }
            
            // elements

            
            

            // Create and bind vertex arrays, the object that will hold the Vertex Buffer Object
            glGenVertexArrays(1, &mVAO);
            glBindVertexArray(mVAO);

            // alt 1: Bind Element Buffer
            glGenBuffers(1, &mEBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vertex) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);
            
            // alt 2: bind Vertex Buffer Object
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
    };
}
