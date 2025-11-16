#pragma once

#include <GL/glew.h>
#include <cmath>

#include "objects/object.h"

class Boid : public Object {
public:
    Boid(Vec3 pos);

    virtual void update(float delta_time) override;
    virtual void accelerate(float direction, float delta_time) override;
    virtual void turn(float direction, float delta_time) override;
    virtual void pitch(float direction, float delta_time) override;

protected:
    GLfloat m_wing_phase;         // Acumulador de tempo para o sin()
    GLfloat m_wing_speed;         // Quão rápido as asas batem
    GLfloat m_wing_amplitude;     // O ângulo máximo (ex: 30 graus)
    GLfloat m_current_wing_angle; // O resultado final (usado pelo draw)
    GLfloat m_bobbing_amplitude;  // Quão alto/baixo ele flutua (o offset)
    Vec3 m_bobbing_phases;        // Fases independentes (X, Y, Z)
    Vec3 m_bobbing_speeds;        // Velocidades independentes (X, Y, Z)

    virtual void draw() const override;

private:
    static constexpr float MIN_SPEED = 2.0f;
    static constexpr float MAX_SPEED = 30.0f;

    // Função auxiliar para desenhar uma pirâmide padrão
    void drawUnitPyramid() const;
};