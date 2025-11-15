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
    GLfloat m_turn_speed;
    GLfloat m_pitch_speed;
    GLfloat m_accel_amount;

    virtual void draw() const = 0;

public:
    Object(Vec3 pos) : m_position(pos),
                       m_position_offset({0.0f, 0.0f, 0.0f}),
                       m_forward({1.0f, 0.0f, 0.0f}),             // looking "forward" X-axis
                       m_up({0.0f, 0.0f, 1.0f}),                  // "Up" is the Z axis
                       m_right({0.0f, 1.0f, 0.0f}),               // "Right" is the Y axis
                       m_local_rotation_axis({0.0f, 0.0f, 0.0f}), // Girar em torno do eixo
                       m_speed(0.0f),
                       m_rotation_angle(0.0f),
                       m_rotation_speed(0.0f),
                       m_scale(1.0f),
                       m_turn_speed(0.0f),
                       m_pitch_speed(0.0f),
                       m_accel_amount(0.0f) {}

    virtual ~Object() = default;

    void render() const {

        glPushMatrix();

        Vec3 final_pos = vec3_add(m_position, m_position_offset);

        // Create a 4x4 Template Matrix from our vectors
        // OpenGL expects matrices in "column-major order"
        GLfloat matrix[16] = {
            m_forward.x, m_forward.y, m_forward.z, 0.0f, // Coluna 0 (Eixo X) = Nosso m_forward
            m_right.x, m_right.y, m_right.z, 0.0f,       // Coluna 1 (Eixo Y) = Nosso m_right
            m_up.x, m_up.y, m_up.z, 0.0f,                // Coluna 2 (Eixo Z) = Nosso m_up
            final_pos.x, final_pos.y, final_pos.z, 1.0f};

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

    virtual void update(float delta_time) {}

    virtual void accelerate(float direction, float delta_time) {}

    virtual void turn(float direction, float delta_time) {}

    virtual void pitch(float direction, float delta_time) {}

    Vec3 getPosition() const {
        return m_position;
    }

    Vec3 getForward() const {
        return m_forward;
    }

    Vec3 getUp() const {
        return m_up;
    }

    Vec3 getVelocity() const {
        return vec3_scale(m_forward, m_speed);
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