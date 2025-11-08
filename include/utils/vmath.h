#ifndef VMATH_H
#define VMATH_H

#include <math.h>       // Para sqrtf, fabsf

// Definição da nossa estrutura de Vetor 3D
typedef struct {
    float x, y, z;
} Vec3;

// --- Funções Básicas ---
Vec3 vec3_create(float x, float y, float z);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_scale(Vec3 v, float s);

// --- Funções de Magnitude e Normalização ---
float vec3_length(Vec3 v);
Vec3 vec3_normalize(Vec3 v);

// --- Outras Funções Úteis ---
float vec3_distance(Vec3 a, Vec3 b);
float vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_cross(Vec3 a, Vec3 b);

#endif // VMATH_H