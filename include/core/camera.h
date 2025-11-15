#pragma once

#include <GL/glew.h>
#include <GL/glu.h>

#include "objects/object.h"
#include "utils/vmath.h"

enum class Mode {
    TOWER_TOP,    // Modo 1: No topo da torre
    FOLLOW_PLAYER, // Modo 2: Atrás do bando
    SIDE_PLAYER    // Modo 3: Perpendicular ao bando
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
};