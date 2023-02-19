#include "MathHelpers.h"

#include <iostream>
#include <random>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace KT {
    glm::mat4 MathHelpers::TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {
        glm::mat4 m = glm::mat4(1.f);
        m = glm::translate(m, position);
        m *= mat4_cast(rotation);
        m = glm::scale(m, scale);
        return m;
    }

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1.f, 6.f); // distribution in range [1, 6]

        float Random::Random(const float& from, const float& to) {
            std::uniform_real_distribution<> distribution(from, to);
            // distribution in range [1, 6]
            return distribution(rng);
    }
}
