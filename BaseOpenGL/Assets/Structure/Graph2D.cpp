#include "Graph2D.h"

#include <fstream>

namespace MM
{
    Graph2D::Graph2D(std::function<float(float)> f, int n, float from, float to)
    {
        mVertices.clear();
        float h = (to - from) / (float)n;
        for (float x = from; x < to; x += h)
        {
            float y = f(x);
            float col = y * 0.5f + 0.5f;
            Vertex v(x, y, 0, col,col,col);
            
            mVertices.push_back(v);
        }

        // debug values
        /*
        for (int i = 0; i < mVertices.size(); ++i)
        {
            std::cout << mVertices[i] << std::endl;
        }
        */

        mModelMatrix = glm::mat4x4(1.f);
    }

    Graph2D::Graph2D(std::vector<Vertex> verts)
    {
        mVertices = verts;
        mModelMatrix = glm::mat4x4(1.f);
    }

    void Graph2D::init(GLint matrixUniform)
    {
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

    void Graph2D::draw()
    {
        mModelMatrix = GetModelMatrix();
        glBindVertexArray(mVAO);
        glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &mModelMatrix[0][0]);
        glDrawArrays(GL_LINE_LOOP, 0, mVertices.size());
    }
    
    void Graph2D::readFile(std::string fileName) {
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

    void Graph2D::toFile(std::string fileName) {
        std::fstream o;
        o.open(fileName.c_str(), std::ios::out);
        o << mVertices.size() << std::endl;
        for (int i = 0; i < mVertices.size(); ++i) {
            o << mVertices[i] << std::endl;
        }
        o.close();
    }
}
