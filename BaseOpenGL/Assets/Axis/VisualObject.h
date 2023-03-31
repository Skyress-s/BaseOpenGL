#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Math/MathHelpers.h"
#include "../Shader/Shader.h"
#include "../Structure/CameraMatricies.h"
#include "../Structure/Vertex.h"

class Shader;

namespace KT {

    class VisualObject {
    public:
        VisualObject();
        virtual ~VisualObject();

        /**
         * \brief Should be called after mVertices are set up, sets up VBOs etc
         * \param matrixUniform 
         */
        virtual void init(GLint matrixUniform);

        virtual void draw() = 0;
        virtual void Update(float deltaTime);
        std::string name{"N/A"};

        glm::vec3 GetPosition() {
            return glm::vec3(x, y, z);
            glm::vec3 pos{};

            pos.x = mModelMatrix[3][0];
            pos.y = mModelMatrix[3][1];
            pos.z = mModelMatrix[3][2];

            return pos;
        }

        void SetPosition(const float& x, const float& y, const float& z) {
            SetPosition(glm::vec3(x, y, z));
        }

        void SetPosition(glm::vec3 newPosition) {
            x = newPosition.x;
            y = newPosition.y;
            z = newPosition.z;
        }

        glm::vec3 GetScale() const {
            return glm::vec3(sx, sy, sz);
        }

        void SetScale(glm::vec3 newScale) {
            sx = newScale[0];
            sy = newScale[1];
            sz = newScale[2];
        }

        void SetScale(const float& uniformScale) {
            sx = uniformScale;
            sy = uniformScale;
            sz = uniformScale;
        }

        void SetScale(const float& x, const float& y, const float& z) {
            sx = x;
            sy = y;
            sz = z;
        }

        // glm::mat4 GetRotationEulerAngles() {

        // }

        glm::quat GetRotation() const {
            return _rotation;
        }

        void SetRotation(glm::vec3 rot) {
            _rotation = glm::quat(rot);
        }

        void SetRotation(glm::quat rot) {
            _rotation = rot;
        }

        void SetRotation(const float& x, const float& y, const float& z) {
            SetRotation(glm::vec3(x, y, z));
        }

        // model data
        void SetVertices(const std::vector<Vertex>& vertices, const std::vector<int>& indices) {
            mVertices = vertices;
            mIndices = indices;
        }

    protected:
        std::vector<KT::Vertex> mVertices;
        std::vector<int> mIndices;
        GLuint mVAO{0};
        GLuint mVBO{0};
        GLuint mEBO{0};
        GLuint mMatrixUniform{0};
        glm::mat4x4 mModelMatrix;

        Shader* mShader = nullptr;
        std::vector<unsigned int> mTextures{};


        glm::mat4 GetModelMatrix() const {
            glm::mat4 mat = glm::mat4(1.f);
            mat = glm::translate(mat, glm::vec3(x, y, z));
            mat *= glm::mat4_cast(_rotation);
            mat = glm::scale(mat, glm::vec3(sx, sy, sz));
            return mat;
        }

        // helper functions for drawing

        /**
         * \brief 
         * \param drawMode typical GL_TRIANGLES, GL_LINES, GL_LINE_LOOP or GL_LINE_STRIP
         */
        void Draw(GLenum drawMode, glm::mat4 modelMatrix) const {
            // mModelMatrix = GetModelMatrix();
            glBindVertexArray(mVAO);

            glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &modelMatrix[0][0]);
            glDrawArrays(drawMode, 0, mVertices.size());

            // good practice
            glBindVertexArray(0);
        }

        void DrawElements(GLenum drawMode, glm::mat4 modelMatrix) {
            glBindVertexArray(mVAO);
            
            glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &modelMatrix[0][0]);
            glDrawElements(drawMode, static_cast<unsigned int>(mIndices.size()), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
        }

        void DrawElementsWithShader(GLenum drawMode, const glm::mat4 model) {
            mShader->use();
            // MathHelpers::print_matrix(CameraView);
            mShader->setMat4("view", CameraView);
            mShader->setMat4("projection", CameraProjection);
            mShader->setMat4("matrix", model);
            
            // set the textures
            // TODO add support for multiple shaders
            if (mTextures.size() >= 1) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mTextures[0]);
            }
            if (mTextures.size() >= 2) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, mTextures[1]);
            }
            
            mShader->use();
            glBindVertexArray(mVAO);
            // glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &modelMatrix[0][0]);
            glDrawElements(drawMode, static_cast<unsigned int>(mIndices.size()), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
        }

        void DrawWithShader(GLenum drawMode) {
            mShader->use();
            
            mShader->setMat4("view", CameraView);
            mShader->setMat4("projection", CameraProjection);
            mShader->setMat4("matrix", GetModelMatrix());
            
            // set the textures
            // TODO add support for multiple shaders
            if (mTextures.size() >= 1) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mTextures[0]);
            }
            if (mTextures.size() >= 2) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, mTextures[1]);
            }
            
            // mModelMatrix = GetModelMatrix();
            glBindVertexArray(mVAO);

            glDrawArrays(drawMode, 0, mVertices.size());
            // good practice
            glBindVertexArray(0);
        }

        float x{};
        float y{};
        float z{};

        glm::quat _rotation = glm::quat(1.f, 0, 0, 0);

        // float rx{};
        // float ry{};
        // float rz{};
        //
        float sx = 1.f;
        float sy = 1.f;
        float sz = 1.f;

        // 
    };
}
