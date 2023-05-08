#include "pch.h"
#include "playerObject.h"

namespace KT {
    playerObject::playerObject(TriangleSurface* target, std::vector<Vertex> vertices, std::vector<int> indices, Shader* shader) {
        mTarget = target;
        mVertices = vertices;
        mIndices = indices;
        mShader = shader;
        // // front
        // mVertices.push_back(Vertex(-1,-1,1    ,1,1,1, 0,0));
        // mVertices.push_back(Vertex(1,-1,1    ,1,1,1, 0,1));
        // mVertices.push_back(Vertex(1,1,1    ,1,1,1, 1,1));
        //
        // mVertices.push_back(Vertex(1,1,1    ,1,1,1, 1,0));
        // mVertices.push_back(Vertex(-1,1,1    ,1,1,1,0,0));
        // mVertices.push_back(Vertex(-1,-1,1    ,1,1,1,0,1));
        //
        // // bottom
        // mVertices.push_back(Vertex(1,-1,-1    ,1,1,1,1,1));
        // mVertices.push_back(Vertex(1,-1,1    ,1,1,1,1,0));
        // mVertices.push_back(Vertex(-1,-1,1    ,1,1,1, 1, 1));
        //
        // mVertices.push_back(Vertex(-1,-1,1    ,1,1,1, 0, 0));
        // mVertices.push_back(Vertex(-1,-1,-1    ,1,1,1, 0,1));
        // mVertices.push_back(Vertex(1,-1,-1    ,1,1,1,1,0));
        //
        // // side R
        // Vertex s1 = Vertex(1, -1,-1     , 1,1,1);
        // Vertex s2 = Vertex(1, 1,-1     , 1,1,1);
        // Vertex s3 = Vertex(1, 1,1     , 1,1,1);
        // Vertex s4 = Vertex(1, -1,1     , 1,1,1);
        //
        // mVertices.push_back(s1);
        // mVertices.push_back(s2);
        // mVertices.push_back(s3);
        //
        // mVertices.push_back(s3);
        // mVertices.push_back(s4);
        // mVertices.push_back(s1);
        //
        // // size L
        // s1 = Vertex(-1, -1,-1     , 1,1,1);
        // s2 = Vertex(-1, 1,-1     , 1,1,1);
        // s3 = Vertex(-1, 1,1     , 1,1,1);
        // s4 = Vertex(-1, -1,1     , 1,1,1);
        //
        // mVertices.push_back(s3);
        // mVertices.push_back(s2);
        // mVertices.push_back(s1);
        //
        // mVertices.push_back(s1);
        // mVertices.push_back(s4);
        // mVertices.push_back(s3);
        //
        // // back
        // mVertices.push_back(Vertex(1,1,-1    ,1,1,1));
        // mVertices.push_back(Vertex(1,-1,-1    ,1,1,1));
        // mVertices.push_back(Vertex(-1,-1,-1    ,1,1,1));
        //
        // mVertices.push_back(Vertex(-1,-1,-1    ,1,1,1));
        // mVertices.push_back(Vertex(-1,1,-1    ,1,1,1));
        // mVertices.push_back(Vertex(1,1,-1    ,1,1,1));
        //
        //
        // // top
        // mVertices.push_back(Vertex(-1,1,1    ,1,1,1));
        // mVertices.push_back(Vertex(1,1,1    ,1,1,1));
        // mVertices.push_back(Vertex(1,1,-1    ,1,1,1));
        //
        // mVertices.push_back(Vertex(1,1,-1    ,1,1,1));
        // mVertices.push_back(Vertex(-1,1,-1    ,1,1,1));
        // mVertices.push_back(Vertex(-1,1,1    ,1,1,1));
        
        mModelMatrix = glm::mat4x4(1.f); // creates identity matrix
    }

    void playerObject::init(GLint matrixUniform) {
        mCurrentTriangle = 0;
        

        glm::vec3 positions[3];
        mTarget->GetTrianglePositions(mCurrentTriangle, positions);
        glm::vec3 pos = 1.f / 3.f * positions[0] + 1.f / 3.f * positions[1] + 1.f / 3.f * positions[2];
        SetPosition(pos);
        
        VisualObject::init(matrixUniform);
    }

    void playerObject::draw() {
        VisualObject::DrawElementsWithShader(GL_TRIANGLES, GetModelMatrix());
        // VisualObject::DrawElements(GL_TRIANGLES, GetModelMatrix());
        // VisualObject::DrawVertices(GL_TRIANGLES, GetModelMatrix());
    }

    
}
