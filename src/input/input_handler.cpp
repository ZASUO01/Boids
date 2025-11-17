#include "input/input_handler.h"
#include "core/program.h"

static int random_int(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

InputHandler::InputHandler() {}

float InputHandler::random_float(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

void InputHandler::SpawnBoid(Program *program) {

    if (program->m_playerBoid == nullptr) {
        Vec3 pos = vec3_create(0.0f, 0.0f, 5.0f);
        auto boid_fallback = std::make_unique<Boid>(pos);
        program->m_objects.push_back(std::move(boid_fallback));
        return;
    }

    Vec3 player_pos = program->m_playerBoid->getPosition();

    float spawn_radius = 5.0f;
    float randX = random_float(-spawn_radius, spawn_radius);
    float randY = random_float(-spawn_radius, spawn_radius);
    float randZ = random_float(-2.0f, 2.0f);

    Vec3 pos = vec3_add(player_pos, vec3_create(randX, randY, randZ));
    auto boid = std::make_unique<Boid>(pos);

    boid->setForward(program->m_playerBoid->getForward());
    program->m_objects.push_back(std::move(boid));
}

void InputHandler::RemoveBoid(Program *program) {

    std::vector<std::vector<std::unique_ptr<Object>>::iterator> targets;

    for (auto it = program->m_objects.begin(); it != program->m_objects.end(); ++it) {

        Boid *boid_ptr = dynamic_cast<Boid *>(it->get());
        if (boid_ptr != nullptr && boid_ptr != program->m_playerBoid) {
            targets.push_back(it);
        }
    }

    if (targets.empty()) {
        return;
    }

    int random_index = random_int(0, static_cast<int>(targets.size() - 1));

    auto iterator_to_remove = targets[random_index];
    program->m_objects.erase(iterator_to_remove);
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
    case GLFW_KEY_P:
        program->ToggleState();
        break;
    case GLFW_KEY_F:
        program->ToggleFog();
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
