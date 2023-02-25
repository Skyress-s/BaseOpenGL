#pragma once
#include <vector>
#include "Vertex.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace KT
{
    namespace Primitives
    {
        
        /**
         * \brief 
         * \param outVerts can be uninitialized 
         * \param indices can be uninitialized
         * \return 
         */
        void Cube(std::vector<Vertex>& outVerts, std::vector<int>& indices)
        {
            
            outVerts = std::vector<Vertex>();
            indices = std::vector<int>();

            const float u = 0.5f;
            outVerts.push_back(Vertex());
        }
    }
}
