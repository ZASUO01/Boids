#include "objects/tower.h"
#include "utils/vmath.h" // Garanta que vmath.h está incluído
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Tower::Tower(Vec3 pos) : Object(pos) {
    speed = 0.0f;
}

void Tower::draw() const {
    float base_radius = 5.0f;
    float height = 15.0f;
    int slices = 20;
    float z_pos = 0.0f;

    float angle_step = (2.0f * (float)M_PI) / (float)slices;

    glColor3f(0.8f, 0.2f, 0.2f);

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, z_pos);

    for (int i = 0; i <= slices; i++) {
        float angle = (float)i * angle_step;
        float x = base_radius * (float)cos(angle);
        float y = base_radius * (float)sin(angle);
        glVertex3f(x, y, z_pos);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    Vec3 top = vec3_create(0.0f, 0.0f, z_pos + height);

    for (int i = 0; i < slices; i++) {
        float angle1 = (float)i * angle_step;
        float x1 = base_radius * (float)cos(angle1);
        float y1 = base_radius * (float)sin(angle1);
        Vec3 v1 = vec3_create(x1, y1, z_pos);

        float angle2 = (float)(i + 1) * angle_step;
        float x2 = base_radius * (float)cos(angle2);
        float y2 = base_radius * (float)sin(angle2);
        Vec3 v2 = vec3_create(x2, y2, z_pos);

        glNormal3f(x1, y1, 0.2f);

        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(top.x, top.y, top.z);
    }
    glEnd();
}