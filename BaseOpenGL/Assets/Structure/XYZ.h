#pragma once
#include <glad/glad.h>

#include "../Axis/VisualObject.h"

namespace KT {
    
class XYZ : public VisualObject {
public:
    XYZ();
    ~XYZ();
    void init(GLint matrixUniform) override;
    void draw() override;
};
}
