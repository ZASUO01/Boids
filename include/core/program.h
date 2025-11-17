#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <memory>
#include <random>
#include <vector>

#include "core/camera.h"
#include "input/input_handler.h"
#include "objects/boid.h"
#include "objects/cube.h"
#include "objects/ground.h"
#include "objects/object.h"
#include "objects/tower.h"
#include "utils/logger.h"

enum class CameraMode {
    STATIC_WORLD,
    FOLLOW_PLAYER
};

enum class ProgramState {
    UPDATING,
    PAUSED
};

class Program {
public:
    Program();
    void Init(const char *name);
    void Shutdown();
    void RunLoop();

    void PrintStats() const;
    void ToggleFog();

    static const int WINDOW_WIDTH = 1024;
    static const int WINDOW_HEIGHT = 768;
    static const int MAX_TITLE_LENGTH = 256;
    static constexpr double FPS_UPDATE_INTERVAL = 1.0;

    void ToggleState();

private:
    GLFWwindow *m_window;
    Camera m_camera;
    InputHandler m_inputHandler;
    std::vector<std::unique_ptr<Object>> m_objects;

    Object *m_playerBoid;
    bool m_isRunning;
    bool m_fogEnabled;
    int m_frameCount;
    double m_lastFrameTime;
    double m_lastFPSTime;

    void InitWindow(const char *name);
    void InitOpenGL();
    void InitInput();
    void SetupScene();
    void InitTimers();

    void Input();
    void Update();
    void Render();

    void SpawnObstacles();
    void DrawCircleWaves(float radius, int num, float scale);

    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

    void setNameWithFPS(double currentTime);

    friend class InputHandler;

    ProgramState m_state;
};