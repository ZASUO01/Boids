#include "objects/tower.h"

Tower::Tower(Vec3 pos) : Object(pos) {
    speed = 0.0f;
}

void Tower::draw() const {
    float base_radius = 5.0f;
    float height = 15.0f;
    int slices = 20;
    float y_pos = 0.0f;
    
    float angle_step = (2.0f * (float) M_PI) / (float)slices;

    glColor3f(0.8f, 0.2f, 0.2f);

    // Tower base
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, y_pos, 0.0f);
        
        for (int i = 0; i <= slices; i++) {
            float angle = (float) i * angle_step;
            float x = base_radius * (float) cos(angle);
            float z = base_radius * (float) sin(angle);
            glVertex3f(x, y_pos, z);
        }
    glEnd();

    // Tower sides
    glBegin(GL_TRIANGLES);
        Vec3 top = vec3_create(0.0f, y_pos + height, 0.0f);
        for (int i = 0; i < slices; i++) {
            float angle1 = (float) i * angle_step;
            float x1 = base_radius * (float) cos(angle1);
            float z1 = base_radius * (float) sin(angle1);
            Vec3 v1 = vec3_create(x1, y_pos, z1);

            float angle2 = (float) (i + 1) * angle_step;
            float x2 = base_radius * (float) cos(angle2);
            float z2 = base_radius * (float) sin(angle2);
            Vec3 v2 = vec3_create(x2, y_pos, z2);
            
            glNormal3f(x1, 0.2f, z1);
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(top.x, top.y, top.z);
        }
    glEnd();
}