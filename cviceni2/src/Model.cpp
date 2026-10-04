#include "Model.h"

Model::Model(const float* vertices, GLsizeiptr bufferSize, GLsizei count)
    : vao(0), vbo(0), vertexCount(count) {

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW);

    GLsizei stride = 6 * sizeof(float);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(3 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Model::~Model() {
    if (vbo != 0) glDeleteBuffers(1, &vbo);
    if (vao != 0) glDeleteVertexArrays(1, &vao);
}

void Model::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}