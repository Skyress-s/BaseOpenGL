#pragma once
#include "Axis/InteractiveObject.h"
#include "IO/FileHandler.h"
#include "Structure/GeneralVisualObject.h"

namespace KT {
    class KeySwitch : public KT::GeneralVisualObject {
    private:
        KT::InteractiveObject* mTarget;
        bool mInKey = false;
        bool mIsOn = true;

    public:
        KeySwitch(KT::InteractiveObject* target, const std::string& filepath) {
            KT::FileHandler::Import_obj_importer(filepath, mVertices, mIndices);
            std::cout << "vertices : " << mVertices.size() << " " << "indices : " << mIndices.size() << std::endl;
            mTarget = target;
        }

        void draw() override {
            VisualObject::DrawElementsWithShader(GL_TRIANGLES, GetModelMatrix());
        }

        bool IsInRange() {
            // only check in xz plane
            glm::vec2 p1 = glm::vec2(GetPosition().x, GetPosition().z);
            glm::vec2 p2 = glm::vec2(mTarget->GetPosition().x, mTarget->GetPosition().z); 
            if (glm::distance(p1,p2) < 0.04f)
                return true;
            return false;
        }

        bool IsOn() {
            return mIsOn;
        }

        void Update(float deltaTime) override {
            if (IsInRange()) {
                if (!mInKey) {
                    mInKey = true;
                    mIsOn = !mIsOn;
                }
            }
            else {
                mInKey = false;
            }
        }
    };
}
