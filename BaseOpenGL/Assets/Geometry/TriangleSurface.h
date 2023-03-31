#pragma once
#include <functional>

#include "../Axis/VisualObject.h"
#include "../Math/TriangulationHandler.h"

namespace KT {
    class TriangleSurface : public VisualObject {
    private:
        std::vector<tri_data> mTriDatas_{};

    public:
        TriangleSurface();
        TriangleSurface(Shader* shader, unsigned int texture);
        TriangleSurface(std::string fileName);
        ~TriangleSurface();

        void readFile(std::string fileName);
        void toFile(std::string fileName);
        void init(GLint matrixUniform) override;
        void draw() override;
        void construct();
        void constructWithLambda(std::function<float(float, float)> func);
        void constructWithTexture(KTTexture2D texture);

        void SetupTriData() {
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
                    int neighbourTriIndex = FindNeighbourTriangleRaw(indices[(0 + j) % 3], indices[(1 + j) % 3], i);
                    if (neighbourTriIndex != -1) {
                        tri_data.neighbour_triangles[j] = neighbourTriIndex;
                    }
                }
                mTriDatas_.push_back(tri_data);
            }
            /*
            // test that it works
            tri_data tri_data1 = mTriDatas_[45];
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


        // void GetTrianglePositions(const int& i, vector<glm::vec3>& outPositions) {
        void GetTrianglePositions(const int& i, glm::vec3 outPositions[3]) {
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

        void GetTriangleIndices(const int& i, int indices[3]) {
            indices[0] = mIndices[i * 3 + 0];
            indices[1] = mIndices[i * 3 + 1];
            indices[2] = mIndices[i * 3 + 2];
        }


        bool InTriangle(const int& i, const glm::vec3& x) {
            glm::vec3 positions[3];
            GetTrianglePositions(i, positions);
            glm::vec3 baryc = BarycentricCoordinates3d(positions[0], positions[1], positions[2],
                                                       x);
            if (baryc.x < 0 || baryc.y < 0 || baryc.z < 0)
                return false;
            return true;
        }

        
        int FindNeighbourTriangleRaw(const int& vertexIndexA, const int& vertexIndexB, const int& ignoreTriIndex) {
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

    private:
        std::vector<int> neighbour_triangle_indices(int index) {
            std::vector<int> retVal{};
            const unsigned int numTriangles = mIndices.size() / 3;
        }
    };
}
