#pragma once

#include <glad/gl.h>

class Model {
private:
    GLuint VAO;
    GLuint VBO;
    GLsizei vertexCount;

public:
    Model(const float* vertices, GLsizeiptr dataSize, GLsizei count);
    ~Model();

    Model(const float* vertices, GLsizeiptr dataSize, GLsizei count, bool hasColor);

    void draw() const;
    GLuint getVAO() const;
    GLsizei getVertexCount() const;
};