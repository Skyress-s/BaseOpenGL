#pragma once
#include <assimp/anim.h>
#include <assimp/Importer.hpp>

// #include "Geometry/Model.h"
#include "Gizmo/GizmoDrawer.h"
#include "Math/MathHelpers.h"
#include "Structure/Bounds.h"

namespace KT {
    class House : public VisualObject {
    private:
        Model mModel;
        Shader& mShader;

        GizmoDrawer boundsDrawer;
    public:
        House(const std::string& modelPath, Shader& shader):
            mShader(shader), mModel(modelPath) {
            boundsDrawer = GizmoDrawer();
        }

        Bounds GetBounds() { return Bounds(GetPosition() + glm::vec3(0,0.4,0), GetScale() * 2.4f); }

        void init(GLint matrixUniform) override {
            Bounds bounds = GetBounds();
            boundsDrawer.init(bounds.GetIndices(), bounds.GetVertices());
        }


        void draw() override {
            mShader.setMat4("matrix", GetModelMatrix());
            // boundsDrawer.draw(GL_TRIANGLES/*, GetModelMatrix(), mShader*/);
            mModel.Draw(mShader);
            boundsDrawer.draw(GL_LINES, MathHelpers::TRS(GetBounds().mCenter, glm::quat(1,0,0,0), glm::vec3(1.f)), mShader);
        }
    };
}
