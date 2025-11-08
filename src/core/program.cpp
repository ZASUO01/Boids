#include "core/program.h"

Program::Program()
:isRunning(true)
,window(nullptr)
{}

void Program::Init(const char *name){
    if (!glfwInit()) {
       Logger::logExit("glfw init failure");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(Program::WINDOW_WIDTH, Program::WINDOW_HEIGHT, name, NULL, NULL);
    if (!window) {
        glfwTerminate();
        Logger::logExit("glfw create window failure");
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        Logger::logExit("glew init failure");
    }

}   


void Program::RunLoop(){
    while(isRunning){

    }
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