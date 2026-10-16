/**
 * @file Model.h
 * @author Student (login: ZHA0067)
 * @brief Model class for handling 3D model data and rendering.
 */
#pragma once

#include <glad/gl.h>

class Model {
private:
    GLuint VAO;
    GLuint VBO;
    GLsizei vertexCount;

public:
    Model(const float* vertices, GLsizeiptr dataSize, GLsizei count, bool hasColor = false);
    ~Model();

    void draw() const;
    GLuint getVAO() const;
    GLsizei getVertexCount() const;
};