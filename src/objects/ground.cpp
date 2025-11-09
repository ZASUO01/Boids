#include "objects/ground.h"

Ground::Ground(Vec3 pos) : Object(pos) {
    speed = 0.0f;
}

void Ground::draw() const {
    float size = 100.0f;
    float half_size = size / 2.0f;
    
    // Ground color 
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-half_size, 0,  half_size);
        glVertex3f( half_size, 0,  half_size);
        glVertex3f( half_size, 0, -half_size);
        glVertex3f(-half_size, 0, -half_size);
    glEnd();
}