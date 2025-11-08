#include "core/program.h"
#include <GL/glew.h>    // GLEW (tem que vir ANTES do glfw3.h)
#include <GL/glu.h>     // GLU (para gluPerspective e gluLookAt)
#include <GLFW/glfw3.h> // GLFW
#include <stdio.h>      // Para fputs
#include <stdlib.h>     // Para exit()

// Nossos próprios headers
#include "graphics/render.h"

/**
 * @brief Configura a matriz de Projeção (perspectiva) e 
 * a matriz de View (câmera).
 */
void setup_camera_and_projection(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    // --- 1. Configurar Projeção (Perspective) ---
    glMatrixMode(GL_PROJECTION); // Mudar para a matriz de projeção
    glLoadIdentity();            // Resetar
    
    // (Campo de visão Y, Aspect Ratio, Distância Mínima, Distância Máxima)
    gluPerspective(45.0f, aspect, 0.1f, 500.0f);

    // --- 2. Configurar Câmera (View) ---
    glMatrixMode(GL_MODELVIEW); // Mudar de volta para a matriz de visão/modelo
    glLoadIdentity();           // Resetar

    // (Posição do Olho, Ponto para onde olha, Vetor "Up")
    // Esta é uma câmera estática temporária olhando para a origem
    gluLookAt(
        20.0f, 25.0f, 20.0f,  // Posição do olho (longe e acima)
         0.0f,  5.0f,  0.0f,  // Ponto para onde olha (o centro, um pouco acima do chão)
         0.0f,  1.0f,  0.0f   // Vetor "Up" (Y positivo é para cima)
    );
}

int main(void) {
    Program program;
    
    program.Init("Boids");
    program.PrintStats();

    program.RunLoop();

    program.Shutdown();

    return 0;
}