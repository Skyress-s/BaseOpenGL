#pragma once
#include "TriangleSurface.h"

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
}
