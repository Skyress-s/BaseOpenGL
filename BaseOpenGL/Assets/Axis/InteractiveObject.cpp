#include "InteractiveObject.h"

#include <glm/ext/matrix_transform.hpp>

namespace KT {
    InteractiveObject::InteractiveObject() {
    }

    InteractiveObject::~InteractiveObject() {
    }

    /**
     * \brief 
     * \param x 
     * \param y 
     * \param func float1 = return, float2 param1, float3 param2 
     */
    void InteractiveObject::move(float x, float z, std::function<float(float,float)> func) {
        this->x += x;
        this->z += z;
        float newY = func(this->x,this->z);
        y = newY;
        
        
        
    }
    void InteractiveObject::move(float x, float y, float z) {
        this->x += x;
        this->y += y;
        this->z += z;
        mModelMatrix = glm::translate(mModelMatrix, glm::vec3(x,y,z));
    }
}
