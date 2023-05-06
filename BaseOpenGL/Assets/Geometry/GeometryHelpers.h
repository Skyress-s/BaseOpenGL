#pragma once
#include "TriangleSurface.h"
#include "../Structure/OctahedronBall.h"

namespace KT {
    namespace GeometryHelpers {
        void GetTrianglePositions(const int& i, glm::vec3 outPositions[3], const vector<Vertex>& mVertices,
                                  const vector<int>& mIndices);

        void GetTriangleIndices(const int& i, int indices[3], const vector<int>& mIndices);

        std::vector<unsigned int> GetTrianglesWithVertex(const std::vector<std::vector<unsigned int>>& list,
                                                         const int& vertexIndex);

        glm::vec3 calculate_normal_of_triangle_weighted(const int& triangleIndex, const vector<Vertex>& mVertices,
                                                        const vector<int>& mIndices);


        std::vector<Vertex> planeVertices();

        std::vector<int> planeIndices();

        void plane(std::vector<Vertex>& outVertices, std::vector<int>& outIndices);


    }

    class geometry_helpers : public VisualObject {
    public:
        static std::vector<Vertex> cube;
        static geometry_helpers& getInstance(){
            static geometry_helpers instance;
            return instance;
        }

        void init(GLint matrixUniform) override {
            mVertices = cube;
            VisualObject::init(matrixUniform);   
        }

        void draw() override {
            
        }

        void Draw(const glm::vec3& pos,const glm::quat& rot, const glm::vec3& scale) {
            
            VisualObject::DrawVertices(GL_TRIANGLES, MathHelpers::TRS(pos, rot, scale));
        }

        static void DrawSphere(const glm::vec3& pos,const glm::quat& rot, const glm::vec3& scale) {
            getInstance().Draw(pos, rot, scale);
        }

    private:
        geometry_helpers();

        geometry_helpers(geometry_helpers const&) = delete;
        void operator=(geometry_helpers const&) = delete;
    };
}
