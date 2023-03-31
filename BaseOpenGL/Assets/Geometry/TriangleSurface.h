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
                    int neighbourTriIndex = FindNeighbourTriangleRaw(indices[(1 + j) % 3], indices[(2 + j) % 3], i);
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
            glm::vec3 baryc = BarycentricCoordinatesXZ(positions[0], positions[1], positions[2],
                                                       x);
            std::cout << "baryc : " << baryc.x << ", " << baryc.y << ", " << baryc.z << std::endl;
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

        int FindNeigbourTriangle(const int& currentTriangle, const int& edge1, const int& edge2) {
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

        glm::vec3 SearchCurrentAndNearest(unsigned int& outCurrentTriangle, const glm::vec3& x) {
            // search current
            glm::vec3 pos[3];
            int triIndices[3];
            GetTrianglePositions(outCurrentTriangle, pos);
            GetTriangleIndices(outCurrentTriangle, triIndices);
            glm::vec3 baryc = BarycentricCoordinatesXZ(pos[0], pos[1], pos[2], x);
            
            if (InTriangle(outCurrentTriangle, x)) {
                // in bounds
                glm::vec3 calcpos = pos[0] * baryc.x + pos[1] * baryc.y + pos[2] * baryc.z;
                return calcpos;
            }
            // search nearest neighbour
            
            // find nearest
            int edge[2];
            if (baryc.x < baryc.y && baryc.x < baryc.z) {
                edge[0] = 1;
                edge[1] = 2;
                
            }
            if (baryc.y < baryc.z && baryc.y < baryc.x) {
                edge[0] = 0;
                edge[1] = 2;
            }
            if (baryc.z < baryc.y && baryc.z < baryc.x) {
                edge[0] = 0;
                edge[1] = 1;
            }

            int neighbourTri = FindNeigbourTriangle(outCurrentTriangle, edge[0], edge[1]);
            if (neighbourTri != -1 && InTriangle(neighbourTri, x)) {
                GetTrianglePositions(neighbourTri, pos);
                // baryc = BarycentricCoordinates3d(pos.begin()._Ptr, _objectToMove->GetPosition());
                baryc = BarycentricCoordinatesXZ(pos[0], pos[1], pos[2], x);
                outCurrentTriangle = neighbourTri;
                return baryc.x * pos[0] + baryc.y * pos[1] + baryc.z * pos[2];
            }

            return x;
        }
    private:
        /*
        std::vector<int> neighbour_triangle_indices(int index) {
            std::vector<int> retVal{};
            const unsigned int numTriangles = mIndices.size() / 3;
        }
    */
    };
}
