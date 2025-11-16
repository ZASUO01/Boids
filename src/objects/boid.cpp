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
                       m_bobbing_amplitude(0.2f),
                       m_isPlayer(false) {

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

void Boid::update(float delta_time, const std::vector<std::unique_ptr<Object>> &all_objects, Object *player_boid) {

    if (!m_isPlayer) {
        updateAI(delta_time, all_objects, player_boid);
    }

    applyPhysics(delta_time);
    updateAnimations(delta_time);
}

void Boid::updateAI(float delta_time,
                    const std::vector<std::unique_ptr<Object>>& all_objects,
                    Object* player_boid) {
    
    Vec3 separation_force = {0,0,0};
    Vec3 alignment_force = {0,0,0};
    Vec3 cohesion_force = {0,0,0};
    Vec3 follow_force = {0,0,0};
    
    Vec3 center_of_mass = {0,0,0};
    Vec3 velocity_sum = {0,0,0};
    int neighbors_found = 0;
    
    float desired_separation = 3.0f;
    float neighbor_radius = 10.0f;
    float sep_weight = 1.5f;
    float ali_weight = 1.0f;
    float coh_weight = 1.0f;
    float follow_weight = 0.5f;
    float target_speed = 10.0f; 

    bool isChasing = false;
    if (player_boid != nullptr) {
        float dist_to_player = vec3_distance(m_position, player_boid->getPosition());
        
        if (dist_to_player > neighbor_radius) {
            isChasing = true;
            
            follow_force = vec3_sub(player_boid->getPosition(), m_position);
            follow_force = vec3_normalize(follow_force);
            follow_weight = 2.0f;
            
            ali_weight = 0.0f;
            coh_weight = 0.0f;
            
            target_speed = 30.0f;
        }
    }

    if (!isChasing) {
        for (const auto& other_obj : all_objects) {
            if (other_obj.get() == this) continue;
            Boid* other_boid = dynamic_cast<Boid*>(other_obj.get());
            if (other_boid == nullptr) continue; 
            
            float dist = vec3_distance(m_position, other_boid->getPosition());
            
            if (dist <= neighbor_radius) {
                neighbors_found++;
                
                if (dist < desired_separation) {
                    Vec3 diff = vec3_sub(m_position, other_boid->getPosition());
                    diff = vec3_normalize(diff);
                    diff = vec3_scale(diff, 1.0f / (dist + 0.0001f));
                    separation_force = vec3_add(separation_force, diff);
                }
                
                velocity_sum = vec3_add(velocity_sum, other_boid->getVelocity());
                center_of_mass = vec3_add(center_of_mass, other_boid->getPosition());
            }
        }

        if (neighbors_found > 0) {
            // Alinhamento
            alignment_force = vec3_scale(velocity_sum, 1.0f / neighbors_found);
            target_speed = vec3_length(alignment_force); 
            alignment_force = vec3_normalize(alignment_force);
            
            // Coesão
            cohesion_force = vec3_scale(center_of_mass, 1.0f / neighbors_found);
            cohesion_force = vec3_sub(cohesion_force, m_position);
            cohesion_force = vec3_normalize(cohesion_force);
        }
    }
    
    Vec3 total_force = {0,0,0};
    total_force = vec3_add(total_force, vec3_scale(separation_force, sep_weight));
    total_force = vec3_add(total_force, vec3_scale(alignment_force, ali_weight));
    total_force = vec3_add(total_force, vec3_scale(cohesion_force, coh_weight));
    total_force = vec3_add(total_force, vec3_scale(follow_force, follow_weight));
    
    if (vec3_length(total_force) > 0.0f) {
        total_force = vec3_normalize(total_force);

        // Suaviza a curva (interpolação)
        m_forward = vec3_add(vec3_scale(m_forward, 0.95f), vec3_scale(total_force, 0.05f));
        m_forward = vec3_normalize(m_forward);
        
        m_right = vec3_normalize(vec3_cross(m_forward, vec3_create(0,0,1)));
        m_up = vec3_normalize(vec3_cross(m_right, m_forward));
    }
    
    // Suaviza a velocidade atual em direção à velocidade do bando/perseguição
    float speed_lerp_factor = 0.02f;
    m_speed = (m_speed * (1.0f - speed_lerp_factor)) + (target_speed * speed_lerp_factor);

    const float MIN_SPEED = 2.0f;
    if (m_speed < MIN_SPEED) m_speed = MIN_SPEED;
}

void Boid::applyPhysics(float delta_time) {
    m_position = vec3_add(m_position, vec3_scale(m_forward, m_speed * delta_time));
}

void Boid::updateAnimations(float delta_time) {
    // Animação de Asas
    m_wing_phase += m_wing_speed * delta_time;
    if (m_wing_phase > (GLfloat)(2.0f * M_PI))
        m_wing_phase -= (GLfloat)(2.0f * M_PI);
    m_current_wing_angle = sin(m_wing_phase) * m_wing_amplitude;

    // Animação de Flutuação
    m_bobbing_phases.x += m_bobbing_speeds.x * delta_time;
    m_bobbing_phases.y += m_bobbing_speeds.y * delta_time;
    m_bobbing_phases.z += m_bobbing_speeds.z * delta_time;

    if (m_bobbing_phases.x > (GLfloat)(2.0f * M_PI))
        m_bobbing_phases.x -= (GLfloat)(2.0f * M_PI);

    if (m_bobbing_phases.y > (GLfloat)(2.0f * M_PI))
        m_bobbing_phases.y -= (GLfloat)(2.0f * M_PI);

    if (m_bobbing_phases.z > (GLfloat)(2.0f * M_PI))
        m_bobbing_phases.z -= (GLfloat)(2.0f * M_PI);

    // Calcula o offset
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