#include "TransformUI.h"

#include "../../Vendor/imgui/imgui.h"

void TransformUI::Draw() {
    if (target == nullptr) 
        return;

    // Tansform
    glm::vec3 pos = target->GetPosition();
    float v[3];
    v[0] = pos.x;
    v[1] = pos.y;
    v[2] = pos.z;
    ImGui::DragFloat3(target->name.c_str(), v, 0.1f);
    target->SetPosition(glm::vec3(v[0], v[1], v[2]));

    
}
