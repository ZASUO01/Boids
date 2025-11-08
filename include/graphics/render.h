#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>

#include "utils/vmath.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Desenha um plano horizontal (chão) centrado em (0, 0, 0)
void draw_ground(float size, float y_pos);

// Desenha um cone (torre) centrado em (0, 0, 0)
// A base fica em y_pos e ele cresce para +Y
void draw_tower(float base_radius, float height, int slices, float y_pos);

#endif