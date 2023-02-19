#pragma once
#include <glm/vec3.hpp>

struct Bounds {
public:
    glm::vec3 center;
    glm::vec3 size;

    bool InBounds(const glm::vec3 point) {
        if (point.x > center.x + size.x/2.f || point.x < center.x - size.x/2.f) {
            return false;
        }
        if (point.y > center.y + size.y/2.f || point.y < center.y - size.y/2.f) {
            return false;
        }
        if (point.z > center.z + size.z/2.f || point.z < center.z - size.z/2.f) {
            return false;
        }
        return true;
    }
};
