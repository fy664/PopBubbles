#pragma once
#include "entities/Entity.h"
#include <SFML/Graphics.hpp>

/**
 * 普通泡泡类
 * 随机颜色圆形，持续向玩家移动
 * 被尖针刺中后消失并加分
 * 与玩家碰撞造成伤害
 */
class Bubble : public Entity {
public:
    static constexpr float BASE_SPEED = 140.f;
    static constexpr float RADIUS = 13.f;
    static constexpr int SCORE_VALUE = 10;

    enum class Type {
        Normal,
        Shooter,
        Giant
    };

    Bubble();
    explicit Bubble(Type type);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    Type getBubbleType() const { return m_type; }
    int getScoreValue() const;
    void setTarget(sf::Vector2f target) { m_target = target; }
    sf::Color getColor() const { return m_color; }

    // 刷新外观（颜色变化等）
    void refreshAppearance();

    // Hit cooldown to prevent multi-hit in one attack cycle
    bool canBeHit() const { return m_hitCooldown <= 0.f; }
    void markHit(float cooldown = 0.35f) { m_hitCooldown = cooldown; }
    void updateCooldown(float dt) { if (m_hitCooldown > 0.f) m_hitCooldown -= dt; }

protected:
    Type m_type;
    sf::Color m_color;
    sf::CircleShape m_shape;
    sf::Vector2f m_target;
    float m_hitCooldown = 0.f;

    void initShape();
};
