#include "Disc.h"

namespace MM {
    void Disc::draw() {
        mModelMatrix = GetModelMatrix();

        glBindVertexArray(mVAO);
        glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, &mModelMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
        glDrawElements(GL_TRIANGLES, mVertices.size(), GL_UNSIGNED_INT, 0);
    }
}
