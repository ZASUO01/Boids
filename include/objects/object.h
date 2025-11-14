#pragma once

#include "utils/vmath.h"
#include <GL/glew.h>

#include <stdio.h>

class Object {
protected:
    Vec3 position;
    Vec3 forward;
    Vec3 up;
    Vec3 right;
    Vec3 local_rotation_axis;
    GLfloat speed;
    GLfloat rotation_angle;
    GLfloat rotation_speed;
    GLfloat scale;

    virtual void draw() const = 0;

public:
    Object(Vec3 pos) : position(pos),
                       forward({0.0f, 1.0f, 0.0f}),             // looking "forward" Y-axis
                       up({0.0f, 0.0f, 1.0f}),                  // "Up" is the Z axis
                       right({1.0f, 0.0f, 0.0f}),               // "Right" is the X axis
                       local_rotation_axis({0.0f, 0.0f, 0.0f}), // Girar em torno do eixo
                       speed(0.0f),
                       rotation_angle(0.0f),
                       rotation_speed(0.0f),
                       scale(1.0f) {}

    virtual ~Object() = default;

    virtual void update(float delta_time) {
        position = vec3_add(position, vec3_scale(forward, speed * delta_time));

        rotation_angle += rotation_speed * delta_time;
        if (rotation_angle > 360.0f) {
            rotation_angle -= 360.0f;
        }
    }

    void render() const {

        glPushMatrix();

        // Create a 4x4 Template Matrix from our vectors
        // OpenGL expects matrices in "column-major order"
        GLfloat matrix[16] = {
            right.x, right.y, right.z, 0.0f,           // Coluna 0 (Eixo X local = right)
            forward.x, forward.y, forward.z, 0.0f,     // Coluna 1 (Eixo Y local = forward)
            up.x, up.y, up.z, 0.0f,                    // Coluna 2 (Eixo Z local = up)
            position.x, position.y, position.z, 1.0f}; // Coluna 3 (Posição)

        // Apply this matrix
        glMultMatrixf(matrix);

        glRotatef(rotation_angle,
                  local_rotation_axis.x,
                  local_rotation_axis.y,
                  local_rotation_axis.z);

        glScalef(scale, scale, scale);

        // Call the draw function
        this->draw();

        glPopMatrix();
    }

    void setRotationAxis(const Vec3 &axis) {
        local_rotation_axis = axis;
    }

    void setRotationSpeed(GLfloat rot_speed) {
        rotation_speed = rot_speed;
    }

    void setScale(GLfloat newScale) {
        scale = newScale;
    }
};