#include "entities/GiantBubble.h"
#include <cmath>

GiantBubble::GiantBubble()
    : Bubble(Type::Giant)
    , m_hp(MAX_HP)
    , m_charging(false)
    , m_chargeTimer(0.f)
    , m_cooldownTimer(0.f)
    , m_damaged(false) {

    m_radius = RADIUS;
    m_color = sf::Color(255, 140, 30); // 橙色
    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);
    m_shape.setFillColor(m_color);
    m_shape.setOutlineThickness(5.f);  // 厚重边框
    m_shape.setOutlineColor(sf::Color(200, 80, 10));
}

void GiantBubble::update(float deltaTime) {
    if (!m_active) return;

    // 冷却时间更新
    if (m_cooldownTimer > 0.f) {
        m_cooldownTimer -= deltaTime;
    }

    // 冲刺计时
    if (m_charging) {
        m_chargeTimer -= deltaTime;
        if (m_chargeTimer <= 0.f) {
            m_charging = false;
            m_cooldownTimer = CHARGE_COOLDOWN;
        }
    }

    // 检测是否触发冲刺
    float dist = distanceTo(m_target);
    if (!m_charging && m_cooldownTimer <= 0.f && dist <= CHARGE_RANGE) {
        m_charging = true;
        m_chargeTimer = CHARGE_DURATION;
    }

    // 移动
    sf::Vector2f dir = directionTo(m_target);
    float speed = m_charging ? CHARGE_SPEED : NORMAL_SPEED;

    m_position.x += dir.x * speed * deltaTime;
    m_position.y += dir.y * speed * deltaTime;

    m_shape.setPosition(m_position);
}

void GiantBubble::render(sf::RenderWindow& window) {
    if (!m_active) return;

    // 受伤颜色变浅
    if (m_damaged) {
        m_shape.setFillColor(sf::Color(255, 200, 150));
    }

    // 冲刺时边框发光
    if (m_charging) {
        m_shape.setOutlineColor(sf::Color(255, 255, 100));
    } else {
        m_shape.setOutlineColor(sf::Color(200, 80, 10));
    }

    window.draw(m_shape);
}

void GiantBubble::takeDamage() {
    if (!canBeHit()) return;
    markHit(0.35f);

    m_hp--;
    if (m_hp <= 0) {
        m_active = false;
    } else {
        m_damaged = true;
        m_color = sf::Color(255, 200, 150);
        refreshAppearance();
    }
}
