#pragma once

#include <GL/glew.h>
#include <GL/glu.h>

#include "objects/object.h"
#include "utils/vmath.h"

enum class Mode {
    TOWER_TOP,          // Modo 1: No topo da torre
    FOLLOW_PLAYER,      // Modo 2: Atrás do bando
    SIDE_LEFT,          // Modo 3: Perpendicular ao bando (lado esquerdo)
    SIDE_RIGHT          // Modo 4: Perpendicular ao bando (lado direito)
};

class Camera {
public:
    Camera();

    void setFollowTarget(Object* target);

    void toggleMode();

    void setupViewMatrix() const;

private:
    Mode m_mode;
    Object* m_followTarget;

    void setupStaticView(Vec3& eye, Vec3& target) const;

    void setupTowerView(Vec3& eye, const Vec3& target) const;

    void setupFollowView(Vec3& eye, Vec3& target) const;

    void setupSideView(Vec3& eye, const Vec3& target, float side_multiplier) const;
};