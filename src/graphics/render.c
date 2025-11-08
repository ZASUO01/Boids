#include "graphics/render.h"

// Desenha um plano horizontal (chão)
void draw_ground(float size, float y_pos) {
    float half_size = size / 2.0f;
    
    // Define uma cor cinza para o chão
    glColor3f(0.5f, 0.5f, 0.5f);

    // Usamos GL_QUADS para desenhar um quadrado
    glBegin(GL_QUADS);
        // Normal para iluminação (aponta para cima, +Y)
        glNormal3f(0.0f, 1.0f, 0.0f);
        
        // Vértices
        glVertex3f(-half_size, y_pos,  half_size);
        glVertex3f( half_size, y_pos,  half_size);
        glVertex3f( half_size, y_pos, -half_size);
        glVertex3f(-half_size, y_pos, -half_size);
    glEnd();
}

// Desenha um cone (torre)
void draw_tower(float base_radius, float height, int slices, float y_pos) {
    float angle_step = (2.0f * (float) M_PI) / (float)slices;

    // Define uma cor (ex: vermelho) para a torre
    glColor3f(0.8f, 0.2f, 0.2f);

    // --- 1. Desenha a base do cone (um círculo) ---
    glBegin(GL_TRIANGLE_FAN);
        // Normal aponta para baixo (-Y)
        glNormal3f(0.0f, -1.0f, 0.0f);
        // Vértice central
        glVertex3f(0.0f, y_pos, 0.0f);
        // Vértices da borda
        for (int i = 0; i <= slices; i++) {
            float angle = (float) i * angle_step;
            float x = base_radius * (float) cos(angle);
            float z = base_radius * (float) sin(angle);
            glVertex3f(x, y_pos, z);
        }
    glEnd();

    // --- 2. Desenha os lados do cone ---
    glBegin(GL_TRIANGLES);
        // Vértice do topo
        Vec3 top = {0.0f, y_pos + height, 0.0f};

        for (int i = 0; i < slices; i++) {
            float angle1 = (float) i * angle_step;
            float x1 = base_radius * (float) cos(angle1);
            float z1 = base_radius * (float) sin(angle1);
            Vec3 v1 = {x1, y_pos, z1};

            float angle2 = (float) (i + 1) * angle_step;
            float x2 = base_radius * (float) cos(angle2);
            float z2 = base_radius * (float) sin(angle2);
            Vec3 v2 = {x2, y_pos, z2};

            // NOTA: O cálculo da normal aqui está simplificado
            // Uma normal correta por face seria mais complexa (cross product)
            // Mas para o modelo de iluminação padrão, uma normal "média" funciona
            // (apontando para fora do centro)
            
            // Triângulo 1 (lado)
            glNormal3f(x1, 0.2f, z1); // Normal simplificada
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(top.x, top.y, top.z);
        }
    glEnd();
}