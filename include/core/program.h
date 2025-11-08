#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <cstdio>

#include "utils/logger.h"


class Program{
public:
    Program();
    void Init(const char *name);
    void Shutdown();
    void RunLoop();

    void PrintStats() const ;

    static const int WINDOW_WIDTH = 1024;
    static const int WINDOW_HEIGHT = 768;
private:
    bool isRunning;    
    GLFWwindow* window;
};