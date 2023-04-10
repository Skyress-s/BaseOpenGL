#pragma once
// #include "../Geometry/Model.h"

namespace KT
{
    /**
     * \brief similar to visual object, but uses a model class under the hood
     */
    class ModelVisualObject : public VisualObject
    {
        Model mModel;
        Shader mShader;
    public:
        /**
         * \brief 
         * \param filepath example: "Assets/Art/Models/cube.fbx"
         */
        ModelVisualObject(const std::string& filePath, Shader& shader) : mShader(shader), mModel(filePath) {
        }
        void draw() override
        {
            glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &GetModelMatrix()[0][0]);
            mModel.Draw(mShader);
            
            
        }

    };
}
