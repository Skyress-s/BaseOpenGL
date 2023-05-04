#pragma once

namespace KT {
    class BSpline : public VisualObject {
    public:
        const float k_tolerance = 0.0000001f;

        BSpline() {
            std::vector<float> ts = {0,0,0, 1, 2, 3, 4,5, 6,6,6};


            int points = 100;
            const int d = 2;
            mVertices.clear();
            for (int j = 0; j < ts.size() - d - 1; ++j) {
                for (int i = 0; i < points; ++i) {
                    float t = i / ((float)points - 1.f)*ts[ts.size()-1];
                    float x = t;
                    float y = BSplineB(ts, t, j, d);
                    std::cout << " t : " << t << " y : " << y << std::endl;

                    mVertices.push_back(Vertex(glm::vec3(x, y, 0), glm::vec3(0, 0, 0)));
                }
            }
        }

        void draw() override {
            DrawVertices(GL_LINE_STRIP, GetModelMatrix());
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
