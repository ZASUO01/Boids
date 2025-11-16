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
        m_mode = Mode::SIDE_LEFT;
        break;
    case Mode::SIDE_LEFT:
        m_mode = Mode::SIDE_RIGHT;
        break;
    case Mode::SIDE_RIGHT:
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
    Vec3 eye, target;

    if (m_followTarget == nullptr) {
        setupStaticView(eye, target);
        return;
    }

    target = m_followTarget->getPosition();

    switch (m_mode) {
    case Mode::TOWER_TOP:
        setupTowerView(eye, target);
        break;
    case Mode::FOLLOW_PLAYER:
        setupFollowView(eye, target);
        break;
    case Mode::SIDE_LEFT:
        setupSideView(eye, target, -1.0f);
        break;
    case Mode::SIDE_RIGHT:
        setupSideView(eye, target, 1.0f);
        break;
    default:
        setupFollowView(eye, target);
        break;
    }

    gluLookAt(
        eye.x, eye.y, eye.z,
        target.x, target.y, target.z,
        up_vec.x, up_vec.y, up_vec.z);
}

void Camera::setupStaticView(Vec3 &eye, Vec3 &target) const {
    eye = vec3_create(20.0f, 20.0f, 20.0f);
    target = vec3_create(0.0f, 0.0f, 5.0f);
}

void Camera::setupTowerView(Vec3 &eye, const Vec3 &target) const {
    eye = vec3_create(0.0f, 0.0f, 1 + Tower::TOWER_HEIGHT);
}

void Camera::setupFollowView(Vec3 &eye, Vec3 &target) const {
    Vec3 fwd = m_followTarget->getForward();
    Vec3 up = m_followTarget->getUp();
    float dist_behind = 5.0f;
    float dist_above = 3.0f;

    eye = vec3_sub(target, vec3_scale(fwd, dist_behind));
    eye = vec3_add(eye, vec3_scale(up, dist_above));
    target = vec3_add(target, vec3_scale(fwd, 5.0f));
}

void Camera::setupSideView(Vec3 &eye, const Vec3 &target, float side_multiplier) const {
    Vec3 fwd = m_followTarget->getForward();
    const Vec3 up_world = vec3_create(0.0f, 0.0f, 1.0f);
    float dist_side = 7.0f;

    Vec3 side_vec = vec3_normalize(vec3_cross(fwd, up_world));

    eye = vec3_add(target, vec3_scale(side_vec, dist_side * side_multiplier));
    eye.z += 3.0f;
}
