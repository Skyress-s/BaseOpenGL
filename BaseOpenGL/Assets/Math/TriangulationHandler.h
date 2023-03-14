#pragma once
#include "MathHelpers.h"
#include "../IO/FileHandler.h"

namespace KT {
    typedef std::pair<int[3], int[3]> TriVertex;

    class TriangulationHandler : public VisualObject {
    private:
        std::vector<int> adjecents{};
        VisualObject* _objectToMove = nullptr;
        int currentTriangle = -1;

    public:
        TriangulationHandler(const std::string& vertexFilePath, const std::string& metaFilePath,
                             VisualObject* objectToMove)
            : _objectToMove(objectToMove) {
            std::cout << "TRIANGULATION HANDLER READING VERTEX DATA" << std::endl;
            mVertices = ReadFileVertex(vertexFilePath);
            std::cout << "TRIANGULATION HANDLER READING META DATA" << std::endl;
            ReadFileMeta(metaFilePath, mIndices, adjecents);
            for (int i = 0; i < mIndices.size(); ++i) {
                std::cout << mIndices[i] << " " << mIndices[i + 1] << " " << mIndices[i + 2] << std::endl;
                i += 2;
                /*
                std::cout << mIndices[i] << " ";
                std::cout << mIndices[++i] << " ";
                std::cout << mIndices[++i];
                std::cout << std::endl;
            */
            }

            SearchEntireTopology(currentTriangle);
            
        }

        void draw() override {
            VisualObject::DrawElements(GL_TRIANGLES, GetModelMatrix());
        }

        void init(GLint matrixUniform) override {
            VisualObject::init(matrixUniform);
        }

        void Update(float deltaTime) override {

            //  SearchCurrentAndNearest(currentTriangle);
            //
            // glm::vec3 position = SearchEntireTopology(tri);
            // _objectToMove->SetPosition(position);
        }

        void GetTrianglePositions(const int& i, vector<glm::vec3>& outPositions) {
            glm::vec3 p1,p2,p3;
            p1 = mVertices[mIndices[i*3+0]].posToVec3();
            p2 = mVertices[mIndices[i*3+1]].posToVec3();
            p3 = mVertices[mIndices[i*3+2]].posToVec3();
            outPositions.clear();
            outPositions.push_back(p1);
            outPositions.push_back(p2);
            outPositions.push_back(p3);
        }

        void GetTriangleIndices(const int& i, int indices[3]) {
            indices[0] = mIndices[i*3+0];
            indices[1] = mIndices[i*3+1];
            indices[2] = mIndices[i*3+2];
        }
        
        glm::vec3 SearchEntireTopology(int& outTriangle) {
            for (int i = 0; i < mIndices.size()/3; ++i) {
                glm::vec3 p1, p2, p3, x;
                p1 = mVertices[mIndices[i*3 + 0]].posToVec3();
                p2 = mVertices[mIndices[i*3 + 1]].posToVec3();
                p3 = mVertices[mIndices[i*3 + 2]].posToVec3();
                // i += 2;
                x = _objectToMove->GetPosition();

                glm::vec3 baryc = barycentricCoordinates3d(p1, p2, p3, x);
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

        bool InTriangle(const int& i, const glm::vec3& x) {
            std::vector<glm::vec3> positions{};
            GetTrianglePositions(i, positions);
            glm::vec3 baryc = barycentricCoordinates3d(positions[0], positions[1], positions[2], _objectToMove->GetPosition());
            if (baryc.x <0 || baryc.y < 0 || baryc.z < 0) 
                return false;
            return true;
        }

        auto GetEdge(const int& a,const int& b) {
            int indices[3];
            for (int i = 0; i < mIndices.size() / 3; ++i) {
                
                // GetTriangleIndices(i, )
            }
        }
        
        glm::vec3 SearchCurrentAndNearest(int& outTriangle) {
            std::vector<glm::vec3> pos = std::vector<glm::vec3>(); 
            GetTrianglePositions(currentTriangle, pos);
            int triIndices[3];
            GetTriangleIndices(currentTriangle, triIndices);
            
            glm::vec3 baryc = barycentricCoordinates3d(pos[0], pos[1], pos[2], _objectToMove->GetPosition());
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
                    float col = (float)i / (float)n;
                    vertex.m_normal[0] = col;
                    vertex.m_normal[1] = col;
                    vertex.m_normal[2] = col;
                    vertices.push_back(vertex);

                    std::cout << vertex << std::endl;
                }
            }
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

                    std::cout << vertex[0] << " " << vertex[1] << " " << vertex[2] << " | "
                        << adjacent[0] << " " << adjacent[1] << " " << adjacent[2] << std::endl;
                }
            }
            outIndices = indices;
            outAdjecent = adjecents;
        }
    };
}
