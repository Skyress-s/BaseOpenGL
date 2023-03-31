#include "pch.h"
#include "Cube.h"

namespace KT {
    Cube::Cube(TriangleSurface* target) {
        mTarget = target;
        
        // front
        mVertices.push_back(Vertex(-1,-1,1    ,1,1,1));
        mVertices.push_back(Vertex(1,-1,1    ,1,1,1));
        mVertices.push_back(Vertex(1,1,1    ,1,1,1));
        
        mVertices.push_back(Vertex(1,1,1    ,1,1,1));
        mVertices.push_back(Vertex(-1,1,1    ,1,1,1));
        mVertices.push_back(Vertex(-1,-1,1    ,1,1,1));

        // bottom
        mVertices.push_back(Vertex(1,-1,-1    ,1,1,1));
        mVertices.push_back(Vertex(1,-1,1    ,1,1,1));
        mVertices.push_back(Vertex(-1,-1,1    ,1,1,1));
        
        mVertices.push_back(Vertex(-1,-1,1    ,1,1,1));
        mVertices.push_back(Vertex(-1,-1,-1    ,1,1,1));
        mVertices.push_back(Vertex(1,-1,-1    ,1,1,1));

        // side R
        Vertex s1 = Vertex(1, -1,-1     , 1,1,1);
        Vertex s2 = Vertex(1, 1,-1     , 1,1,1);
        Vertex s3 = Vertex(1, 1,1     , 1,1,1);
        Vertex s4 = Vertex(1, -1,1     , 1,1,1);

        mVertices.push_back(s1);
        mVertices.push_back(s2);
        mVertices.push_back(s3);

        mVertices.push_back(s3);
        mVertices.push_back(s4);
        mVertices.push_back(s1);

        // size L
        s1 = Vertex(-1, -1,-1     , 1,1,1);
        s2 = Vertex(-1, 1,-1     , 1,1,1);
        s3 = Vertex(-1, 1,1     , 1,1,1);
        s4 = Vertex(-1, -1,1     , 1,1,1);

        mVertices.push_back(s3);
        mVertices.push_back(s2);
        mVertices.push_back(s1);

        mVertices.push_back(s1);
        mVertices.push_back(s4);
        mVertices.push_back(s3);

        // back
        mVertices.push_back(Vertex(1,1,-1    ,1,1,1));
        mVertices.push_back(Vertex(1,-1,-1    ,1,1,1));
        mVertices.push_back(Vertex(-1,-1,-1    ,1,1,1));
        
        mVertices.push_back(Vertex(-1,-1,-1    ,1,1,1));
        mVertices.push_back(Vertex(-1,1,-1    ,1,1,1));
        mVertices.push_back(Vertex(1,1,-1    ,1,1,1));


        // top
        mVertices.push_back(Vertex(-1,1,1    ,1,1,1));
        mVertices.push_back(Vertex(1,1,1    ,1,1,1));
        mVertices.push_back(Vertex(1,1,-1    ,1,1,1));
        
        mVertices.push_back(Vertex(1,1,-1    ,1,1,1));
        mVertices.push_back(Vertex(-1,1,-1    ,1,1,1));
        mVertices.push_back(Vertex(-1,1,1    ,1,1,1));
        
        mModelMatrix = glm::mat4x4(1.f); // creates identity matrix
    }

    void Cube::init(GLint matrixUniform) {
        mCurrentTriangle = 0;
        

        glm::vec3 positions[3];
        mTarget->GetTrianglePositions(mCurrentTriangle, positions);
        glm::vec3 pos = 1.f / 3.f * positions[0] + 1.f / 3.f * positions[1] + 1.f / 3.f * positions[2];
        SetPosition(pos);
        VisualObject::init(matrixUniform);
    }

    void Cube::draw() {
        VisualObject::Draw(GL_TRIANGLES, GetModelMatrix());
    }

    
}
