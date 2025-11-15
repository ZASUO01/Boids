#pragma once

#include <memory>
#include <random>
#include <vector>

#include "core/camera.h"
#include "objects/boid.h"
#include "utils/vmath.h"

#include <GLFW/glfw3.h>

// Evitar dependências circulares
class Program;

class InputHandler {
public:
    InputHandler();

    void ProcessKey(Program *program, int key, int action);
    void ProcessMouse(int button, int action);
    void ProcessContinuousInput(GLFWwindow *window, float delta_time, Object *player);

private:
    void SpawnBoid(Program *program);
    void RemoveBoid(Program *program);
    float random_float(float min, float max);
};