﻿#pragma once
#include "Structure/Primitives.h"

namespace KT
{
    class Door : public VisualObject
    {
    private:
        Model mModel;
        Shader& mShader;
        VisualObject* mTarget{};

    public:
        Door(VisualObject* target, std::string modePath, Shader& shader): mModel(modePath), mShader(shader)
        {
            mTarget = target;

            /*
            Primitives::Cube(mVertices, mIndices);
            for (int i = 0; i < mVertices.size(); ++i)
            {
                // offset
                mVertices[i].m_xyz[0] += 0.5;

                // thining
                mVertices[i].m_xyz[2] *= 0.1f;

                // Heighting
                mVertices[i].m_xyz[1] *= 2.0f;
            }
        */
        }

        void draw() override
        {
            mShader.setMat4("matrix", GetModelMatrix());
            mModel.Draw(mShader);
            // DrawElements(GL_TRIANGLES, GetModelMatrix());
        }

        void Update(float deltaTime) override
        {
            glm::quat targetQuat{};
            if (glm::distance(GetPosition(), mTarget->GetPosition()) < 0.62f) // open
            {
                targetQuat = glm::quatLookAt(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
            }
            else
            {
                targetQuat = glm::quatLookAt(glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
            }
            _rotation = glm::slerp(_rotation, targetQuat, deltaTime * 4.f);
            // x += deltaTime;
            // y += deltaTime;
        }

        void init(GLint matrixUniform) override
        {
            VisualObject::init(matrixUniform);
        }
    };
}
