#pragma once
#include "VisualObjectUI.h"
#include "../Axis/VisualObject.h"

class TransformUI : public VisualObjectUI {
public:
    KT::VisualObject* target{};
    void Draw() override;
};
