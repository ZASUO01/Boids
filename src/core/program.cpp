#include "core/program.h"

Program::Program()
    : isRunning(true),
      lastFrameTime(0),
      m_lastFPSTime(0.0),
      m_frameCount(0),
      window(nullptr) {}

void Program::Init(const char *name) {
    if (!glfwInit()) {
        Logger::logExit("glfw init failure");
    }

    window = glfwCreateWindow(Program::WINDOW_WIDTH, Program::WINDOW_HEIGHT, name, NULL, NULL);
    if (!window) {
        glfwTerminate();
        Logger::logExit("glfw create window failure");
    }

    glfwMakeContextCurrent(window);
    glViewport(0, 0, Program::WINDOW_WIDTH, Program::WINDOW_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    m_objects.push_back(
        std::make_unique<Ground>(vec3_create(0, 0, 0)) // Posição na origem
    );

    m_objects.push_back(
        std::make_unique<Tower>(vec3_create(0, 0, 0)) // Posição na origem
    );

    auto testCube1 = std::make_unique<Cube>(vec3_create(5, 5, 0.5));
    testCube1->setRotationAxis(vec3_create(0.0f, 0.0f, 1.0f));
    testCube1->setRotationSpeed(45.0f);
    m_objects.push_back(std::move(testCube1));

    lastFrameTime = glfwGetTime();
    m_lastFPSTime = lastFrameTime;
}

void Program::RunLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        Input();
        Update();
        Render();
    }
}

void Program::Input() {
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
}

void Program::setNameWithFPS(double currentTime) {
    m_frameCount++;

    if (currentTime - m_lastFPSTime >= FPS_UPDATE_INTERVAL) {
        char title[MAX_TITLE_LENGTH];
        
        snprintf(title, MAX_TITLE_LENGTH, "Boids | FPS: %d", m_frameCount);
        glfwSetWindowTitle(window, title);

        m_frameCount = 0;
        m_lastFPSTime = currentTime;
    }
}

void Program::Update() {

    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - lastFrameTime);

    setNameWithFPS(currentTime);

    for (const auto &obj : m_objects) {
        obj->update(deltaTime);
    }

    lastFrameTime = currentTime;
}

void Program::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    if (width == 0 || height == 0)
        return;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0f, (float)width / (float)height, 0.1f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        20.0f, 20.0f, 20.0f,
        0.0f, 0.0f, 5.0f,
        0.0f, 0.0f, 1.0f);

    for (const auto &obj : m_objects) {
        obj->render();
    }

    glfwSwapBuffers(window);
}

void Program::Shutdown() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Program::PrintStats() const {
    printf("Window width: %d\n", Program::WINDOW_WIDTH);
    printf("Window height: %d\n", Program::WINDOW_HEIGHT);
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
}

void Program::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    Program *prog = static_cast<Program *>(glfwGetWindowUserPointer(window));
    if (prog) {
        prog->ProcessKey(key, action);
    }
}

void Program::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    Program *prog = static_cast<Program *>(glfwGetWindowUserPointer(window));
    if (prog) {
        prog->ProcessKey(button, action);
    }
}

static float random_float(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

void Program::ProcessKey(int key, int action) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }

    switch (key) {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;

    case GLFW_KEY_EQUAL:
    case GLFW_KEY_KP_ADD: {

        float randX = random_float(-20.0f, 20.0f);
        float randY = random_float(-20.0f, 20.0f);
        Vec3 pos = vec3_create(randX, randY, 0.5f);

        Vec3 axis = vec3_create(
            random_float(-1.0f, 1.0f),
            random_float(-1.0f, 1.0f),
            random_float(-1.0f, 1.0f));
        axis = vec3_normalize(axis);

        float speed = random_float(20.0f, 120.0f);

        auto newCube = std::make_unique<Cube>(pos);
        newCube->setRotationAxis(axis);
        newCube->setRotationSpeed(speed);

        m_objects.push_back(std::move(newCube));
        break;
    }

    case GLFW_KEY_MINUS:
    case GLFW_KEY_KP_SUBTRACT: {

        for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it) {

            Cube *cube_ptr = dynamic_cast<Cube *>(it->get());

            if (cube_ptr != nullptr) {
                m_objects.erase(std::next(it).base());
                break;
            }
        }
        break;
    }
    default:
        break;
    }
}