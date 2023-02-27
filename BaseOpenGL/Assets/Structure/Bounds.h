#pragma once
#include <glm/vec3.hpp>


namespace KT {
    struct Bounds {
    public:
        glm::vec3 mCenter;
        glm::vec3 mExtends;


        Bounds(const glm::vec3& center, const glm::vec3& size) {
            mCenter = center;
            mExtends = size / 2.f;
        }

        glm::vec3 GetScale() const { return mExtends * 2.f; }
        void SetScale(const glm::vec3& scale) { mExtends = scale / 2.f; }

        bool InBounds(const glm::vec3 point) const {
            if (point.x > mCenter.x + mExtends.x || point.x < mCenter.x - mExtends.x) {
                return false;
            }
            if (point.y > mCenter.y + mExtends.y || point.y < mCenter.y - mExtends.y) {
                return false;
            }
            if (point.z > mCenter.z + mExtends.z || point.z < mCenter.z - mExtends.z) {
                return false;
            }
            return true;
        }

        bool InBounds(const Bounds& bounds) const {
            
        }

        std::vector<Vertex> GetVertices() {
            
            std::vector<Vertex> outVertices = std::vector<Vertex>(0);
            // outVertices.reserve(8);
            constexpr glm::vec3 white = glm::vec3(1,1,1);
            
            outVertices.push_back( Vertex(mExtends.x, -mExtends.y, mExtends.z, white));
            outVertices.push_back( Vertex(-mExtends.x, -mExtends.y, mExtends.z, white));
            outVertices.push_back( Vertex(-mExtends.x, -mExtends.y, -mExtends.z, white));
            outVertices.push_back( Vertex(mExtends.x, -mExtends.y, -mExtends.z, white));
            
            outVertices.push_back( Vertex(mExtends.x, mExtends.y, mExtends.z, white));
            outVertices.push_back( Vertex(-mExtends.x, mExtends.y, mExtends.z, white));
            outVertices.push_back( Vertex(-mExtends.x, mExtends.y, -mExtends.z, white));
            outVertices.push_back( Vertex(mExtends.x, mExtends.y, -mExtends.z, white));
            return outVertices;
        }

        std::vector<int> GetIndices() {
            std::vector<int> outIndices = std::vector<int>();
            // outIndices.reserve(12*2);
            // outIndices.reserve(8);
            outIndices.push_back(0);
            outIndices.push_back(1);
            
            outIndices.push_back(1);
            outIndices.push_back(2);
            
            outIndices.push_back(2);
            outIndices.push_back(3);
            
            outIndices.push_back(3);
            outIndices.push_back(0);

            // sides
            outIndices.push_back(0);
            outIndices.push_back(4);
            
            outIndices.push_back(1);
            outIndices.push_back(5);
            
            outIndices.push_back(2);
            outIndices.push_back(6);
            
            outIndices.push_back(3);
            outIndices.push_back(7);

            // top
            outIndices.push_back(4);
            outIndices.push_back(5);
            
            outIndices.push_back(5);
            outIndices.push_back(6);
            
            outIndices.push_back(6);
            outIndices.push_back(7);
            
            outIndices.push_back(7);
            outIndices.push_back(4);
            
            return outIndices;
        }
        
        void VerticesAndIndicesLS(std::vector<Vertex>& outVertices, std::vector<int>& outIndices) {
            outVertices = std::vector<Vertex>(0);
            outVertices.reserve(8);
            constexpr glm::vec3 white = glm::vec3(1,1,1);
            
            outVertices[0] = Vertex(mExtends.x, -mExtends.y, mExtends.z, white);
            outVertices[1] = Vertex(-mExtends.x, -mExtends.y, mExtends.z, white);
            outVertices[2] = Vertex(-mExtends.x, -mExtends.y, -mExtends.z, white);
            outVertices[3] = Vertex(mExtends.x, -mExtends.y, -mExtends.z, white);
            
            outVertices[4] = Vertex(mExtends.x, mExtends.y, mExtends.z, white);
            outVertices[5] = Vertex(-mExtends.x, mExtends.y, mExtends.z, white);
            outVertices[6] = Vertex(-mExtends.x, mExtends.y, -mExtends.z, white);
            outVertices[7] = Vertex(mExtends.x, mExtends.y, -mExtends.z, white);


            outIndices = std::vector<int>();
            // outIndices.reserve(12*2);
            outIndices.reserve(8);
            outIndices[0] = 0;
            outIndices[0] = 1;
            
            outIndices[0] = 1;
            outIndices[0] = 2;
            
            outIndices[0] = 2;
            outIndices[0] = 3;
            
            outIndices[0] = 3;
            outIndices[0] = 0;
        }
    };
}
