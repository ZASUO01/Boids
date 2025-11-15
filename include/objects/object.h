#pragma once

#include "utils/vmath.h"
#include <GL/glew.h>

#include <stdio.h>

class Object {
protected:
    Vec3 m_position;
    Vec3 m_position_offset;
    Vec3 m_forward;
    Vec3 m_up;
    Vec3 m_right;
    Vec3 m_local_rotation_axis;
    GLfloat m_speed;
    GLfloat m_rotation_angle;
    GLfloat m_rotation_speed;
    GLfloat m_scale;

    virtual void draw() const = 0;

public:
    Object(Vec3 pos) : m_position(pos),
                       m_position_offset({0.0f, 0.0f, 0.0f}),
                       m_forward({0.0f, 1.0f, 0.0f}),             // looking "forward" Y-axis
                       m_up({0.0f, 0.0f, 1.0f}),                  // "Up" is the Z axis
                       m_right({1.0f, 0.0f, 0.0f}),               // "Right" is the X axis
                       m_local_rotation_axis({0.0f, 0.0f, 0.0f}), // Girar em torno do eixo
                       m_speed(0.0f),
                       m_rotation_angle(0.0f),
                       m_rotation_speed(0.0f),
                       m_scale(1.0f) {}

    virtual ~Object() = default;

    virtual void update(float delta_time) {
        m_position = vec3_add(m_position, vec3_scale(m_forward, m_speed * delta_time));

        m_rotation_angle += m_rotation_speed * delta_time;
        if (m_rotation_angle > 360.0f) {
            m_rotation_angle -= 360.0f;
        }
    }

    void render() const {

        glPushMatrix();

        Vec3 final_pos = vec3_add(m_position, m_position_offset);

        // Create a 4x4 Template Matrix from our vectors
        // OpenGL expects matrices in "column-major order"
        GLfloat matrix[16] = {
            m_right.x, m_right.y, m_right.z, 0.0f,        // Coluna 0 (Eixo X local = right)
            m_forward.x, m_forward.y, m_forward.z, 0.0f,  // Coluna 1 (Eixo Y local = forward)
            m_up.x, m_up.y, m_up.z, 0.0f,                 // Coluna 2 (Eixo Z local = up)
            final_pos.x, final_pos.y, final_pos.z, 1.0f}; // Coluna 3 (Posição)

        // Apply this matrix
        glMultMatrixf(matrix);

        glRotatef(m_rotation_angle,
                  m_local_rotation_axis.x,
                  m_local_rotation_axis.y,
                  m_local_rotation_axis.z);

        glScalef(m_scale, m_scale, m_scale);

        // Call the draw function
        this->draw();

        glPopMatrix();
    }

    void accelerate(float amount, float delta_time) {
        float max_speed = 50.0f;
        m_speed += amount * delta_time;
        if (m_speed > max_speed)
            m_speed = max_speed;
        if (m_speed < 0.0f)
            m_speed = 0.0f; // Sem ré
    }

    void turn(float degrees, float delta_time) {
        float rad = (degrees * delta_time) * ((float)M_PI / 180.0f);
        float c = cos(rad);
        float s = sin(rad);

        // Rotaciona os vetores 'forward' e 'right' em torno do 'up' (eixo Z)
        Vec3 old_forward = m_forward;
        m_forward.x = old_forward.x * c - old_forward.y * s;
        m_forward.y = old_forward.x * s + old_forward.y * c;

        Vec3 old_right = m_right;
        m_right.x = old_right.x * c - old_right.y * s;
        m_right.y = old_right.x * s + old_right.y * c;

        m_forward = vec3_normalize(m_forward);
        m_right = vec3_normalize(m_right);
    }

    Vec3 getPosition() const {
        return m_position;
    }

    Vec3 getForward() const {
        return m_forward;
    }

    Vec3 getUp() const {
        return m_up;
    }

    void setRotationAxis(const Vec3 &axis) {
        m_local_rotation_axis = axis;
    }

    void setRotationSpeed(GLfloat rot_speed) {
        m_rotation_speed = rot_speed;
    }

    void setScale(GLfloat newScale) {
        m_scale = newScale;
    }
};