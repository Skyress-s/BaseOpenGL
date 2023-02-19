#pragma once
#include <vector>

#include "../../Axis/VisualObject.h"
#include "../../IO/FileHandler.h"

namespace KT
{
    class MathComp2Handler : public VisualObject
    {
    private:
        std::vector<glm::vec3> points;

    public:
        MathComp2Handler();
        ~MathComp2Handler() override;

        // misc
        std::vector<Vertex> GetVertices() {
            return mVertices;
        }
        
        // task 1
        void HandleTask1();
        std::vector<glm::vec3> GenerateRandomPoints() const;

        // task 2
        void HandleTask2();

        // IO
        void ToFile(const std::string& filepath) {
            FileHandler::VertexToFile(filepath, mVertices);
        }

        void FromFile(const std::string& filepath) {
            mVertices = FileHandler::VertexFromFile(filepath);
        }
        
        
        // inherited
        void init(GLint matrixUniform) override;
        void draw() override;
        void Update(float deltaTime) override;
    };
}
