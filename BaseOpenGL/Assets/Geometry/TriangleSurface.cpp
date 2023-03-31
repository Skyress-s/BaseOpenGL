#include "pch.h"
#include "TriangleSurface.h"

#include <fstream>
#include <thread>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <GLFW/glfw3native.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>

namespace KT {
    void process_chunk(std::vector<std::atomic<Vertex>>& data, int startIndex, int endIndex,
                       KTTexture2D texture, int numWidthVertices, int numHeightVertices) {
        for (int i = startIndex; i < endIndex; ++i) {
            float u = (float)(i % numWidthVertices) / (float)numWidthVertices;
            float v = (float)(i / numWidthVertices) / (float)numHeightVertices; // i / numWidthVertices used truncation

            float yavg = 0.f;
            glm::vec3 n = glm::vec3(0, 1, 0);
            glm::vec3 p = glm::vec3(0, 0, 0);
            const int offset = 5;
            float y1 = texture.ValueAt(u, v)[0];
            float y2 = texture.ValueAt(u, v, offset, 0)[0];
            float y3 = texture.ValueAt(u, v, -offset, 0)[0];
            float y4 = texture.ValueAt(u, v, 0, offset)[0];
            float y5 = texture.ValueAt(u, v, 0, -offset)[0];
            yavg = (y1 + y2 + y3 + y4 + y5) / 5.f;

            p.x = u; // * widthLength;
            p.z = v; // * heightLength;
            p.y = yavg;
            data[i] = Vertex{p.x, p.y, p.z, n.x, n.y, n.z, u, v};
            // mVertices.push_back(Vertex{p.x, p.y, p.z, n.x, n.y, n.z, u, v});
        }
        std::cout << "Thread " << std::this_thread::get_id() << " finished  " << "iterations : " << endIndex-startIndex << " of total : " << numWidthVertices * numHeightVertices << std::endl;
    }

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

    TriangleSurface::TriangleSurface(Shader* shader, unsigned int texture) {
        mShader = shader;
        mTextures.push_back(texture);
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
        VisualObject::init(matrixUniform);

        return;
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
        if (mShader == nullptr) {
            DrawElements(GL_TRIANGLES, GetModelMatrix());
            // glBindVertexArray(mVAO);
            // glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &mModelMatrix[0][0]);
            // glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
            return;
        }
        // Draw(GL_NONE, GetModelMatrix());
        DrawElementsWithShader(GL_TRIANGLES, GetModelMatrix());
    }


    void TriangleSurface::construct() {
        glm::mat<4, 4, glm::f32, glm::defaultp> a;

        mVertices.clear();
        const float k_pi = glm::pi<float>();
        const float e = glm::e<float>();
        auto aaa = [k_pi](float x, float y) {
            return sin(x * k_pi) * sin(y * k_pi);
        };

        auto franke = [e](float x, float y) {
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

        auto myFunc = [](float x, float y) {
            return x * x * y;
        };

        auto funcX = [](float x, float y) {
            return 2.f * x * y;
        };

        auto funcY = [](float x, float y) {
            return x * x;
        };

        auto myFuncNormal = [funcX, funcY](float x, float y) {
            float xx = funcX(x, y);
            float yy = funcY(x, y);

            // new Anders amazing way, see proof in camera roll
            return glm::normalize(glm::vec3(-funcX(x, y), 1, -funcY(x, y)));
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
                mVertices.push_back(Vertex{x, y, z + h, n.x, n.y, n.z});

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
                mVertices.push_back(Vertex{x, y, z + h, n.x, n.y, n.z});
            }
    }

    void TriangleSurface::constructWithLambda(std::function<float(float, float)> func) {
        auto FindNormal = [](float x, float z, std::function<float(float, float)> funcc) {
            const float step = 0.01f;
            float xx = funcc(x + step, z) - funcc(x - step, z);
            xx /= (step * 2.f);
            float yy = funcc(x, z + step) - funcc(x, z - step);
            yy /= (step * 2.f);

            return glm::normalize(glm::vec3(-xx, 1, -yy));
        };
        mVertices.clear();
        float xmin = -1.f, xmax = 1.0f, zmin = -1.f, zmax = 1.0f;
        float h = 1.f / (2 << 4);
        for (float x = xmin; x < xmax; x += h)
            for (float z = zmin; z < zmax; z += h) {
                float y;
                glm::vec3 n = glm::vec3(1, 1, 1);
                y = func(x, z + h);
                n = FindNormal(x, z + h, func);
                mVertices.push_back(Vertex{x, y, z + h, n.x, n.y, n.z});

                y = func(x + h, z);
                n = FindNormal(x + h, z, func);
                mVertices.push_back(Vertex{x + h, y, z, n.x, n.y, n.z});

                y = func(x, z);
                n = FindNormal(x, z, func);
                mVertices.push_back(Vertex{x, y, z, n.x, n.y, n.z});


                // second triangle
                // ----------------------------------------
                y = func(x + h, z + h);
                n = FindNormal(x + h, z + h, func);
                mVertices.push_back(Vertex{x + h, y, z + h, n.x, n.y, n.z});

                y = func(x + h, z);
                n = FindNormal(x + h, z, func);
                mVertices.push_back(Vertex{x + h, y, z, n.x, n.y, n.z});

                y = func(x, z + h);
                n = FindNormal(x, z + h, func);
                mVertices.push_back(Vertex{x, y, z + h, n.x, n.y, n.z});
            }
    }


    /**
     * \brief 
     * \param texture 
     */
    void TriangleSurface::constructWithTexture(KTTexture2D texture) {
        mVertices.clear();
        mIndices.clear();
        // zero to one
        // construct the vertices
        int numWidthVertices = 2 << 3; // xxxxx
        int numHeightVertices = 2 << 3; // yyyyy
        float widthLength = 1.f;
        float heightLength = 1.f;
        auto funcX = [numWidthVertices](int x, int z) {
            return (z * numWidthVertices + x);
        };
        std::vector<std::atomic<Vertex>> data = std::vector<std::atomic<Vertex>>(numHeightVertices * numWidthVertices);
        
        float time = glfwGetTime();
        
        int numThreads = 4;
        int chunckSize = numHeightVertices * numWidthVertices / numThreads;
        std::vector<std::thread> threads(numThreads);
        
        for (int i = 0; i < numThreads; ++i) {
            int startIndex = i * chunckSize;
            int endIndex = (i == numThreads - 1) ? numHeightVertices * numWidthVertices : (i + 1) * chunckSize;
            // last thread, make sure all data is processed
            threads[i] = std::thread(process_chunk, std::ref(data), startIndex, endIndex, texture,
                                     numWidthVertices, numHeightVertices);
            std::cout << "thread " << i << " " <<threads[i].get_id() << " start" << std::endl;
        }
        
        for (auto& thread : threads) {
        std::cout << "waiting for thread to finish " <<thread.get_id() << std::endl;
        thread.join();
        }
        mVertices = std::vector<Vertex>(data.begin(), data.end());
        // memcpy(mVertices.data(), data.data(), sizeof(Vertex) * numHeightVertices * numWidthVertices);
        
        // mVertices = std::vector<Vertex>(data.begin(), data.end());

        // for (int i = 0; i < numHeightVertices * numWidthVertices; ++i) {
        //     float u = (float)(i % numWidthVertices) / (float)numWidthVertices;
        //     float v = (float)(i / numWidthVertices) / (float)numHeightVertices; // i / numWidthVertices used truncation
        //
        //     float yavg = 0.f;
        //     glm::vec3 n = glm::vec3(0, 1, 0);
        //     glm::vec3 p = glm::vec3(0, 0, 0);
        //     const int offset = 5;
        //     float y1 = texture.ValueAt(u, v)[0];
        //     float y2 = texture.ValueAt(u, v, offset, 0)[0];
        //     float y3 = texture.ValueAt(u, v, -offset, 0)[0];
        //     float y4 = texture.ValueAt(u, v, 0, offset)[0];
        //     float y5 = texture.ValueAt(u, v, 0, -offset)[0];
        //     yavg = (y1 + y2 + y3 + y4 + y5) / 5.f;
        //
        //     p.x = u * widthLength;
        //     p.z = v * heightLength;
        //     p.y = yavg;
        //
        //     mVertices.push_back(Vertex{p.x, p.y, p.z, n.x, n.y, n.z, u, v});
        // }


        /*
        for (int z = 0; z < numHeightVertices; ++z) {
            for (int x = 0; x < numWidthVertices; ++x) {
                float yavg = 0.f;
                glm::vec3 n = glm::vec3(0, 1, 0);
                glm::vec3 p = glm::vec3(0, 0, 0);
                glm::vec2 uv = glm::vec2(0, 0);
                uv = glm::vec2(static_cast<float>(x) / ((float)numWidthVertices - 1.f),
                               static_cast<float>(z) / ((float)numHeightVertices - 1.f));
                const int offset = 5;
                float y1 = texture.ValueAt(uv.x, uv.y)[0];
                float y2 = texture.ValueAt(uv.x, uv.y, offset, 0)[0];
                float y3 = texture.ValueAt(uv.x, uv.y, -offset, 0)[0];
                float y4 = texture.ValueAt(uv.x, uv.y, 0, offset)[0];
                float y5 = texture.ValueAt(uv.x, uv.y, 0, -offset)[0];
                yavg = (y1 + y2 + y3 + y4 + y5) / 5.f;


                p = glm::vec3(uv.x * widthLength, yavg, uv.y * heightLength);
                mVertices.push_back(Vertex(p.x, p.y, p.z, n.x, n.y, n.z, uv.x, uv.y));
            }
        }
        */
        std::cout << "Time used for constructing vertices : " << glfwGetTime() - time << "s" << std::endl;
        // loop through all execpt the last row and column
        for (int z = 0; z < numHeightVertices - 1; ++z) {
            for (int x = 0; x < numWidthVertices - 1; ++x) {
                // first triangle
                mIndices.push_back(funcX(x, z));
                mIndices.push_back(funcX(x, z + 1));
                mIndices.push_back(funcX(x + 1, z));

                // second triangle
                mIndices.push_back(funcX(x + 1, z + 1));
                mIndices.push_back(funcX(x + 1, z));
                mIndices.push_back(funcX(x, z + 1));
            }
        }

        // GLint maxIndices;
        // glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &maxIndices);
        // std::cout << "MAX ELEMENTS : " <<  maxIndices << " CURRENT : " << mIndices.size() << " VALID ? " <<
        //     (maxIndices > mIndices.size()) << std::endl;

        //reduce height
        for (auto& v : mVertices) {
            v.m_xyz[1] *= 1.f / 1900.f;
        }

        return;
        /*
        float xmin = 0.f, xmax = 1.0f, zmin = 0.f, zmax = 1.0f;

        float h = 1.f / (2 << 4);
        h = 1.f / 35;
        for (float x = xmin; x < xmax - h / 2.; x += h)
            for (float z = zmin; z < zmax - h / 2.; z += h) {
                float y;

                glm::vec3 n = glm::vec3(1, 1, 1);

                // vertices
                y = texture.ValueAt(x / xmax, (z) / zmax)[0];
                mVertices.push_back(Vertex{x, y, z, n.x, n.y, n.z, x / xmax, z / zmax});
                mIndices.push_back(mVertices.size() - 1);


                y = texture.ValueAt((x) / xmax, (z + h) / zmax)[0];
                mVertices.push_back(Vertex{x, y, z + h, n.x, n.y, n.z, x / xmax, (z + h) / zmax});
                mIndices.push_back(mVertices.size() - 1);

                y = texture.ValueAt((x + h) / xmax, (z) / zmax)[0];
                mVertices.push_back(Vertex{x + h, y, z, n.x, n.y, n.z, (x + h) / xmax, z / zmax});
                mIndices.push_back(mVertices.size() - 1);


                y = texture.ValueAt((x + h) / xmax, (z + h) / zmax)[0];
                mVertices.push_back(Vertex{x + h, y, z + h, n.x, n.y, n.z, (x + h) / xmax, (z + h) / zmax});
                mIndices.push_back(mVertices.size() - 1);
                mIndices.push_back(mVertices.size() - 1 - 1);
                mIndices.push_back(mVertices.size() - 1 - 2);

                // indices
            }

        // reduce height
        for (int i = 0; i < mVertices.size(); ++i) {
            mVertices[i].m_xyz[1] /= 950.f;
        }
            */
    }

}
