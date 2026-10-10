#include "Model.h"

Model::Model(const float* vertices, GLsizeiptr dataSize, GLsizei count)
    : VAO(0), VBO(0), vertexCount(count) {

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertices, GL_STATIC_DRAW);

    GLsizei stride = 6 * sizeof(float);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(3 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

GLuint Model::getVAO() const { return VAO; }
GLsizei Model::getVertexCount() const { return vertexCount; }

Model::~Model() {
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
    }
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }
}