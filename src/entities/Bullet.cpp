#include "entities/Bullet.h"
#include "core/Game.h"
#include <algorithm>

// ---------- Bullet ----------

Bullet::Bullet()
    : m_lifeTimer(0.f) {
    m_radius = RADIUS;
    m_active = false;

    m_shape.setRadius(RADIUS);
    m_shape.setOrigin(RADIUS, RADIUS);
    m_shape.setFillColor(sf::Color(200, 50, 200));
    m_shape.setOutlineThickness(1.f);
    m_shape.setOutlineColor(sf::Color(150, 30, 150));
}

void Bullet::update(float deltaTime) {
    if (!m_active) return;

    m_lifeTimer += deltaTime;
    if (m_lifeTimer >= LIFETIME) {
        deactivate();
        return;
    }

    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;

    m_shape.setPosition(m_position);

    // 离开屏幕则失效
    if (isOffScreen(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, 50.f)) {
        deactivate();
    }
}

void Bullet::render(sf::RenderWindow& window) {
    if (!m_active) return;
    window.draw(m_shape);
}

void Bullet::fire(sf::Vector2f position, sf::Vector2f direction) {
    m_position = position;
    m_velocity = direction * SPEED;
    m_active = true;
    m_lifeTimer = 0.f;
    m_shape.setPosition(m_position);
}

void Bullet::deactivate() {
    m_active = false;
    m_lifeTimer = 0.f;
}


// ---------- BulletPool ----------

BulletPool::BulletPool() {
    m_pool.resize(POOL_SIZE);
    for (size_t i = 0; i < POOL_SIZE; ++i) {
        m_freeList.push_back(i);
    }
}

Bullet* BulletPool::acquire() {
    if (m_freeList.empty()) return nullptr;

    size_t index = m_freeList.back();
    m_freeList.pop_back();
    m_pool[index].deactivate();
    return &m_pool[index];
}

void BulletPool::release(Bullet* bullet) {
    if (!bullet) return;
    bullet->deactivate();

    // 找到在池中的索引
    size_t index = bullet - &m_pool[0];
    if (index < POOL_SIZE) {
        m_freeList.push_back(index);
    }
}

void BulletPool::updateAll(float deltaTime) {
    for (auto& bullet : m_pool) {
        if (bullet.isActive()) {
            bullet.update(deltaTime);
            if (!bullet.isActive()) {
                size_t index = &bullet - &m_pool[0];
                if (std::find(m_freeList.begin(), m_freeList.end(), index) == m_freeList.end()) {
                    m_freeList.push_back(index);
                }
            }
        }
    }
}

void BulletPool::renderAll(sf::RenderWindow& window) {
    for (auto& bullet : m_pool) {
        if (bullet.isActive()) {
            bullet.render(window);
        }
    }
}

std::vector<Bullet*> BulletPool::getActiveBullets() const {
    std::vector<Bullet*> active;
    for (auto& bullet : m_pool) {
        if (bullet.isActive()) {
            active.push_back(const_cast<Bullet*>(&bullet));
        }
    }
    return active;
}

void BulletPool::clear() {
    for (auto& bullet : m_pool) {
        bullet.deactivate();
    }
    m_freeList.clear();
    for (size_t i = 0; i < POOL_SIZE; ++i) {
        m_freeList.push_back(i);
    }
}

size_t BulletPool::getActiveCount() const {
    return std::count_if(m_pool.begin(), m_pool.end(),
        [](const Bullet& b) { return b.isActive(); });
}
