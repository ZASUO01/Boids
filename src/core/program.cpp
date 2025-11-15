#include "core/program.h"

Program::Program()
    : m_window(nullptr),
      m_camera(),
      m_inputHandler(),
      m_objects(),
      m_playerBoid(nullptr),
      m_isRunning(true),
      m_frameCount(0),
      m_lastFrameTime(0.0),
      m_lastFPSTime(0.0) {}

void Program::InitWindow(const char *name) {
    if (!glfwInit()) {
        Logger::logExit("glfw init failure");
    }

    m_window = glfwCreateWindow(Program::WINDOW_WIDTH, Program::WINDOW_HEIGHT, name, NULL, NULL);
    if (!m_window) {
        glfwTerminate();
        Logger::logExit("glfw create window failure");
    }

    glfwMakeContextCurrent(m_window);
}

void Program::InitOpenGL() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        Logger::logExit("Failed to initialize GLEW");
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    glEnable(GL_LIGHTING);
    GLfloat global_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_NORMALIZE);
}

void Program::InitInput() {
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, KeyCallback);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
}

void Program::SetupScene() {
    m_objects.push_back(std::make_unique<Ground>(vec3_create(0.0f, 0.0f, 0.0f)));
    m_objects.push_back(std::make_unique<Tower>(vec3_create(0.0f, 0.0f, 1.0f)));

    const float scale = 7.0f;
    const float cubeY = -30.0f;
    const Vec3 rotAxis = vec3_create(0.0f, 0.0f, 1.0f);

    std::vector<float> speeds = {45.0f, -60.0f, 30.0f};

    for (size_t i = 0; i < speeds.size(); ++i) {
        float zPos = (0.5f * scale) + ((float)i * scale);
        Vec3 pos = vec3_create(0.0f, cubeY, zPos);

        auto cube = std::make_unique<Cube>(pos);
        cube->setRotationAxis(rotAxis);
        cube->setRotationSpeed(speeds[i]);
        cube->setScale(scale);
        m_objects.push_back(std::move(cube));
    }

    auto player = std::make_unique<Boid>(vec3_create(0, 0, 5));
    m_playerBoid = player.get();
    m_camera.setFollowTarget(player.get());
    m_objects.push_back(std::move(player));
}

void Program::InitTimers() {
    m_lastFrameTime = glfwGetTime();
    m_lastFPSTime = m_lastFrameTime;
}

void Program::Init(const char *name) {
    InitWindow(name);
    InitOpenGL();
    InitInput();
    SetupScene();
    InitTimers();
}

void Program::RunLoop() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        Update();
        Render();
    }
}

void Program::Input() {
    glfwSetWindowUserPointer(m_window, this);

    glfwSetKeyCallback(m_window, KeyCallback);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
}

void Program::setNameWithFPS(double currentTime) {
    m_frameCount++;

    if (currentTime - m_lastFPSTime >= FPS_UPDATE_INTERVAL) {
        char title[MAX_TITLE_LENGTH];

        snprintf(title, MAX_TITLE_LENGTH, "Boids | FPS: %d", m_frameCount);
        glfwSetWindowTitle(m_window, title);

        m_frameCount = 0;
        m_lastFPSTime = currentTime;
    }
}

void Program::Update() {

    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - m_lastFrameTime);

    setNameWithFPS(currentTime);
    m_inputHandler.ProcessContinuousInput(m_window, deltaTime, m_playerBoid);

    for (const auto &obj : m_objects) {
        obj->update(deltaTime);
    }

    m_lastFrameTime = currentTime;
}

void Program::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    if (width == 0 || height == 0)
        return;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0f, (float)width / (float)height, 0.1f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    m_camera.setupViewMatrix();

    GLfloat light_position[] = {1.0f, 1.0f, 1.0f, 0.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glEnable(GL_LIGHT0);

    for (const auto &obj : m_objects) {
        obj->render();
    }

    glfwSwapBuffers(m_window);
}

void Program::Shutdown() {
    glfwDestroyWindow(m_window);
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
        prog->m_inputHandler.ProcessKey(prog, key, action);
    }
}

void Program::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    Program *prog = static_cast<Program *>(glfwGetWindowUserPointer(window));
    if (prog) {
        prog->m_inputHandler.ProcessMouse(button, action);
    }
}
