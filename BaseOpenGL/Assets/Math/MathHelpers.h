#pragma once
#include <iostream>
#include <GLFW/glfw3native.h>
#include <glm/fwd.hpp>

namespace KT {
    static class MathHelpers {
    public:
        static glm::mat4 TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

        template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
        static GLM_FUNC_QUALIFIER void print_matrix(glm::mat<C, R, T, Q> const& m) {
            for (int y = 0; y < R; ++y) {
                for (int x = 0; x < C; ++x) {
                    std::cout << m[x][y];
                }
                std::cout << std::endl;
            }
        }

        static float ReScale(const float& val, const float& startLow, const float& startHigh, const float& endLow,
                             const float& endHigh);
        
        template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
        void SolveThreePlanes(glm::mat<C, R, T, Q> mat, glm::vec<R, T, Q> equals);
    };

    template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
    void MathHelpers::SolveThreePlanes(glm::mat<C, R, T, Q> mat, glm::vec<R, T, Q> equals) {
        static_assert(R == C, "Rows and Collums has to be equal!");
        // example how to do it
        // glm::mat3 testMat = glm::mat3(1.f);
        // testMat[0][0] = 1.f;
        // testMat[1][0] = 1.f;
        // testMat[2][0] = 1.f;
        //
        // testMat[0][1] = 5.f;
        // testMat[1][1] = 3.f;
        // testMat[2][1] = 2.f;
        //
        // testMat[0][2] = 1.f;
        // testMat[1][2] = 3.f;
        // testMat[2][2] = 2.f;
        std::cout << "MATRIX" << std::endl;
        print(mat);
        
        glm::mat<C, R, T, Q> inv = inverse(mat);
        
        
        glm::vec<R, T, Q> res = inv * equals;
        std::cout << "RESULTS XYZW VALUES" << std::endl;
        for (int i = 0; i < R; ++i) {
            std::cout << res[i] << " ";
        }
        std::cout << std::endl;
        
        std::cout << "RECALCULATE" << std::endl;
        for (int y = 0; y < C; ++y) {
            float plane = 0.f;
            for (int x = 0; x < C; ++x) {
                plane += mat[x][y] * res[x];
            }
            std::cout << plane << " ";
        }
        std::cout << std::endl;
        
        std::cout << "SHOULD EQUAL" << std::endl;
        for (int i = 0; i < R; ++i) {
            std::cout << equals[i] << " ";
        }
        std::cout << std::endl;
        
        
        return;
    }


    static glm::vec3 BarycentricCoordinates3d(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
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
        // std::cout << areal_123 << std::endl;
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

    static glm::vec3 BarycentricCoordinates3d(glm::vec3 pps[3], glm::vec3 x) {
        return BarycentricCoordinates3d(pps[0], pps[1], pps[2], x);
    }

    static glm::vec3 BarycentricCoordinatesXZ(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
                                              glm::vec3 x) {
        // saving x
        p1.y = p1.x;
        p2.y = p2.x;
        p3.y = p3.x;
        x.y = x.x;

        // transferring x to y
        p1.x = p1.z;
        p2.x = p2.z;
        p3.x = p3.z;
        x.x = x.z;
        // setting to zero
        p1.z = p2.z = p3.z = x.z = 0.0f;
        return BarycentricCoordinates3d(p1, p2, p3, x);
        // actual code
        glm::vec3 p12 = p2 - p1;
        glm::vec3 p13 = p3 - p1;
        glm::vec3 n = glm::cross(p12, p13);
        // float areal_123 = n.length(); // dobbelt areal
        float areal_123 = glm::length(n); // dobbelt areal
        // std::cout << areal_123 << std::endl;
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
