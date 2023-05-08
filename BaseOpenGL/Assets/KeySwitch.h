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
            VisualObject::DrawElements(GL_TRIANGLES, GetModelMatrix());
        }

        bool IsInRange() {
            if (glm::distance(GetPosition(), mTarget->GetPosition()) < 0.04)
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
