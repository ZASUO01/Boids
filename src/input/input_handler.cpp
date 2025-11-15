#include "input/input_handler.h"
#include "core/program.h"

InputHandler::InputHandler() {}

float InputHandler::random_float(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

void InputHandler::SpawnBoid(Program *program) {

    float randX = random_float(-50.0f, 50.0f);
    float randY = random_float(-50.0f, 50.0f);
    Vec3 pos = vec3_create(randX, randY, 5.0f);

    auto boid = std::make_unique<Boid>(pos);
    program->m_objects.push_back(std::move(boid));
}

void InputHandler::RemoveBoid(Program *program) {

    for (auto it = program->m_objects.rbegin(); it != program->m_objects.rend(); ++it) {
        if (dynamic_cast<Boid *>(it->get()) != nullptr) {
            program->m_objects.erase(std::next(it).base());
            break;
        }
    }
}

void InputHandler::ProcessKey(Program *program, int key, int action) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }

    switch (key) {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(program->m_window, GLFW_TRUE);
        break;
    case GLFW_KEY_EQUAL:
    case GLFW_KEY_KP_ADD:
        SpawnBoid(program);
        break;
    case GLFW_KEY_MINUS:
    case GLFW_KEY_KP_SUBTRACT:
        RemoveBoid(program);
        break;
    case GLFW_KEY_C:
        program->m_camera.toggleMode();
        break;
    default:
        break;
    }
}

void InputHandler::ProcessMouse(int button, int action) {
    // (Lógica do mouse aqui)
}

void InputHandler::ProcessContinuousInput(GLFWwindow *window, float delta_time, Object *player) {
    if (!player) {
        return;
    }

   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player->accelerate(1.0f, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player->accelerate(-1.0f, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player->turn(1.0f, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player->turn(-1.0f, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        player->pitch(1.0f, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        player->pitch(-1.0f, delta_time);
    }
}