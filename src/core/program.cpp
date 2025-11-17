#include "core/program.h"

Program::Program()
    : m_window(nullptr),
      m_camera(),
      m_inputHandler(),
      m_objects(),
      m_playerBoid(nullptr),
      m_isRunning(true),
      m_fogEnabled(false),
      m_frameCount(0),
      m_lastFrameTime(0.0),
      m_lastFPSTime(0.0),
      m_state(ProgramState::UPDATING) {}

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
    GLfloat global_ambient[] = {0.0f, 0.0f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat fogColor[4] = {0.0f, 0.0f, 0.2f, 1.0f};
    
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogfv(GL_FOG_COLOR, fogColor); 
    glFogf(GL_FOG_DENSITY, 0.01f);
    glHint(GL_FOG_HINT, GL_NICEST);

    glEnable(GL_NORMALIZE);
}

void Program::InitInput() {
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, KeyCallback);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
}

void Program::SetupScene() {
    m_objects.push_back(std::make_unique<Ground>(vec3_create(0.0f, 0.0f, 0.0f)));
    m_objects.push_back(std::make_unique<Tower>(vec3_create(0.0f, 0.0f, 0.0f)));

    SpawnObstacles();

    auto player = std::make_unique<Boid>(vec3_create(-10, -10, 5));
    player->m_isPlayer = true;
    m_playerBoid = player.get();

    m_camera.setFollowTarget(m_playerBoid);
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
    

    if(m_state != ProgramState::PAUSED){
        m_inputHandler.ProcessContinuousInput(m_window, deltaTime, m_playerBoid);

        for (const auto &obj : m_objects) {
            obj->update(deltaTime, m_objects, m_playerBoid);
        }
    }
 
    m_lastFrameTime = currentTime;
}
void Program::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    if (width == 0 || height == 0)
        return;

    // --- 1. PROJEÇÃO ---
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0f, (float)width / (float)height, 0.1f, 500.0f);

    // --- 2. CÂMERA (VIEW) ---
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    m_camera.setupViewMatrix(); // Define o gluLookAt

    // --- 3. NÉVOA (FOG) ---
    // (MOVIDO PARA DEPOIS DA CÂMERA)
    if (m_fogEnabled) {
        glEnable(GL_FOG);
    } else {
        glDisable(GL_FOG);
    }

    // --- 4. LUZES ---
    // (DEVE VIR DEPOIS DA CÂMERA, PARA FICAR NO "WORLD SPACE")
    GLfloat light_position[] = {1.0f, 1.0f, 1.0f, 0.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glEnable(GL_LIGHT0);

    // --- 5. OBJETOS ---
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
    printf("GLFW Version: %s\n", glfwGetVersionString());
}

void Program::ToggleFog() {
    m_fogEnabled = !m_fogEnabled;
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


void Program::SpawnObstacles() {
    DrawCircleWaves(20.0f, 4, 5.0f);    
    DrawCircleWaves(50.0f, 8, 10.0f);
    DrawCircleWaves(80.0f, 12, 15.0f);
}

static int random_int(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

void Program::DrawCircleWaves(float radius, int num, float scale){
    const float step =  2 * (float)M_PI / (float)num;
    float degrees = 0;

    float posX = 0.0f;
    float posY = 0.0f;
    float posZ = 0.0f;

    Vec3 rotationAxis = vec3_create(0.0f, 0.0f, 1.0f);
    
    for(int i = 0; i < num; i++){
        posX = cos(degrees) * radius;
        posY = sin(degrees) * radius;
    
        for(int j = 0; j < 2; j++){
            float speed = static_cast<float>(random_int(-60, 60));
            posZ = (0.5f * scale) + ((float)j * scale);
            Vec3 pos = vec3_create(posX, posY, posZ);

            auto cube = std::make_unique<Cube>(pos);

            if(j > 0){
                cube->setRotationAxis(rotationAxis);
                cube->setRotationSpeed(speed);
            }
            
            cube->setScale(scale);
            m_objects.push_back(std::move(cube));
        }
        
        degrees += step;
    }
}

void Program::ToggleState(){
    switch (m_state) {
    case ProgramState::UPDATING:
        m_state = ProgramState::PAUSED;
        break;
    case ProgramState::PAUSED:
        m_state = ProgramState::UPDATING;
        break;
    default:
        break;
    }
}