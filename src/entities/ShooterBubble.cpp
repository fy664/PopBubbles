#include "entities/ShooterBubble.h"
#include <cmath>

ShooterBubble::ShooterBubble()
    : Bubble(Type::Shooter)
    , m_shootTimer(0.f) {

    m_radius = RADIUS;
    m_color = sf::Color(160, 50, 200); // 紫色
    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);
    m_shape.setFillColor(m_color);
    m_shape.setOutlineThickness(3.f);
    m_shape.setOutlineColor(sf::Color(100, 20, 140));
}

void ShooterBubble::update(float deltaTime) {
    if (!m_active) return;

    updateCooldown(deltaTime);  // 必须调用，否则 hit cooldown 永不过期

    m_shootTimer += deltaTime;

    // 与玩家的距离
    float dist = distanceTo(m_target);
    sf::Vector2f dir = directionTo(m_target);

    // 太近则后退，太远则前进，保持 200-300 距离
    if (dist < PREFERRED_DISTANCE_MIN) {
        // 后退
        m_position.x -= dir.x * SPEED * deltaTime;
        m_position.y -= dir.y * SPEED * deltaTime;
    } else if (dist > PREFERRED_DISTANCE_MAX) {
        // 前进
        m_position.x += dir.x * SPEED * deltaTime;
        m_position.y += dir.y * SPEED * deltaTime;
    }
    // 在合适范围内：保持不动（或微小漂移）

    m_shape.setPosition(m_position);
}

bool ShooterBubble::shouldShoot() const {
    return m_shootTimer >= SHOOT_INTERVAL;
}

void ShooterBubble::resetShootTimer() {
    m_shootTimer = 0.f;
}
