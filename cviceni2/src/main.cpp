/**
 * @file main.cpp
 * @brief Main entry point of the ZPG OpenGL application.
 * @author Student (login: ZHA0067)
 */
#include "Application.h"

int main(void)
{
    Application* app = new Application();
    
    app->initialization();
    app->createShaders();
    app->createModels();
    app->createScenes();
    app->run();

    delete app;
    return 0;
}