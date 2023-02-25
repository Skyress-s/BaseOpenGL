#pragma once
#include "Vertex.h"
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
        void Cube(std::vector<Vertex>& outVerts, std::vector<int>& indices);
    }
}
