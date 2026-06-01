#pragma once
#include "entities/Entity.h"
#include <SFML/Graphics.hpp>

/**
 * 玩家类
 * WASD 控制移动，一根锥形尖针从球心伸出始终指向鼠标方向
 * 尖针整体伸缩动画：左键点击时向前延伸35像素持续0.3秒
 * 被动碰撞：静止尖针也能戳破泡泡；攻击延伸获得更远射程
 * 受伤后1.5秒无敌时间，带闪烁效果
 */
class Player : public Entity {
public:
    static constexpr float SPEED = 350.f;
    static constexpr float RADIUS = 18.f;
    static constexpr int BASE_MAX_HP = 5;              // 初始血量
    static constexpr int HP_PER_LEVEL = 2;             // 每级增加血量上限
    static constexpr float INVINCIBLE_TIME = 1.5f;
    static constexpr float NEEDLE_LENGTH = 55.f;      // 尖针从球心伸出总长度
    static constexpr float NEEDLE_EXTEND = 35.f;       // 戳刺时额外延伸距离
    static constexpr float NEEDLE_ATTACK_DURATION = 0.3f; // 攻击持续时间
    static constexpr float NEEDLE_WIDTH = 7.f;         // 尖针粗细
    static constexpr int KILLS_PER_LEVEL = 10;         // 每10击杀升一级

    Player();

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    // 受伤
    void takeDamage();

    // 回复生命值
    void heal();
    void healFull() { m_hp = m_maxHp; }

    // 等级系统
    void addKill();
    int getLevel() const { return m_level; }
    int getKills() const { return m_kills; }

    // 被巨型泡泡击退
    void applyRecoil(sf::Vector2f from, float strength);

    // 状态查询
    bool isInvincible() const { return m_invincible; }
    bool isAttacking() const { return m_attacking; }
    int getHP() const { return m_hp; }
    int getMaxHP() const { return m_maxHp; }
    bool isDead() const { return m_hp <= 0; }

    // 获取尖针尖端位置（攻击判定用）
    sf::Vector2f getNeedleTip() const;

    // 获取尖针线段（用于碰撞检测）
    // 返回 {needleBase, needleTip}
    std::pair<sf::Vector2f, sf::Vector2f> getNeedleLine() const;

    // 设置朝向角度（弧度）
    void setNeedleAngle(float angle);
    float getNeedleAngle() const { return m_needleAngle; }

    // 获取当前尖针实际长度
    float getCurrentNeedleLength() const;

private:
    void handleMovement(float deltaTime);
    void handleAttack(float deltaTime);
    void updateNeedle();

    int m_maxHp = BASE_MAX_HP;   // 必须在 m_hp 之前声明（构造函数用 m_maxHp 初始化 m_hp）
    int m_hp;
    int m_level = 1;
    int m_kills = 0;
    bool m_invincible;
    float m_invincibleTimer;
    float m_blinkTimer;

    // 尖针相关
    float m_needleAngle;        // 尖针朝向角度（弧度）
    bool m_attacking;            // 是否正在攻击
    float m_attackTimer;        // 攻击计时器
    float m_needleAnimProgress; // 尖针动画进度 0~1（延伸比例）

    sf::CircleShape m_body;
    sf::ConvexShape m_needle;   // 锥形尖针（整体伸缩动画）
};
