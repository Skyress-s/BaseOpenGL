#pragma once
#include "Axis/InteractiveObject.h"
#include "IO/FileHandler.h"
#include "Structure/GeneralVisualObject.h"

namespace KT {
    class Key : public KT::GeneralVisualObject {
    private:
        KT::InteractiveObject* mTarget;
        bool mInKey = false;

    public:
        Key(KT::InteractiveObject* target, const std::string& filepath) {
            KT::FileHandler::Import_obj_importer(filepath, mVertices, mIndices);
            std::cout << "vertices : " << mVertices.size() << " " << "indices : " << mIndices.size() << std::endl;
            mTarget = target;
        }

        void draw() override {
            VisualObject::DrawElements(GL_TRIANGLES, GetModelMatrix());
        }

        bool IsInRange() {
            if (glm::distance(GetPosition(), mTarget->GetPosition()) < 0.04) 
                return true;
            return false;
        }
        
        void Update(float deltaTime) override {
            if (IsInRange() && !mInKey) {
                mInKey = true;
            }
            else {
                mInKey = false;
            }
        }
    };
}
