#pragma once

#include <glad/gl.h>

class Model {
private:
    GLuint vao;
    GLuint vbo;
    GLsizei vertexCount;

public:
    Model(const float* vertices, GLsizeiptr bufferSize, GLsizei count);
    ~Model();

    void draw() const;
    GLuint getVAO() const { return vao; }
};