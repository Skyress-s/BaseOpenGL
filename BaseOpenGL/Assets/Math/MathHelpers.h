#pragma once
#include <iostream>
#include <glm/fwd.hpp>

namespace KT
{
    static class MathHelpers
    {
    public:
        static glm::mat4 TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

        template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
        static GLM_FUNC_QUALIFIER void printt(glm::mat<C, R, T, Q> const& m)
        {
            for (int y = 0; y < R; ++y)
            {
                for (int x = 0; x < C; ++x)
                {
                    std::cout << m[x][y];
                }
                std::cout << std::endl;
            }
        }
    };
}
