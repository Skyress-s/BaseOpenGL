#include "pch.h"
#include "GeometryHelpers.h"

void KT::GeometryHelpers::GetTrianglePositions(const int& i, glm::vec3 outPositions[3], const vector<Vertex>& mVertices,
    const vector<int>& mIndices) {
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

void KT::GeometryHelpers::GetTriangleIndices(const int& i, int indices[3], const vector<int>& mIndices) {
    indices[0] = mIndices[i * 3 + 0];
    indices[1] = mIndices[i * 3 + 1];
    indices[2] = mIndices[i * 3 + 2];
}


std::vector<unsigned> KT::GeometryHelpers::GetTrianglesWithVertex(const std::vector<std::vector<unsigned>>& list,
                                                                  const int& vertexIndex) {
    return list[vertexIndex];
}

glm::vec3 KT::GeometryHelpers::calculate_normal_of_triangle_weighted(const int& triangleIndex,
                                    const vector<Vertex>& mVertices, const vector<int>& mIndices) {
    
    int indices[3];
    glm::vec3 positions[3];
    GetTriangleIndices(triangleIndex, indices, mIndices);
    GetTrianglePositions(triangleIndex, positions, mVertices, mIndices);
    glm::vec3 u = positions[0] - positions[1];
    glm::vec3 v = positions[2] - positions[1];
    return glm::cross(v, u);
}



