#pragma once
#include "../Geometry/Model.h"

namespace KT
{
    class ModelVisualObject : public VisualObject
    {
        Model* mModel;
        Shader mShader;
    public:
        /**
         * \brief 
         * \param filepath example: "Assets/Art/Models/cube.fbx"
         */
        ModelVisualObject(const std::string& filepath, Shader& shader) : mShader(shader), VisualObject() {
            mModel = new Model(filepath);
        }

        void init(GLint matrixUniform) override {
            mMatrixUniform = matrixUniform;
            
        }
        void Update(float deltaTime) override {
            
        }
        void draw() override
        {
            glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &GetModelMatrix()[0][0]);
            mModel->Draw(mShader);
        }

    };
}
