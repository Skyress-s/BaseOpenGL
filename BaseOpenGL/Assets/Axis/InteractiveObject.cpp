#include "InteractiveObject.h"

#include <glm/ext/matrix_transform.hpp>

namespace MM {
    InteractiveObject::InteractiveObject() {
    }

    InteractiveObject::~InteractiveObject() {
    }

    
    
    void InteractiveObject::move(float x, float y, float z) {
        this->x += x;
        this->y += y;
        this->z += z;
        mModelMatrix = glm::translate(mModelMatrix, glm::vec3(x,y,z));
    }
}
