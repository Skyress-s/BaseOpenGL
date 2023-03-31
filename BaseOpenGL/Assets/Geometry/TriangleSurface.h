#pragma once
#include <functional>

#include "../Axis/VisualObject.h"
#include "../Math/TriangulationHandler.h"

namespace KT {
    class TriangleSurface : public VisualObject {
    private:
        std::vector<tri_data> mTriDatas_{};

    public:
        TriangleSurface();
        TriangleSurface(Shader* shader, unsigned int texture);
        TriangleSurface(std::string fileName);
        ~TriangleSurface();

        void readFile(std::string fileName);
        void toFile(std::string fileName);
        void init(GLint matrixUniform) override;
        void draw() override;
        void construct();
        void constructWithLambda(std::function<float(float, float)> func);
        void constructWithTexture(KTTexture2D texture);
        void SetupTriData() {
            const unsigned int numTriangles = mIndices.size() / 3;

            for (int i = 0; i < numTriangles; ++i) {
                tri_data tri_data{};
                tri_data.vertex_indices[0] = mIndices[i * 3];
                tri_data.vertex_indices[0] = mIndices[i * 3+1];
                tri_data.vertex_indices[0] = mIndices[i * 3+2];
            }
        }

    private:
    };
}
