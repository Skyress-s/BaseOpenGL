#pragma once

namespace KT {
    class GizmoDrawer {
    protected:
        std::vector<int> mIndices;
        std::vector<Vertex> mVertices;
        GLuint mVAO{0};
        GLuint mEBO{0};
        GLuint mVBO{0};

    public:
        void init(std::vector<int> indices, std::vector<Vertex> vertices) {
            mIndices = indices;
            mVertices = vertices;
            
            // vertex array
            glGenVertexArrays(1, &mVAO);
            glBindVertexArray(mVAO);

            // Vertex Buffer
            glGenBuffers(1, &mVBO);
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

            // alt 1: Bind Element Buffer
            glGenBuffers(1, &mEBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO); // binds EBO to VBO!
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vertex) * mIndices.size(), mIndices.data(),
                         GL_STATIC_DRAW); // sets data

            // attrib pointers
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        }

        
        void draw(GLenum drawMode, glm::mat4 modelMatrix, Shader shader) {
            glBindVertexArray(mVAO);

            shader.setMat4("matrix", modelMatrix);
            // glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &modelMatrix[0][0]);
            glDrawElements(drawMode, static_cast<unsigned int>(mIndices.size()), GL_UNSIGNED_INT, 0);
            
            glBindVertexArray(0);
        }
    };
}
