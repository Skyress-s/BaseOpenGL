#pragma once
#include <functional>

#include "GeometryHelpers.h"
#include "../Axis/VisualObject.h"
#include "../Math/TriangulationHandler.h"

namespace KT {
    class TriangleSurface : public VisualObject {
    private:
        std::vector<tri_data> mTriDatas_{};
        std::vector<std::vector<unsigned int>> vertexTriangleIndexes{}; // each vertex has a list of triangles it is part of

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

        std::vector<unsigned int> FindCommonElements(const std::vector<unsigned int>& list1,
                                                     const std::vector<unsigned int>& list2);

        void SetupTriData();

        void FindNeighbourTriangles(const int& currentTriangleIndex, int triIndexes[3]);

        void CalculateNormals();

        // void GetTrianglePositions(const int& i, vector<glm::vec3>& outPositions) {
        void GetTrianglePositions(const int& i, glm::vec3 outPositions[3]) const;

        void GetTriangleIndices(const int& i, int indices[3]);

        bool InTriangleXZ(const int& i, const glm::vec3& x) const;

        int FindNeighbourTriangleRaw(const int& vertexIndexA, const int& vertexIndexB, const int& ignoreTriIndex);

        int FindNeigbourTriangle(const int& currentTriangle, const int& edge1, const int& edge2);

        glm::vec3 SearchCurrentAndNearest(unsigned int& outCurrentTriangle, const glm::vec3& x);

        glm::vec3 FindPointOnSurfaceXZ(const glm::vec3& point) const;

        ///write a functions that projects a point onto a line
    private:
        /*
        std::vector<int> neighbour_triangle_indices(int index) {
            std::vector<int> retVal{};
            const unsigned int numTriangles = mIndices.size() / 3;
        }
    */
    };
}
