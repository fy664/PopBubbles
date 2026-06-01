#pragma once
#include <SFML/Graphics.hpp>

/**
 * 游戏实体基类
 * 所有游戏对象的共同基类
 */
class Entity {
public:
    Entity();
    virtual ~Entity() = default;

    // 更新逻辑
    virtual void update(float deltaTime) = 0;

    // 渲染
    virtual void render(sf::RenderWindow& window) = 0;

    // 获取/设置位置
    sf::Vector2f getPosition() const { return m_position; }
    void setPosition(sf::Vector2f pos) { m_position = pos; }
    void setPosition(float x, float y) { m_position = {x, y}; }

    // 获取/设置速度
    sf::Vector2f getVelocity() const { return m_velocity; }
    void setVelocity(sf::Vector2f vel) { m_velocity = vel; }
    void setVelocity(float x, float y) { m_velocity = {x, y}; }

    // 获取半径
    float getRadius() const { return m_radius; }
    void setRadius(float r) { m_radius = r; }

    // 是否活跃
    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }

    // 将实体约束在屏幕边界内
    // 返回是否发生了边界碰撞
    bool clampToScreen(unsigned int screenW, unsigned int screenH);

    // 是否在屏幕外（带边距）
    bool isOffScreen(unsigned int screenW, unsigned int screenH, float margin = 50.f) const;

    // 获取朝向某点的方向向量（归一化）
    sf::Vector2f directionTo(sf::Vector2f target) const;

    // 获取与某点的距离
    float distanceTo(sf::Vector2f target) const;

protected:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_radius;
    bool m_active;
};
