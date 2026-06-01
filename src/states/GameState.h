#pragma once
#include "core/State.h"
#include "entities/Player.h"
#include "entities/Bubble.h"
#include "entities/ShooterBubble.h"
#include "entities/GiantBubble.h"
#include "entities/Bullet.h"
#include <vector>
#include <memory>

/**
 * 核心游戏状态
 * 管理玩家、敌人生成器、得分系统、碰撞检测、游戏循环
 */
class GameState : public State {
public:
    GameState();
    ~GameState() override = default;

    void handleInput(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;

    // 获取玩家位置（供泡泡AI使用）
    sf::Vector2f getPlayerPosition() const;

private:
    void spawnEnemy(float deltaTime);
    void updateEnemies(float deltaTime);
    void checkCollisions();
    void updateBullets(float deltaTime);
    void updateUI();
    void spawnEffect(sf::Vector2f position, sf::Color color);

    // 实体
    Player m_player;
    std::vector<std::unique_ptr<Bubble>> m_bubbles;
    std::vector<std::unique_ptr<ShooterBubble>> m_shooterBubbles;
    std::vector<std::unique_ptr<GiantBubble>> m_giantBubbles;
    BulletPool m_bulletPool;

    // 敌人生成
    float m_spawnTimer;
    float m_spawnInterval;
    float m_difficultyTimer;
    int m_difficultyLevel;

    // 得分
    int m_score;
    sf::Text m_scoreText;
    sf::Text m_hpText;
    sf::Text m_waveText;

    // 粒子特效（泡泡破裂等）
    struct Particle {
        sf::CircleShape shape;
        sf::Vector2f velocity;
        float lifetime;
        float maxLifetime;
    };
    std::vector<Particle> m_particles;

    // 屏幕震动
    float m_screenShake;
};
