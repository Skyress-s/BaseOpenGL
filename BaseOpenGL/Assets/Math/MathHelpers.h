#pragma once
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>

namespace KT
{
    class MathHelpers
    {
    public:
        static glm::mat4 TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
    };
}
