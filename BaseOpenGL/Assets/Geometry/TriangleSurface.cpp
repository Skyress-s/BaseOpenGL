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
            float u = (float)(i % numWidthVertices) / ((float)numWidthVertices - 1);
            float v = (float)(i / numWidthVertices) / ((float)numHeightVertices - 1); // i / numWidthVertices used truncation

            float yavg = 0.f;
            glm::vec3 n = glm::vec3(0, 1, 0);
            glm::vec3 p = glm::vec3(0, 0, 0);
            const int offset = 5;
            // float y1 = texture.ValueAt(u, v)[0];
            // float y2 = texture.ValueAt(u, v, offset, 0)[0];
            // float y3 = texture.ValueAt(u, v, -offset, 0)[0];
            // float y4 = texture.ValueAt(u, v, 0, offset)[0];
            // float y5 = texture.ValueAt(u, v, 0, -offset)[0];
            // yavg = (y1 + y2 + y3 + y4 + y5) / 5.f;
            yavg = texture.ValueAt(u, v)[0];           

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


    // Exam task 3
    /**
     * \brief Using multithreading to construct the vertices
     * \param texture 
     */
    void TriangleSurface::constructWithTexture(KTTexture2D texture) {
        mVertices.clear();
        mIndices.clear();
        // zero to one
        // construct the vertices
        int numWidthVertices = 2 << 7;      // xxxxx 
        int numHeightVertices = 2 << 7;     // yyyyy
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
            v.m_xyz[1] *= 1.f / 5900.f;
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

    std::vector<unsigned> TriangleSurface::FindCommonElements(const std::vector<unsigned>& list1,
        const std::vector<unsigned>& list2) {
        std::vector<unsigned int> retval{};
        for (int i = 0; i < list1.size(); ++i)
            for (int j = 0; j < list2.size(); ++j)
                if (list1[i] == list2[j])
                    retval.push_back(list1[i]);
        // most readable sort ever
        return retval;
    }

    void TriangleSurface::SetupTriData() {
        // make an twodimensional vector, where the first layer is the length mVertices.size(),
        // and the second layer, triangle indexes to triangles that have that mVertices as a vertex
        vertexTriangleIndexes = std::vector<std::vector<unsigned int>>(
            mVertices.size(), std::vector<unsigned int>());
        for (int i = 0; i < mIndices.size() / 3; i++) {
            // loop through all triangles
            int vertexIndexes[3];
            GetTriangleIndices(i, vertexIndexes);
            for (int j = 0; j < 3; ++j) {
                // if (vertexIndexes[j] == -1) // dont need this, all vertices are valid
                //     continue;
                vertexTriangleIndexes[vertexIndexes[j]].push_back(i);
            }
        }

        // go through each triangle and find neighbours
        for (int i = 0; i < mIndices.size() / 3; ++i) {
            // loop through all triangles
            tri_data triData{};
            triData.vertex_indices[0] = mIndices[i * 3];
            triData.vertex_indices[1] = mIndices[i * 3 + 1];
            triData.vertex_indices[2] = mIndices[i * 3 + 2];

            // neighbours
            // -----------------------------------------------------------------------------------------------------------------

            // neighbour 2
            vector<unsigned int> trianglesWithThisVertex1 = vertexTriangleIndexes[triData.vertex_indices[0]];
            vector<unsigned int> trianglesWithThisVertex2 = vertexTriangleIndexes[triData.vertex_indices[1]];
            vector<unsigned int> result = FindCommonElements(trianglesWithThisVertex1, trianglesWithThisVertex2);

            for (int j = 0; j < result.size(); ++j) {
                if (result[j] == i) // if match is current triangle, skip
                    continue;
                triData.neighbour_triangles[2] = result[j];
            }

            // neighbour 1
            trianglesWithThisVertex1 = vertexTriangleIndexes[triData.vertex_indices[1]];
            trianglesWithThisVertex2 = vertexTriangleIndexes[triData.vertex_indices[2]];
            result = FindCommonElements(trianglesWithThisVertex1, trianglesWithThisVertex2);

            for (int j = 0; j < result.size(); ++j) {
                if (result[j] == i) // if match is current triangle, skip
                    continue;
                triData.neighbour_triangles[0] = result[j];
            }

            // neighbour 0
            trianglesWithThisVertex1 = vertexTriangleIndexes[triData.vertex_indices[2]];
            trianglesWithThisVertex2 = vertexTriangleIndexes[triData.vertex_indices[0]];
            result = FindCommonElements(trianglesWithThisVertex1, trianglesWithThisVertex2);

            for (int j = 0; j < result.size(); ++j) {
                if (result[j] == i) // if match is current triangle, skip
                    continue;
                triData.neighbour_triangles[1] = result[j];
            }

            mTriDatas_.push_back(triData);
        }

        /*
            // return;
            const unsigned int numTriangles = mIndices.size() / 3;

            for (int i = 0; i < numTriangles; ++i) {
                tri_data tri_data{};
                tri_data.vertex_indices[0] = mIndices[i * 3];
                tri_data.vertex_indices[1] = mIndices[i * 3 + 1];
                tri_data.vertex_indices[2] = mIndices[i * 3 + 2];

                // neighbours
                int indices[3];
                GetTriangleIndices(i, indices);

                for (int j = 0; j < 3; ++j) {
                    // a tridata can only have three adjacent neighbours
                    int neighbourTriIndex = FindNeighbourTriangleRaw(indices[(1 + j) % 3], indices[(2 + j) % 3], i);
                    if (neighbourTriIndex != -1) {
                        tri_data.neighbour_triangles[j] = neighbourTriIndex;
                    }
                }
                mTriDatas_.push_back(tri_data);
            }
            */

        /*
            // test that it works
            tri_data tri_data1 = mTriDatas_[69];
            for (int i = 0; i < 3; ++i) {
                mVertices[tri_data1.vertex_indices[i]].m_xyz[1] = 0.5f;
            }
            for (int j = 0; j < 3; ++j) {
                int tri2 = tri_data1.neighbour_triangles[j];
                int indices[3];
                GetTriangleIndices(tri2, indices);
                for (int i = 0; i < 3; ++i) {
                    mVertices[indices[i]].m_xyz[1] = 0.5f;
                }
            }
        */
    }

    void TriangleSurface::FindNeighbourTriangles(const int& currentTriangleIndex, int triIndexes[3]) {
        triIndexes[0] = triIndexes[1] = triIndexes[2] = -1;
        for (int i = 0; i < 3; ++i) {
            triIndexes[i] = mTriDatas_[currentTriangleIndex].neighbour_triangles[i];
        }
    }

    // Exam task 10
    void TriangleSurface::CalculateNormals() {
        std::vector<glm::vec3> retVal = std::vector<glm::vec3>();

        
        for (int vertex = 0; vertex < mVertices.size(); ++vertex) {

            // find triangles with this vertex
            std::vector<unsigned int> triangleIndices = GeometryHelpers::GetTrianglesWithVertex(
                vertexTriangleIndexes, vertex);

            // calcualate the weighted triangle and add to normal n vector
            glm::vec3 n = glm::vec3(0.f);
            for (int i = 0; i < triangleIndices.size(); ++i) {
                n += GeometryHelpers::calculate_normal_of_triangle_weighted(
                    triangleIndices[i], mVertices, mIndices);
            }

            // normalize and set normal
            n = glm::normalize(n);
            mVertices[vertex].set_normal(n);
        }

        // old version
        /*
        for (int i = 0; i < mVertices.size(); ++i) {
            int triIndexes[3];
            FindNeighbourTriangles(i, triIndexes);
            glm::vec3 normal = glm::vec3(0.f);
            for (int j = 0; j < 3; ++j) {
                if (triIndexes[j] == -1) {
                    continue;
                }
                normal += GeometryHelpers::calculate_normal_of_triangle_weighted(
                    triIndexes[j], mVertices, mIndices);
                // std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
            }
            normal = glm::normalize(normal);
            mVertices[i].m_normal[0] = normal.x;
            mVertices[i].m_normal[1] = normal.y;
            mVertices[i].m_normal[2] = normal.z;
        }
        */
    }

    void TriangleSurface::GetTrianglePositions(const int& i, glm::vec3 outPositions[3]) const {
        glm::vec3 p1, p2, p3;
        p1 = mVertices[mIndices[i * 3 + 0]].posToVec3();
        p2 = mVertices[mIndices[i * 3 + 1]].posToVec3();
        p3 = mVertices[mIndices[i * 3 + 2]].posToVec3();
        // outPositions.clear();
        // outPositions.push_back(p1);
        // outPositions.push_back(p2);
        // outPositions.push_back(p3);
        outPositions[0] = p1;
        outPositions[1] = p2;
        outPositions[2] = p3;
    }

    void TriangleSurface::GetTriangleIndices(const int& i, int indices[3]) {
        indices[0] = mIndices[i * 3 + 0];
        indices[1] = mIndices[i * 3 + 1];
        indices[2] = mIndices[i * 3 + 2];
    }

    bool TriangleSurface::InTriangleXZ(const int& i, const glm::vec3& x) const {
        glm::vec3 positions[3];
        GetTrianglePositions(i, positions);
        glm::vec3 baryc = BarycentricCoordinatesXZ(positions[0], positions[1], positions[2],
                                                   x);
        if (baryc.x < 0 || baryc.y < 0 || baryc.z < 0)
            return false;
        return true;
    }

    int TriangleSurface::FindNeighbourTriangleRaw(const int& vertexIndexA, const int& vertexIndexB,
        const int& ignoreTriIndex) {
        int indices[3];
        for (int i = 0; i < mIndices.size() / 3; ++i) {
            if (i == ignoreTriIndex)
                continue;
            GetTriangleIndices(i, indices);

            auto start = mIndices.begin();
            auto end = mIndices.begin();
            std::advance(start, i * 3);
            std::advance(end, i * 3);
            std::advance(end, 3); // advance two i last, move one further becouse of the != end check further down
            // std::cout << "distance : " << std::distance(start, end) << std::endl;
            vector<int>::iterator x = std::find(start, end, vertexIndexA);
            vector<int>::iterator y = std::find(start, end, vertexIndexB);

            if (x != end && y != end) {
                return i;
            }
        }
        return -1;
    }

    int TriangleSurface::FindNeigbourTriangle(const int& currentTriangle, const int& edge1, const int& edge2) {
        const int sum = edge1 + edge2;
        switch (sum) {
        case 1:
            return mTriDatas_[currentTriangle].neighbour_triangles[2];
        case 2:
            return mTriDatas_[currentTriangle].neighbour_triangles[1];
        case 3:
            return mTriDatas_[currentTriangle].neighbour_triangles[0];
        default:
            return -1;
        }
    }

    glm::vec3 TriangleSurface::SearchCurrentAndNearest(unsigned& outCurrentTriangle, const glm::vec3& x) {
        // search current
        glm::vec3 pos[3];
        int triIndices[3];
        GetTrianglePositions(outCurrentTriangle, pos);
        GetTriangleIndices(outCurrentTriangle, triIndices);
        glm::vec3 baryc = BarycentricCoordinatesXZ(pos[0], pos[1], pos[2], x);

        if (InTriangleXZ(outCurrentTriangle, x)) {
            // in bounds
            glm::vec3 calcpos = pos[0] * baryc.x + pos[1] * baryc.y + pos[2] * baryc.z;
            return calcpos;
        }
        // search nearest neighbour
        // ----------------------------------------

        // find nearest edge
        int edgeIndices[2];
        if (baryc.x < baryc.y && baryc.x < baryc.z) {
            edgeIndices[0] = 1;
            edgeIndices[1] = 2;
        }
        if (baryc.y < baryc.z && baryc.y < baryc.x) {
            edgeIndices[0] = 0;
            edgeIndices[1] = 2;
        }
        if (baryc.z < baryc.y && baryc.z < baryc.x) {
            edgeIndices[0] = 0;
            edgeIndices[1] = 1;
        }
        
        int neighbourTri = FindNeigbourTriangle(outCurrentTriangle, edgeIndices[0], edgeIndices[1]);
        if (neighbourTri != -1 && InTriangleXZ(neighbourTri, x)) {
            GetTrianglePositions(neighbourTri, pos);
            // baryc = BarycentricCoordinates3d(pos.begin()._Ptr, _objectToMove->GetPosition());
            baryc = BarycentricCoordinatesXZ(pos[0], pos[1], pos[2], x);
            outCurrentTriangle = neighbourTri;
            return baryc.x * pos[0] + baryc.y * pos[1] + baryc.z * pos[2];
        }
        if (neighbourTri != -1) { // did not find triangle
            // not in triangle but found a neighbour
            outCurrentTriangle = neighbourTri;
            return SearchCurrentAndNearest(outCurrentTriangle, x); // recursivley seach until we reach a valid triangle
        }

        
        tri_data triData = mTriDatas_[outCurrentTriangle];
        glm::vec3 p1 = mVertices[triData.vertex_indices[edgeIndices[0]]].posToVec3();
        glm::vec3 p2 = mVertices[triData.vertex_indices[edgeIndices[1]]].posToVec3();
        const float ratio = baryc[edgeIndices[1]] + (baryc[edgeIndices[0]]);
        float a, b;
        a = baryc[edgeIndices[0]] / ratio;
        b = baryc[edgeIndices[1]] / ratio;
        return p1 * baryc[edgeIndices[0]] / ratio + p2 * baryc[edgeIndices[1]] / ratio;
    }

    glm::vec3 TriangleSurface::FindPointOnSurfaceXZ(const glm::vec3& point) const {
        for (int i = 0; i < mIndices.size() / 3; ++i) {
            if (InTriangleXZ(i, point)) {
                glm::vec3 positions[3];
                GetTrianglePositions(i, positions);
                glm::vec3 baryc = BarycentricCoordinatesXZ(positions[0], positions[1], positions[2], point);
                return baryc.x * positions[0] + baryc.y * positions[1] + baryc.z * positions[2];
            }
        }
        std::cout << "FindPointOnSurfaceXZ: point not in mesh (XZ plane)" << std::endl;
        return point;
    }
}
