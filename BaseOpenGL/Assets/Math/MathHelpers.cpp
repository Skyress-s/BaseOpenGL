#include "MathHelpers.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace KT
{
    glm::mat4 MathHelpers::TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
    {
        glm::mat4 m = glm::mat4(1.f);
        m = glm::translate(m, position);
        m *= mat4_cast(rotation);
        m = glm::scale(m, scale);
        return m;
    }
}
