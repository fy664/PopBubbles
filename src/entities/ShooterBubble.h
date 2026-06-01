#pragma once
#include "entities/Bubble.h"

/**
 * 射手泡泡类
 * 紫色圆形，保持与玩家 200-300 像素距离
 * 每 3 秒向玩家方向发射一颗子弹
 */
class ShooterBubble : public Bubble {
public:
    static constexpr float PREFERRED_DISTANCE_MIN = 200.f;
    static constexpr float PREFERRED_DISTANCE_MAX = 300.f;
    static constexpr float SHOOT_INTERVAL = 3.f;
    static constexpr float SPEED = 100.f;
    static constexpr int SCORE_VALUE = 30;

    ShooterBubble();

    void update(float deltaTime) override;

    bool shouldShoot() const;
    void resetShootTimer();
    float getShootTimer() const { return m_shootTimer; }

private:
    float m_shootTimer;
};
