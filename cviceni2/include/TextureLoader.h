/**
 * @file TextureLoader.h
 * @author Student (login: ZHA0067)
 * @brief Helper functions for loading 2D textures and cubemaps via stb_image.
 */
#pragma once

#include <vector>
#include <glad/gl.h>

GLuint loadTexture(const char* filePath);
GLuint loadCubemap(const std::vector<const char*>& faces);