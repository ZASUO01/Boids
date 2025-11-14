#include "objects/boid.h"

Boid::Boid(Vec3 pos) : Object(pos), 
                       m_wing_phase(0.0f),
                       m_wing_speed(8.0f),
                       m_wing_amplitude(40.0f),
                       m_current_wing_angle(0.0f),
                       m_bobbing_phase(0.0f),
                       m_bobbing_speed(3.0f),
                       m_bobbing_amplitude(0.2f) {
     
    m_speed = 10.0f; 
    m_scale = 0.5f; 
}

void Boid::update(float delta_time) {
    Object::update(delta_time);

    m_wing_phase += m_wing_speed * delta_time;
    if (m_wing_phase > (GLfloat) (2.0f * M_PI)) {
        m_wing_phase -= (GLfloat) (2.0f * M_PI);
    }

    m_current_wing_angle = sin(m_wing_phase) * m_wing_amplitude;

    m_bobbing_phase += m_bobbing_speed * delta_time;
    if (m_bobbing_phase > (GLfloat) (2.0f * M_PI)) {
        m_bobbing_phase -= (GLfloat) (2.0f * M_PI);
    }

    m_position_offset.z = sin(m_bobbing_phase) * m_bobbing_amplitude;
}

// Função auxiliar: Desenha uma pirâmide de base quadrada
// Base no plano XY (centrada em 0,0), Ponta em +Z
void Boid::drawUnitPyramid() const {
    glBegin(GL_TRIANGLES);
        // Frente
        glNormal3f(0.0f, 0.5f, 0.5f);
        glVertex3f( 0.0f,  0.0f,  1.0f); // Topo
        glVertex3f(-0.5f, -0.5f,  0.0f); // Base Esq
        glVertex3f( 0.5f, -0.5f,  0.0f); // Base Dir

        // Direita
        glNormal3f(0.5f, 0.0f, 0.5f);
        glVertex3f( 0.0f,  0.0f,  1.0f);
        glVertex3f( 0.5f, -0.5f,  0.0f);
        glVertex3f( 0.5f,  0.5f,  0.0f);

        // Trás
        glNormal3f(0.0f, -0.5f, 0.5f);
        glVertex3f( 0.0f,  0.0f,  1.0f);
        glVertex3f( 0.5f,  0.5f,  0.0f);
        glVertex3f(-0.5f,  0.5f,  0.0f);

        // Esquerda
        glNormal3f(-0.5f, 0.0f, 0.5f);
        glVertex3f( 0.0f,  0.0f,  1.0f);
        glVertex3f(-0.5f,  0.5f,  0.0f);
        glVertex3f(-0.5f, -0.5f,  0.0f);
    glEnd();

    // Base (Quadrada)
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-0.5f, -0.5f, 0.0f);
        glVertex3f( 0.5f, -0.5f, 0.0f);
        glVertex3f( 0.5f,  0.5f, 0.0f);
        glVertex3f(-0.5f,  0.5f, 0.0f);
    glEnd();
}

void Boid::draw() const {
    
    // TRONCO
    glColor3f(0.2f, 0.5f, 0.8f); // Azul
    glPushMatrix();
        glScalef(0.6f, 1.5f, 0.6f); 
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 

        drawUnitPyramid();
    glPopMatrix();

    // CABEÇA
    glColor3f(1.0f, 0.8f, 0.2f); // Amarelo
    glPushMatrix();
        glTranslatef(0.0f, 0.75f, 0.0f);
        glScalef(0.4f, 0.6f, 0.4f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        drawUnitPyramid();
    glPopMatrix();

    // BRAÇO ESQUERDO
    glColor3f(0.4f, 0.7f, 1.0f);
    glPushMatrix();
        glTranslatef(-0.3f, 0.2f, 0.1f);
        glRotatef(m_current_wing_angle, 0.0f, 1.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, 0.5f, 0.0f);
        glScalef(0.3f, 1.2f, 0.1f);

        drawUnitPyramid();
    glPopMatrix();

    // BRAÇO DIREITO
    glPushMatrix();
        glTranslatef(0.3f, 0.2f, 0.1f);
        glRotatef(-m_current_wing_angle, 0.0f, 1.0f, 0.0f);
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, 0.5f, 0.0f);
        glScalef(0.3f, 1.2f, 0.1f);

        drawUnitPyramid();
    glPopMatrix();

    // RABO
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, 0.0f, 1.0f); 
        glVertex3f( 0.0f, -0.2f, 0.0f);
        glVertex3f(-0.4f, -1.0f, 0.0f); 
        glVertex3f( 0.4f, -1.0f, 0.0f);
    glEnd();
}