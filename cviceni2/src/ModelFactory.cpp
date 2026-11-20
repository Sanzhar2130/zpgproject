/**
 * @file ModelFactory.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of ModelFactory creating all project models.
 */
#include "ModelFactory.h"

#include "../models/sun.h"
#include "../models/earth.h"
#include "../models/moon.h"
#include "../models/sphere.h"
#include "../models/plain.h"
#include "../models/tree.h"
#include "../models/bushes.h"
#include "../models/gift.h"
#include "../models/zha0067.h"
#include "../models/clock.h"
#include "../models/rocket.h"
#include "../models/Shrek.h"
#include "../models/Fiona.h"
#include "../models/Skybox.h"
#include "../models/EB408.h"
#include "../models/door.h"
#include "../models/door_frame.h"
#include "../models/whiteboard.h"
#include "../models/table.h"
#include "../models/orange_chair.h"

static const float kTriangleVertices[] = {
     0.0f,  0.55f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.55f, -0.45f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.55f, -0.45f, 0.0f,  0.0f, 0.0f, 1.0f
};

std::vector<Model*> ModelFactory::createDefaultModels() {
    std::vector<Model*> models;
    models.reserve(22);

    models.push_back(new Model(kTriangleVertices, sizeof(kTriangleVertices), 3));
    models.push_back(new Model(sphere, sizeof(sphere), 2880));
    models.push_back(new Model(plain, sizeof(plain), 6, false, true));
    models.push_back(new Model(tree, sizeof(tree), 92814));
    models.push_back(new Model(bushes, sizeof(bushes), 8730));
    models.push_back(new Model(gift, sizeof(gift), 66624));
    models.push_back(new Model(zha0067, sizeof(zha0067), 1776));

    models.push_back(new Model(sun, sizeof(sun), 11520, true));
    models.push_back(new Model(earth, sizeof(earth), 11520, true));
    models.push_back(new Model(moon, sizeof(moon), 11520, true));

    models.push_back(new Model(clockModel, sizeof(clockModel), static_cast<GLsizei>(vertexCount), true));
    models.push_back(new Model(rocketModel, sizeof(rocketModel), static_cast<GLsizei>(vertexRocketCount), true));

    models.push_back(new Model(plain, sizeof(plain), 6, false, true));

    GLsizei shrekVerticesCount = static_cast<GLsizei>(sizeof(ShrekModel) / (8 * sizeof(float)));
    models.push_back(new Model(ShrekModel, sizeof(ShrekModel), shrekVerticesCount, false, true));

    GLsizei fionaVerticesCount = static_cast<GLsizei>(sizeof(FionaModel) / (8 * sizeof(float)));
    models.push_back(new Model(FionaModel, sizeof(FionaModel), fionaVerticesCount, false, true));

    models.push_back(new Model(CubeModel, sizeof(CubeModel), CubeVertexCount, false, true));

    models.push_back(new Model(SM_EB408Model, sizeof(SM_EB408Model), SM_EB408VertexCount, false, true));
    models.push_back(new Model(SM_DoorFrame_vertices, sizeof(SM_DoorFrame_vertices), SM_DoorFrame_vertex_count, false, true));
    models.push_back(new Model(SM_Door_vertices, sizeof(SM_Door_vertices), SM_Door_vertex_count, false, true));
    models.push_back(new Model(SM_Whiteboard_vertices, sizeof(SM_Whiteboard_vertices), SM_Whiteboard_vertex_count, false, true));
    models.push_back(new Model(SM_Table_vertices, sizeof(SM_Table_vertices), SM_Table_vertex_count, false, true));
    models.push_back(new Model(OrangeChair_vertices, sizeof(OrangeChair_vertices), OrangeChair_vertex_count, false, true));

    return models;
}