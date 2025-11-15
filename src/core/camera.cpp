#include "core/camera.h"

Camera::Camera()
    : m_mode(Mode::FOLLOW_PLAYER),
      m_followTarget(nullptr) {}

void Camera::setFollowTarget(Object *target) {
    m_followTarget = target;
}

void Camera::toggleMode() {
    switch (m_mode) {
    case Mode::TOWER_TOP:
        m_mode = Mode::FOLLOW_PLAYER;
        break;
    case Mode::FOLLOW_PLAYER:
        m_mode = Mode::SIDE_PLAYER;
        break;
    case Mode::SIDE_PLAYER:
        m_mode = Mode::TOWER_TOP;
        break;
    default:
        m_mode = Mode::FOLLOW_PLAYER;
        break;
    }
}

void Camera::setupViewMatrix() const {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    const Vec3 up_vec = vec3_create(0.0f, 0.0f, 1.0f);

    Vec3 target = vec3_create(0.0f, 0.0f, 5.0f);
    Vec3 eye = vec3_create(20.0f, 20.0f, 20.0f);

    if (m_followTarget != nullptr) {
        target = m_followTarget->getPosition();
    }

    switch (m_mode) {
    case Mode::TOWER_TOP:
        eye = vec3_create(0.0f, 0.0f, 16.0f);
        break;

    case Mode::FOLLOW_PLAYER:
        if (m_followTarget != nullptr) {
            Vec3 fwd = m_followTarget->getForward();
            Vec3 up = m_followTarget->getUp();

            float dist_behind = 5.0f;
            float dist_above = 3.0f;

            eye = vec3_sub(target, vec3_scale(fwd, dist_behind));
            eye = vec3_add(eye, vec3_scale(up, dist_above));
            target = vec3_add(target, vec3_scale(fwd, 5.0f));
        }
        break;

    case Mode::SIDE_PLAYER:

        if (m_followTarget != nullptr) {
            Vec3 fwd = m_followTarget->getForward();
            float dist_side = 7.0f;

            Vec3 side_vec = vec3_normalize(vec3_cross(fwd, up_vec));
            eye = vec3_add(target, vec3_scale(side_vec, dist_side));
            eye.z += 3.0f;
        }
        break;
    default:
        break;
    }

    gluLookAt(
        eye.x, eye.y, eye.z,
        target.x, target.y, target.z,
        up_vec.x, up_vec.y, up_vec.z);
}