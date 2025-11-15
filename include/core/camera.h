#pragma once

#include <GL/glew.h>
#include <GL/glu.h>

#include "objects/object.h"
#include "utils/vmath.h"

enum class Mode {
    STATIC_WORLD,
    FOLLOW_PLAYER
};

class Camera {
public:
    Camera();

    void setFollowTarget(Object *target);

    void toggleMode();

    void setupViewMatrix() const;

private:
    Mode m_mode;
    Object *m_followTarget;
};