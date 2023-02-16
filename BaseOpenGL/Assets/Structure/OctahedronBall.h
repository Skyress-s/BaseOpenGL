#pragma once
#include "../Axis/VisualObject.h"


namespace KT
{
    class OctahedronBall : public VisualObject
    {
    public:
        OctahedronBall();
        OctahedronBall(int recurtions);
        ~OctahedronBall();
        void draw() override;
        void init(GLint matrixUniform) override;

        void makeUnitBall();
        void lagTriangel(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
        void Subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, int n);

        static std::vector<Vertex> makeUnitBall(int subs);
        void static lagTriangel(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, std::vector<KT::Vertex>& list);
        void static Subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, int n, std::vector<KT::Vertex>& list);
    private:
        int _recurtions = 1;
        int index;
    };
}
