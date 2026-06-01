#pragma once
#include "entities/Bubble.h"

/**
 * 巨型泡泡类
 * 橙色大圆形（普通泡泡2倍大），带厚重边框
 * 平时缓慢移动，玩家进入150像素范围时触发冲刺
 * 冲刺：速度×3，持续0.5秒，冷却1秒
 * 需要2次攻击击败，第一次后颜色变浅
 */
class GiantBubble : public Bubble {
public:
    static constexpr float NORMAL_SPEED = 70.f;
    static constexpr float CHARGE_SPEED = 200.f;
    static constexpr float CHARGE_RANGE = 140.f;
    static constexpr float CHARGE_DURATION = 0.5f;
    static constexpr float CHARGE_COOLDOWN = 1.f;
    static constexpr float RADIUS = 26.f;
    static constexpr int SCORE_VALUE = 50;
    static constexpr int MAX_HP = 2;

    GiantBubble();

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void takeDamage();
    int getHP() const { return m_hp; }
    bool isCharging() const { return m_charging; }

private:
    int m_hp;
    bool m_charging;
    float m_chargeTimer;
    float m_cooldownTimer;
    bool m_damaged;  // 受伤状态（颜色变浅）
};
