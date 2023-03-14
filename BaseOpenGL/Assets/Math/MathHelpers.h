#pragma once
#include <iostream>
#include <glm/fwd.hpp>

namespace KT {
    static class MathHelpers {
    public:
        static glm::mat4 TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

        template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
        static GLM_FUNC_QUALIFIER void printt(glm::mat<C, R, T, Q> const& m) {
            for (int y = 0; y < R; ++y) {
                for (int x = 0; x < C; ++x) {
                    std::cout << m[x][y];
                }
                std::cout << std::endl;
            }
        }
    };

    static glm::vec3 barycentricCoordinates3d(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
                                             glm::vec3 x) {
        // 2d plane (xy) 
        p1.z = 0;
        p2.z = 0;
        p3.z = 0;
        x.z = 0;

        // actual code
        glm::vec3 p12 = p2 - p1;
        glm::vec3 p13 = p3 - p1;
        glm::vec3 n = glm::cross(p12, p13);
        // float areal_123 = n.length(); // dobbelt areal
        float areal_123 = glm::length(n); // dobbelt areal
        std::cout << areal_123 << std::endl;
        glm::vec3 baryc; // til retur. Husk
        // u
        glm::vec3 p = p2 - x;
        glm::vec3 q = p3 - x;
        n = glm::cross(p, q);
        baryc.x = n.z / areal_123;
        // v
        p = p3 - x;
        q = p1 - x;
        n = glm::cross(p, q);
        baryc.y = n.z / areal_123;
        // w
        p = p1 - x;
        q = p2 - x;
        n = glm::cross(p, q);
        baryc.z = n.z / areal_123;
        return baryc;
    }

    namespace Random {
        float Random(const float& from, const float& to);
    }
}
