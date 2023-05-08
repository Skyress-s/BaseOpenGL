#pragma once
#include <GLFW/glfw3.h>

#include "../Geometry/GeometryHelpers.h"

namespace KT {
    class BSpline : public VisualObject {
    public:
        const float k_tolerance = 0.0000001f;
        std::vector<glm::vec3> mControlPoints;
        std::vector<float> mKnots;
        int degree = -1;


        BSpline(std::vector<glm::vec3> splinePoints, const int& d) {
            degree = d;
            mControlPoints = splinePoints;
            const unsigned int knots = splinePoints.size() + d + 1;
            mKnots = std::vector<float>();
            int sideKnots = knots - splinePoints.size();
            
            // ts.push_back(0);
            for (int i = 0; i < 2; ++i)
                mKnots.push_back(0);
            for (int i = 0; i < splinePoints.size(); ++i)
                mKnots.push_back(i);
            for (int i = 0; i < 2; ++i)
                mKnots.push_back(splinePoints.size() - 1);

            std::cout << "knots needed : " << knots << "  current knots : " << mKnots.size() << std::endl;
            std::cout << "side knots : " << sideKnots << std::endl;
            const int points = 100;
            for (int j = 0; j < splinePoints.size()/*ts.size() - d - 1*/; ++j) {
                for (int i = 0; i < points; ++i) {
                    float t = i / ((float)points - 1.f) * mKnots[mKnots.size() - 1];
                    float x = t;
                    float y = BSplineB(mKnots, t, j, d);

                    mVertices.push_back(Vertex(glm::vec3(x, y, 0), glm::vec3(0, 0, 0)));
                }
            }
        }

        BSpline() {
            std::vector<float> ts = {0, 0, 0, 1, 2, 3, 4, 5, 6, 6, 6};


            int points = 100;
            const int d = 2;
            mVertices.clear();
            for (int j = 0; j < ts.size() - d - 1; ++j) {
                for (int i = 0; i < points; ++i) {
                    float t = i / ((float)points - 1.f) * ts[ts.size() - 1];
                    float x = t;
                    float y = BSplineB(ts, t, j, d);

                    mVertices.push_back(Vertex(glm::vec3(x, y, 0), glm::vec3(0, 0, 0)));
                }
            }
        }

        void init(GLint matrixUniform) override {
            // mVertices.clear();

            const int points = 100;
            for (int i = 0; i < points; ++i) {
                float t = i / ((float)points - 1.f);
                glm::vec3 pos = Evaluate(t);
                std::cout << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
                mVertices.push_back(Vertex(pos, glm::vec3(0, 1, 0)));
            }


            VisualObject::init(matrixUniform);
        }

        void draw() override {
            // draw spline weights   
            DrawVertices(GL_LINE_STRIP, GetModelMatrix());

            float t = sin(glfwGetTime() * 0.8f) * 0.5f + 0.5f; // 0-1


            glm::vec3 tPosition = Evaluate(t);

            // draw tPosition
            geometry_helpers::DrawSphere(tPosition, glm::identity<glm::quat>(), glm::vec3(0.05f));

            const glm::quat rot = glm::identity<glm::quat>();
            const glm::vec3 scale = glm::vec3(0.1f);
            for (int i = 0; i < mControlPoints.size(); ++i) {
                geometry_helpers::DrawSphere(mControlPoints[i], rot, scale);
            }
        }

        /**
         * \brief 
         * \param t 0 - 1
         * \return 
         */
        glm::vec3 Evaluate(const float& t) {
            // resize
            float val = MathHelpers::ReScale(
                t, 0, 1, mKnots[0] + 1.f, mKnots[mKnots.size() - 1] - 1.f
            );
            val = MathHelpers::ReScale(t, 0, 1, 0 + 1.f, mKnots[mKnots.size() - 1] - 1.f);

            glm::vec3 position{};
            for (int i = 0; i < mControlPoints.size(); ++i) {
                position += mControlPoints[i] * BSplineB(mKnots, val, i, degree);
            }

            return position;
        }

        float BSplineB(std::vector<float> ts, float t, int i, int d) {
            if (d == 0) {
                if (ts[i] <= (t) && (t) < ts[i + 1])
                    return 1.f;
                else
                    return 0.f;
            }

            return W(ts, t, i, d) * BSplineB(ts, t, i, d - 1) +
                (1.f - W(ts, t, i + 1, d)) * BSplineB(ts, t, i + 1, d - 1);
        }

        float W(std::vector<float> ts, float t, int i, int d) {
            if (ts[i] < ts[i + d]) {
                return (t - (float)ts[i]) / ((float)ts[i + d] - (float)ts[i]);
            }
            return 0;
        }
    };
}
