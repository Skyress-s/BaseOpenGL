#pragma once
#include <algorithm>

#include "MathHelpers.h"
#include "../IO/FileHandler.h"

namespace KT {
    struct tri_data {
    public:
        int vertex_indices[3]{};
        int neighbour_triangles[3]{};


        // tri_data() {
        //     vertex_indices[0] = vertex_indices[1] = vertex_indices[2] = -1;
        //     neighbour_triangles[0] = neighbour_triangles[1] = neighbour_triangles[2] = -1;
        // }

        tri_data(int vertex_indices[3], int neighbour_triangles[3]) {
            // std::memcpy(this->vertex_indices, vertex_indices, sizeof(int) * 3);
            // std::memcpy(this->neighbour_triangles, neighbour_triangles, sizeof(int) * 3);
            this->vertex_indices[0] = vertex_indices[0];
            this->vertex_indices[1] = vertex_indices[1];
            this->vertex_indices[2] = vertex_indices[2];
            this->neighbour_triangles[0] = neighbour_triangles[0];
            this->neighbour_triangles[1] = neighbour_triangles[1];
            this->neighbour_triangles[2] = neighbour_triangles[2];
        }

        void print_data() {
            std::cout << "vertex_indices : " << vertex_indices[0] << " " << vertex_indices[1] << " " << vertex_indices[
                2] << std::endl;
            std::cout << "neighbour_triangles : ";
            for (int i = 0; i < 3; ++i)
                std::cout << neighbour_triangles[i] << " ";
            std::cout << std::endl;
        }
    };

    class TriangulationHandler : public VisualObject {
    private:
        std::vector<int> adjecents{};
        VisualObject* _objectToMove = nullptr;
        int currentTriangle = -1;

        // new way
        std::vector<tri_data> tri_datas;

    public:
        TriangulationHandler(const std::string& vertexFilePath, const std::string& metaFilePath,
                             VisualObject* objectToMove)
            : _objectToMove(objectToMove) {
            // std::cout << "TRIANGULATION HANDLER READING VERTEX DATA" << std::endl;
            mVertices = ReadFileVertex(vertexFilePath);
            // std::cout << "TRIANGULATION HANDLER READING META DATA" << std::endl;
            ReadFileMeta(metaFilePath, mIndices, adjecents);
            // for (int i = 0; i < mIndices.size(); ++i) {
            //     std::cout << mIndices[i] << " " << mIndices[i + 1] << " " << mIndices[i + 2] << std::endl;
            //     i += 2;
            //     /*
            //     std::cout << mIndices[i] << " ";
            //     std::cout << mIndices[++i] << " ";
            //     std::cout << mIndices[++i];
            //     std::cout << std::endl;
            // */
            // }

            SearchEntireTopology(currentTriangle);

            int neigbour[3];
            int indices[3];
            for (int i = 0; i < mIndices.size() / 3; ++i) {
                neigbour[0] = adjecents[i * 3 + 0];
                neigbour[1] = adjecents[i * 3 + 1];
                neigbour[2] = adjecents[i * 3 + 2];

                indices[0] = mIndices[i * 3 + 0];
                indices[1] = mIndices[i * 3 + 1];
                indices[2] = mIndices[i * 3 + 2];
                tri_data data = tri_data(indices, neigbour);
                tri_datas.push_back(data);

                data.print_data();
            }

            // find normals
            std::vector<glm::vec3> normals = CalculateNormals();
        }

        // returns a normal for each vertex in mVertices
        std::vector<glm::vec3> CalculateNormals() {
            std::vector<glm::vec3> retVal = std::vector<glm::vec3>();


            for (int i = 0; i < mVertices.size(); ++i) {
                std::vector<int> trianglesWithSameVertex = FindNeighbourTriangles(i);
                glm::vec3 normal = glm::vec3(0.f);
                for (int j = 0; j < trianglesWithSameVertex.size(); ++j) {
                    normal += calculate_normal_of_triangle_weighted(trianglesWithSameVertex[j]);
                    // std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
                }
                normal = glm::normalize(normal);
                mVertices[i].m_normal[0] = normal.x;
                mVertices[i].m_normal[1] = normal.y;
                mVertices[i].m_normal[2] = normal.z;
            }


            return retVal;
        }

        glm::vec3 calculate_normal_of_triangle_weighted(const int& triangleIndex) {
            int indices[3];
            glm::vec3 positions[3];
            GetTriangleIndices(triangleIndex, indices);
            GetTrianglePositions(triangleIndex, positions);
            glm::vec3 u = positions[0] - positions[1];
            glm::vec3 v = positions[2] - positions[1];
            return glm::cross(v, u);
        }

        void draw() override {
            VisualObject::DrawElements(GL_TRIANGLES, GetModelMatrix());
        }

        void init(GLint matrixUniform) override {
            VisualObject::init(matrixUniform);
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
                                                       _objectToMove->GetPosition());
            if (baryc.x < 0 || baryc.y < 0 || baryc.z < 0)
                return false;
            return true;
        }

        int FindNeighbourTriangle(const int& a, const int& b, const int& ignoreTri) {
            int indices[3];
            for (int i = 0; i < mIndices.size() / 3; ++i) {
                if (i == ignoreTri)
                    continue;
                GetTriangleIndices(i, indices);

                // bool bFoundA = false;
                // bool bFoundB = false;
                // for (int j = 0; j < 3; ++j) {
                //     if (indices[j] == a)
                //         bFoundA = true;
                // }
                // for (int j = 0; j < 3; ++j) {
                //     if (indices[j] == b)
                //         bFoundB = true;
                // }
                // if (bFoundA && bFoundB) {
                //     return i;
                // }

                auto start = mIndices.begin();
                auto end = mIndices.begin();
                std::advance(start, i * 3);
                std::advance(end, i * 3);
                std::advance(end, 3); // advance two i last, move one further becouse of the != end check further down
                // std::cout << "distance : " << std::distance(start, end) << std::endl;
                vector<int>::iterator x = std::find(start, end, a);
                vector<int>::iterator y = std::find(start, end, b);

                if (x != end && y != end) {
                    if (i != ignoreTri) {
                        return i;
                    }
                }
            }
            return -1;
        }

        std::vector<int> FindNeighbourTriangles(const int& vertexIndex) {
            std::vector<int> triangleNeighbours{};

            for (int i = 0; i < tri_datas.size(); ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (tri_datas[i].vertex_indices[j] == vertexIndex) {
                        triangleNeighbours.push_back(i);
                        break;
                    }
                }
            }

            return triangleNeighbours;
        }

        glm::vec3 SearchCurrentAndNearest(int& outTriangle) {
            glm::vec3 pos[3];
            int triIndices[3];
            GetTrianglePositions(currentTriangle, pos);
            GetTriangleIndices(currentTriangle, triIndices);

            glm::vec3 baryc = BarycentricCoordinates3d(pos[0], pos[1], pos[2], _objectToMove->GetPosition());
            if (InTriangle(currentTriangle, _objectToMove->GetPosition())) {
                // in bounds
                glm::vec3 calcpos = pos[0] * baryc.x + pos[1] * baryc.y + pos[2] * baryc.z;
                return calcpos;
            }
            // search nearest neighbour
            // find nearest

            int edge[2];
            if (baryc.x < baryc.y && baryc.x < baryc.z) {
                edge[0] = triIndices[1];
                edge[1] = triIndices[2];
            }
            if (baryc.y < baryc.z && baryc.y < baryc.x) {
                edge[0] = triIndices[0];
                edge[1] = triIndices[2];
            }
            if (baryc.z < baryc.y && baryc.z < baryc.x) {
                edge[0] = triIndices[0];
                edge[1] = triIndices[1];
            }

            int neighbourTri = FindNeighbourTriangle(edge[0], edge[1], currentTriangle);
            std::cout << " edge " << edge[0] << " " << edge[1] << " | " << neighbourTri << std::endl;
            if (neighbourTri != -1 && InTriangle(neighbourTri, _objectToMove->GetPosition())) {
                GetTrianglePositions(neighbourTri, pos);
                // baryc = BarycentricCoordinates3d(pos.begin()._Ptr, _objectToMove->GetPosition());
                baryc = BarycentricCoordinates3d(pos[0], pos[1], pos[2], _objectToMove->GetPosition());
                currentTriangle = neighbourTri;
                std::cout << "Did transition " << currentTriangle << std::endl;
                std::cout << "diff : " << glm::distance(baryc.x * pos[0] + baryc.y * pos[1] + baryc.z * pos[2],
                                                        _objectToMove->GetPosition()) << std::endl;
                return baryc.x * pos[0] + baryc.y * pos[1] + baryc.z * pos[2];
            }

            std::cout << "augh" << std::endl;
            return _objectToMove->GetPosition();
        }

        glm::vec3 SearchEntireTopology(int& outTriangle) {
            for (int i = 0; i < mIndices.size() / 3; ++i) {
                glm::vec3 p1, p2, p3, x;
                p1 = mVertices[mIndices[i * 3 + 0]].posToVec3();
                p2 = mVertices[mIndices[i * 3 + 1]].posToVec3();
                p3 = mVertices[mIndices[i * 3 + 2]].posToVec3();
                // i += 2;
                x = _objectToMove->GetPosition();

                glm::vec3 baryc = BarycentricCoordinates3d(p1, p2, p3, x);
                // std::cout << baryc.x << " " << baryc.y << " " << baryc.z << " | " << baryc.x + baryc.y + baryc.z <<
                // std::endl;
                if (baryc.x >= 0 && baryc.y >= 0 && baryc.z >= 0) {
                    // std::cout << "FOUND POINT IN TRIANGLE" << std::endl;

                    float zz = (p1 * baryc.x + p2 * baryc.y + p3 * baryc.z).z;
                    glm::vec3 newPos = _objectToMove->GetPosition();
                    newPos.z = zz;
                    outTriangle = i;

                    return newPos;
                    // _objectToMove->SetPosition(newPos);
                    break;
                }
            }
            return _objectToMove->GetPosition();
        }

        void Update(float deltaTime) override {
            glm::vec3 pos = SearchCurrentAndNearest(currentTriangle);

            _objectToMove->SetPosition(pos);
        }

    private:
        std::vector<Vertex> ReadFileVertex(const std::string& filePath) {
            std::ifstream inn;
            inn.open(filePath.c_str());
            std::vector<Vertex> vertices{};
            if (inn.is_open()) {
                // fist line
                int n;
                Vertex vertex;
                inn >> n;
                vertices.reserve(n);
                // loops the rest
                for (int i = 0; i < n; ++i) {
                    // inn >> vertex;
                    inn >> vertex.m_xyz[0] >> vertex.m_xyz[1] >> vertex.m_xyz[2];
                    // std::copy(std::begin(vertex.m_xyz), std::end(vertex.m_xyz), std::begin(vertex.m_normal));
                    // float col = (float)i / (float)n;
                    float col = 0.f;
                    vertex.m_normal[0] = col;
                    vertex.m_normal[1] = col;
                    vertex.m_normal[2] = col;
                    vertices.push_back(vertex);

                    // std::cout << vertex << std::endl;
                }
            }
            inn.close();
            return vertices;
        }

        void ReadFileMeta(const std::string& filePath, std::vector<int>& outIndices,
                          std::vector<int>& outAdjecent) {
            std::ifstream inn;
            inn.open(filePath.c_str());
            std::vector<int> indices{};
            std::vector<int> adjecents{};
            if (inn.is_open()) {
                // fist line
                int n;
                int vertex[3];
                int adjacent[3];
                inn >> n;
                indices.reserve(n);
                adjecents.reserve(n);
                // loops the rest
                for (int i = 0; i < n; ++i) {
                    // inn >> vertex;
                    inn >> vertex[0] >> vertex[1] >> vertex[2] >> adjacent[0] >> adjacent[1] >> adjacent[2];
                    // std::copy(std::begin(vertex.m_xyz), std::end(vertex.m_xyz), std::begin(vertex.m_normal));
                    indices.push_back(vertex[0]);
                    indices.push_back(vertex[1]);
                    indices.push_back(vertex[2]);

                    adjecents.push_back(adjacent[0]);
                    adjecents.push_back(adjacent[1]);
                    adjecents.push_back(adjacent[2]);

                    // std::cout << vertex[0] << " " << vertex[1] << " " << vertex[2] << " | "
                    // << adjacent[0] << " " << adjacent[1] << " " << adjacent[2] << std::endl;
                }
            }
            inn.close();
            outIndices = indices;
            outAdjecent = adjecents;
        }
    };
}
