#pragma once

namespace KT {
    class GeneralVisualObject : public VisualObject {
    public:
        GeneralVisualObject() = default;

        GeneralVisualObject(const vector<Vertex>& vertices) : GeneralVisualObject() {
            mVertices = vertices;
        }
        GeneralVisualObject(const vector<Vertex>& vertices, const vector<int>& indices) : GeneralVisualObject(vertices) {
            mIndices = indices;
        }

        

        void UseShader(Shader* shader) {
            mShader = shader;
        }

        void AddTexture(unsigned int texture) {
            mTextures.push_back(texture);
        }

        void draw() override {
            const GLenum drawMode = GL_TRIANGLES;
            const glm::mat4 model = GetModelMatrix();
            // elements
            if (mIndices.size() > 0) {
                if (mShader == nullptr) {
                    DrawElements(drawMode, model);
                    return;
                }
                DrawElementsWithShader(drawMode, model);
                return;
            }

            // vertex array
            if (mShader == nullptr) {
                DrawVertices(drawMode, model);
                return;
            }
            DrawWithShader(drawMode);
            return;
        }
    };
}
