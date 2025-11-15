#include "objects/boid.h"

#include <random>

static float random_float(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

Boid::Boid(Vec3 pos) : Object(pos),
                       m_wing_phase(0.0f),
                       m_wing_speed(8.0f),
                       m_wing_amplitude(40.0f),
                       m_current_wing_angle(0.0f),
                       m_bobbing_amplitude(0.2f) {

    m_speed = 10.0f;
    m_scale = 0.5f;
    m_accel_amount = 25.0f;
    m_turn_speed = 90.0f;
    m_pitch_speed = 7.5f;

    m_bobbing_phases = vec3_create(
        random_float(0.0f, 2.0f * (float)M_PI),
        random_float(0.0f, 2.0f * (float)M_PI),
        random_float(0.0f, 2.0f * (float)M_PI));

    m_bobbing_speeds = vec3_create(
        random_float(1.5f, 3.0f),
        random_float(1.5f, 3.0f),
        random_float(1.5f, 3.0f));
}

void Boid::accelerate(float direction, float delta_time) {
    m_speed += m_accel_amount * direction * delta_time;

    if (m_speed > Boid::MAX_SPEED)
        m_speed = Boid::MAX_SPEED;

    if (m_speed < Boid::MIN_SPEED)
        m_speed = Boid::MIN_SPEED;
}

void Boid::turn(float direction, float delta_time) {
    float degrees = m_turn_speed * direction;
    float rad = degrees * delta_time * ((float)M_PI / 180.0f);
    float c = cos(rad);
    float s = sin(rad);

    Vec3 f = m_forward;
    Vec3 r = m_right;

    m_forward.x = f.x * c - f.y * s;
    m_forward.y = f.x * s + f.y * c;

    m_right.x = r.x * c - r.y * s;
    m_right.y = r.x * s + r.y * c;
}

void Boid::pitch(float direction, float delta_time) {
    m_position.z += m_pitch_speed * direction * delta_time;
}

void Boid::update(float delta_time) {
    m_position = vec3_add(m_position, vec3_scale(m_forward, m_speed * delta_time));

    m_rotation_angle += m_rotation_speed * delta_time;
    if (m_rotation_angle > 360.0f) {
        m_rotation_angle -= 360.0f;
    }

    // Lógica das asas
    m_wing_phase += m_wing_speed * delta_time;
    if (m_wing_phase > (GLfloat)(2.0f * M_PI)) {
        m_wing_phase -= (GLfloat)(2.0f * M_PI);
    }
    m_current_wing_angle = sin(m_wing_phase) * m_wing_amplitude;

    // Lógica da flutuação
    m_bobbing_phases.x += m_bobbing_speeds.x * delta_time;
    m_bobbing_phases.y += m_bobbing_speeds.y * delta_time;
    m_bobbing_phases.z += m_bobbing_speeds.z * delta_time;

    if (m_bobbing_phases.x > (GLfloat)(2.0f * M_PI))
        m_bobbing_phases.x -= (GLfloat)(2.0f * M_PI);

    if (m_bobbing_phases.y > (GLfloat)(2.0f * M_PI))
        m_bobbing_phases.y -= (GLfloat)(2.0f * M_PI);

    if (m_bobbing_phases.z > (GLfloat)(2.0f * M_PI))
        m_bobbing_phases.z -= (GLfloat)(2.0f * M_PI);

    m_position_offset.x = sin(m_bobbing_phases.x) * m_bobbing_amplitude;
    m_position_offset.y = sin(m_bobbing_phases.y) * m_bobbing_amplitude;
    m_position_offset.z = sin(m_bobbing_phases.z) * m_bobbing_amplitude;
}

// Função auxiliar: Desenha uma pirâmide de base quadrada
// Base no plano XY (centrada em 0,0), Ponta em +Z
void Boid::drawUnitPyramid() const {
    glBegin(GL_TRIANGLES);

    // Frente (Face +Y)
    glNormal3f(0.0f, 0.894f, 0.447f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);

    // Direita (Face +X)
    glNormal3f(0.894f, 0.0f, 0.447f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);

    // Trás (Face -Y)
    glNormal3f(0.0f, -0.894f, 0.447f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);

    // Esquerda (Face -X)
    glNormal3f(-0.894f, 0.0f, 0.447f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glEnd();

    // Base (Quadrada)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();
}

void Boid::draw() const {

    // TRONCO
    glColor3f(0.2f, 0.5f, 0.8f); // Azul
    glPushMatrix();
    glScalef(1.5f, 0.6f, 0.6f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawUnitPyramid();
    glPopMatrix();

    // CABEÇA
    glColor3f(1.0f, 0.8f, 0.2f); // Amarelo
    glPushMatrix();
    glTranslatef(0.75f, 0.0f, 0.0f);
    glScalef(0.6f, 0.4f, 0.4f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawUnitPyramid();
    glPopMatrix();

    // BRAÇO ESQUERDO
    glColor3f(0.4f, 0.7f, 1.0f);
    glPushMatrix();
    glTranslatef(0.2f, -0.3f, 0.1f);
    glRotatef(m_current_wing_angle, 1.0f, 0.0f, 0.0f);
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glScalef(0.3f, 1.2f, 0.1f);
    drawUnitPyramid();
    glPopMatrix();

    // BRAÇO DIREITO
    glPushMatrix();
    glTranslatef(0.2f, 0.3f, 0.1f);
    glRotatef(-m_current_wing_angle, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glScalef(0.3f, 1.2f, 0.1f);
    drawUnitPyramid();
    glPopMatrix();

    // RABO
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.2f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -0.4f, 0.0f);
    glVertex3f(-1.0f, 0.4f, 0.0f);
    glEnd();
}