#include "entities/Bubble.h"
#include "core/Game.h"
#include <random>
#include <cmath>

// 随机数工具
static std::random_device rd;
static std::mt19937 rng(rd());
static std::uniform_int_distribution<int> colorDist(50, 220);

Bubble::Bubble() : Bubble(Type::Normal) {}

Bubble::Bubble(Type type)
    : m_type(type) {
    m_radius = RADIUS;
    m_active = true;

    initShape();
}

void Bubble::initShape() {
    // 随机颜色（针对普通泡泡和巨型泡泡）
    m_color = sf::Color(
        colorDist(rng),
        colorDist(rng),
        colorDist(rng),
        255
    );

    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);
    m_shape.setFillColor(m_color);
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color(
        std::max(0, m_color.r - 30),
        std::max(0, m_color.g - 30),
        std::max(0, m_color.b - 30)
    ));
}

void Bubble::update(float deltaTime) {
    if (!m_active) return;

    // 向目标（玩家）移动
    sf::Vector2f dir = directionTo(m_target);
    float speed = BASE_SPEED;

    m_position.x += dir.x * speed * deltaTime;
    m_position.y += dir.y * speed * deltaTime;

    // 不要将普通泡泡clamp到屏幕内——它们可以在屏幕边缘外生成并移入
    // 只更新 shape 位置
    m_shape.setPosition(m_position);
}

void Bubble::render(sf::RenderWindow& window) {
    if (!m_active) return;
    window.draw(m_shape);
}

int Bubble::getScoreValue() const {
    return SCORE_VALUE;
}

void Bubble::refreshAppearance() {
    m_shape.setFillColor(m_color);
    m_shape.setOutlineColor(sf::Color(
        std::max(0, m_color.r - 30),
        std::max(0, m_color.g - 30),
        std::max(0, m_color.b - 30)
    ));
}
