#include "objects/cube.h"
#include <GL/glew.h>

void Cube::update(float delta_time, const std::vector<std::unique_ptr<Object>>& all_objects, Object* player_boid) {
    m_rotation_angle += m_rotation_speed * delta_time;
    
    if (m_rotation_angle > 360.0f) {
        m_rotation_angle -= 360.0f;
    }
}

void Cube::draw() const {
    glBegin(GL_QUADS);

    // Frente (azul) - Z = +0.5
    glColor3f(0.2f, 0.4f, 1.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Trás (vermelho) - Z = -0.5
    glColor3f(1.0f, 0.3f, 0.3f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Esquerda (verde) - X = -0.5
    glColor3f(0.3f, 1.0f, 0.3f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Direita (amarelo) - X = +0.5
    glColor3f(1.0f, 1.0f, 0.3f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Topo (roxo) - Y = +0.5
    glColor3f(0.8f, 0.3f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Base (ciano) - Y = -0.5
    glColor3f(0.3f, 1.0f, 1.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glEnd();
}