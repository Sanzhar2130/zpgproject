/**
 * @file ModelFactory.h
 * @author Student (login: ZHA0067)
 * @brief Factory for creating scene and prop 3D models.
 */
#pragma once

#include <vector>
#include "Model.h"

class ModelFactory {
public:
    static std::vector<Model*> createDefaultModels();
};