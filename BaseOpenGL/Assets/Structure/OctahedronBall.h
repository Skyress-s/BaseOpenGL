#pragma once
#include "../Axis/VisualObject.h"

namespace MM
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

private:
    static const glm::vec3 baseOctahedron[];
    int _recurtions = 1;
    int index;
};
}
