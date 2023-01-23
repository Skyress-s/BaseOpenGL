#pragma once
#include "VisualObjectUI.h"
#include "../Axis/VisualObject.h"

class TransformUI : public VisualObjectUI {
public:
    VisualObject* target{};
    void Draw() override;
};
