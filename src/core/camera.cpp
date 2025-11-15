#include "core/camera.h"

Camera::Camera()
    : m_mode(Mode::STATIC_WORLD),
      m_followTarget(nullptr) {}

void Camera::setFollowTarget(Object *target) {
    m_followTarget = target;
}

void Camera::toggleMode() {
    if (m_mode == Mode::STATIC_WORLD) {
        m_mode = Mode::FOLLOW_PLAYER;
    } else {
        m_mode = Mode::STATIC_WORLD;
    }
}

void Camera::setupViewMatrix() const {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (m_mode == Mode::STATIC_WORLD || m_followTarget == nullptr) {
        // Câmera estática
        gluLookAt(
            20.0f, 20.0f, 20.0f, // eye
            0.0f, 0.0f, 5.0f,    // target
            0.0f, 0.0f, 1.0f);   // up (Z-Up)

    } else {
        // Câmera de seguir
        Vec3 pos = m_followTarget->getPosition();
        Vec3 fwd = m_followTarget->getForward();
        Vec3 up = m_followTarget->getUp();

        float dist_behind = 5.0f;
        float dist_above = 3.0f;

        Vec3 eye = vec3_sub(pos, vec3_scale(fwd, dist_behind));
        eye = vec3_add(eye, vec3_scale(up, dist_above));
        Vec3 target = vec3_add(pos, vec3_scale(fwd, 5.0f));

        gluLookAt(
            eye.x, eye.y, eye.z,
            target.x, target.y, target.z,
            up.x, up.y, up.z);
    }
}