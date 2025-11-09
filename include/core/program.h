#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <memory>
#include <vector>
#include <random>

#include "objects/cube.h"
#include "objects/ground.h"
#include "objects/object.h"
#include "objects/tower.h"
#include "utils/logger.h"

#define MAX_TITLE_LENGTH 256
#define FPS_UPDATE_INTERVAL 1.0

class Program {
public:
    Program();
    void Init(const char *name);
    void Shutdown();
    void RunLoop();

    void PrintStats() const;

    static const int WINDOW_WIDTH = 1024;
    static const int WINDOW_HEIGHT = 768;

private:
    bool isRunning;
    double lastFrameTime;
    double m_lastFPSTime;  // Armazena o tempo da última atualização de FPS
    int m_frameCount;
    GLFWwindow *window;
    std::vector<std::unique_ptr<Object>> m_objects;

    void Input();
    void Update();
    void Render();

    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void ProcessKey(int key, int action);

    void setNameWithFPS(double currentTime);
};