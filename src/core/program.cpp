#include "core/program.h"

void drawCube()
{
    glBegin(GL_QUADS);

    // Frente (azul)
    glColor3f(0.2f, 0.4f, 1.0f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);

    // Trás (vermelho)
    glColor3f(1.0f, 0.3f, 0.3f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);

    // Esquerda (verde)
    glColor3f(0.3f, 1.0f, 0.3f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);

    // Direita (amarelo)
    glColor3f(1.0f, 1.0f, 0.3f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);

    // Topo (roxo)
    glColor3f(0.8f, 0.3f, 1.0f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);

    // Base (ciano)
    glColor3f(0.3f, 1.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);

    glEnd();
}

Program::Program()
:isRunning(true)
,lastFrameTime(0)
,window(nullptr)
{}

void Program::Init(const char *name){
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

    // renderer
    lastFrameTime = glfwGetTime();
}   


void Program::RunLoop(){   
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        Input();
        Update();
        Render();
   }
}

void Program::Input(){

}

void Program::Update(){
    while (glfwGetTime() < lastFrameTime + 0.016){}

    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - lastFrameTime);

    if (deltaTime > 0.05f){
        deltaTime = 0.05f;
    }

    lastFrameTime = currentTime;
}

void Program::Render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);

        // --- Configurar View ---
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // "camera" em (3,3,3), olhando para (0,0,0)
    gluLookAt(3, 3, 3,
            0, 0, 0,
            0, 1, 0);

     // --- Transformação Model ---
    static float angle = 0.0f;
    angle += 0.5f;
    glRotatef(angle, 1.0f, 1.0f, 1.0f);

    // desenhar o cubo
    drawCube();


    glfwSwapBuffers(window);
}


void Program::Shutdown(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void  Program::PrintStats() const {
    printf("Window width: %d\n", Program::WINDOW_WIDTH);
     printf("Window height: %d\n", Program::WINDOW_HEIGHT);
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
}