#pragma once

#include <GL/glew.h>
#include <cmath>

#include "objects/object.h"

class Boid : public Object {
public:
    Boid(Vec3 pos);

protected:
    GLfloat m_wing_phase;           // Acumulador de tempo para o sin()
    GLfloat m_wing_speed;           // Quão rápido as asas batem
    GLfloat m_wing_amplitude;       // O ângulo máximo (ex: 30 graus)
    GLfloat m_current_wing_angle;   // O resultado final (usado pelo draw)
    GLfloat m_bobbing_phase;        // Acumulador de tempo para o sin()
    GLfloat m_bobbing_speed;        // Quão rápido ele flutua
    GLfloat m_bobbing_amplitude;    // Quão alto/baixo ele flutua (o offset)

    virtual void update(float delta_time) override;

    virtual void draw() const override;

private:
    // Função auxiliar para desenhar uma pirâmide padrão
    void drawUnitPyramid() const;
};