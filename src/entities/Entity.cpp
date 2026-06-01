#include "entities/Entity.h"
#include <cmath>

Entity::Entity()
    : m_position(0.f, 0.f)
    , m_velocity(0.f, 0.f)
    , m_radius(20.f)
    , m_active(true) {
}

bool Entity::clampToScreen(unsigned int screenW, unsigned int screenH) {
    bool clamped = false;
    float r = m_radius;

    if (m_position.x < r) {
        m_position.x = r;
        clamped = true;
    }
    if (m_position.x > static_cast<float>(screenW) - r) {
        m_position.x = static_cast<float>(screenW) - r;
        clamped = true;
    }
    if (m_position.y < r) {
        m_position.y = r;
        clamped = true;
    }
    if (m_position.y > static_cast<float>(screenH) - r) {
        m_position.y = static_cast<float>(screenH) - r;
        clamped = true;
    }

    return clamped;
}

bool Entity::isOffScreen(unsigned int screenW, unsigned int screenH, float margin) const {
    float r = m_radius + margin;
    return m_position.x < -r ||
           m_position.x > static_cast<float>(screenW) + r ||
           m_position.y < -r ||
           m_position.y > static_cast<float>(screenH) + r;
}

sf::Vector2f Entity::directionTo(sf::Vector2f target) const {
    sf::Vector2f diff = target - m_position;
    float len = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (len < 0.0001f) return {0.f, 0.f};
    return {diff.x / len, diff.y / len};
}

float Entity::distanceTo(sf::Vector2f target) const {
    float dx = target.x - m_position.x;
    float dy = target.y - m_position.y;
    return std::sqrt(dx * dx + dy * dy);
}
