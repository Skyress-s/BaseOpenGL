#pragma once
namespace KT {
    class TextureTest : public VisualObject {
    public:
        TextureTest() {
            mVertices.clear();
            glm::vec3 nor = glm::vec3(0,0,1);
            mVertices.push_back(Vertex(0,0,0, nor.x, nor.y, nor.z, 0,0));
            mVertices.push_back(Vertex(1,0,0, nor.x, nor.y, nor.z, 1,0));
            mVertices.push_back(Vertex(1,1,0, nor.x, nor.y, nor.z, 1,1));
            mVertices.push_back(Vertex(0,1,0, nor.x, nor.y, nor.z, 0,1));

            mIndices.clear();
            mIndices.push_back(0);
            mIndices.push_back(1);
            mIndices.push_back(2);
            
            mIndices.push_back(3);
            mIndices.push_back(0);
            mIndices.push_back(2);
        }
        void init(GLint matrixUniform) override {
        VisualObject::init(matrixUniform);
        }
        
        void draw() override {
            DrawElements(GL_TRIANGLES, GetModelMatrix());
            // std::cout << "being drawn!" << std::endl;
        }
    };
}
