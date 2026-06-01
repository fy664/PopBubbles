#pragma once
#include "entities/Entity.h"
#include <SFML/Graphics.hpp>
#include <vector>

/**
 * 子弹类
 * 由射手泡泡发射，向玩家方向飞行
 */
class Bullet : public Entity {
public:
    static constexpr float SPEED = 280.f;
    static constexpr float RADIUS = 5.f;
    static constexpr float LIFETIME = 4.f;  // 最大生存时间

    Bullet();

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    // 激活子弹
    void fire(sf::Vector2f position, sf::Vector2f direction);

    // 失效
    void deactivate();
    bool isActive() const { return m_active; }
    float getLifeTimer() const { return m_lifeTimer; }

private:
    sf::CircleShape m_shape;
    float m_lifeTimer;
};


/**
 * 子弹对象池
 * 预分配子弹数组，复用减少内存分配
 */
class BulletPool {
public:
    static constexpr size_t POOL_SIZE = 100;

    BulletPool();

    // 获取一个可用子弹
    Bullet* acquire();

    // 归还子弹
    void release(Bullet* bullet);

    // 更新所有活跃子弹
    void updateAll(float deltaTime);

    // 渲染所有活跃子弹
    void renderAll(sf::RenderWindow& window);

    // 获取活跃子弹列表
    std::vector<Bullet*> getActiveBullets() const;

    // 清空所有子弹
    void clear();

    size_t getActiveCount() const;

private:
    std::vector<Bullet> m_pool;
    std::vector<size_t> m_freeList;
};
