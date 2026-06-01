#include "entities/Player.h"
#include "core/Input.h"
#include "core/Game.h"
#include <cmath>

Player::Player()
    : m_hp(MAX_HP)
    , m_invincible(false)
    , m_invincibleTimer(0.f)
    , m_blinkTimer(0.f)
    , m_needleAngle(0.f)
    , m_attacking(false)
    , m_attackTimer(0.f)
    , m_needleAnimProgress(0.f) {

    m_radius = RADIUS;
    m_position = {Game::WINDOW_WIDTH / 2.f, Game::WINDOW_HEIGHT / 2.f};

    // 玩家身体
    m_body.setRadius(RADIUS);
    m_body.setOrigin(RADIUS, RADIUS);
    m_body.setFillColor(sf::Color(80, 180, 255));
    m_body.setOutlineThickness(3.f);
    m_body.setOutlineColor(sf::Color(40, 120, 200));

    // 尖针 - 细长矩形
    m_needle.setSize({NEEDLE_LENGTH, NEEDLE_WIDTH});
    m_needle.setOrigin(0.f, NEEDLE_WIDTH / 2.f);
    m_needle.setFillColor(sf::Color(200, 200, 200));
    m_needle.setOutlineThickness(1.f);
    m_needle.setOutlineColor(sf::Color(150, 150, 150));

    // 针尖
    m_needleTip.setRadius(4.f);
    m_needleTip.setOrigin(4.f, 4.f);
    m_needleTip.setFillColor(sf::Color(255, 50, 50));
}

void Player::update(float deltaTime) {
    handleMovement(deltaTime);
    handleAttack(deltaTime);
    updateNeedle();

    // 无敌时间更新
    if (m_invincible) {
        m_invincibleTimer -= deltaTime;
        m_blinkTimer += deltaTime;
        if (m_invincibleTimer <= 0.f) {
            m_invincible = false;
            m_body.setFillColor(sf::Color(80, 180, 255));
        }
    }
}

void Player::handleMovement(float deltaTime) {
    sf::Vector2f moveDir(0.f, 0.f);

    if (Input::isKeyDown(sf::Keyboard::W)) moveDir.y -= 1.f;
    if (Input::isKeyDown(sf::Keyboard::S)) moveDir.y += 1.f;
    if (Input::isKeyDown(sf::Keyboard::A)) moveDir.x -= 1.f;
    if (Input::isKeyDown(sf::Keyboard::D)) moveDir.x += 1.f;

    // 归一化
    float len = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
    if (len > 0.0001f) {
        moveDir.x /= len;
        moveDir.y /= len;
    }

    m_position.x += moveDir.x * SPEED * deltaTime;
    m_position.y += moveDir.y * SPEED * deltaTime;

    // 边界检测
    clampToScreen(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);

    m_body.setPosition(m_position);
}

void Player::handleAttack(float deltaTime) {
    // 检查左键点击，触发攻击
    if (Input::isMousePressed(sf::Mouse::Left) && !m_attacking) {
        m_attacking = true;
        m_attackTimer = NEEDLE_ATTACK_DURATION;
    }

    // 攻击动画计时
    if (m_attacking) {
        m_attackTimer -= deltaTime;
        // 动画：前0.15秒伸出，后0.15秒收回
        float halfDur = NEEDLE_ATTACK_DURATION / 2.f;
        if (m_attackTimer > halfDur) {
            // 伸出阶段
            float t = 1.f - (m_attackTimer - halfDur) / halfDur;
            m_needleAnimProgress = t;
        } else if (m_attackTimer > 0.f) {
            // 收回阶段
            float t = m_attackTimer / halfDur;
            m_needleAnimProgress = t;
        }

        if (m_attackTimer <= 0.f) {
            m_attacking = false;
            m_needleAnimProgress = 0.f;
        }
    }
}

void Player::updateNeedle() {
    auto& window = Game::instance->getWindow();
    sf::Vector2f mouseWorld = Input::getMouseWorld(window);

    // 计算尖针朝向角度
    float dx = mouseWorld.x - m_position.x;
    float dy = mouseWorld.y - m_position.y;
    m_needleAngle = std::atan2(dy, dx);

    // 更新尖针位置和旋转
    m_needle.setPosition(m_position);
    m_needle.setRotation(m_needleAngle * 180.f / 3.14159265f);

    // 针尖位置
    sf::Vector2f tip = getNeedleTip();
    m_needleTip.setPosition(tip);
}

void Player::render(sf::RenderWindow& window) {
    // 无敌闪烁效果
    if (m_invincible) {
        float blink = std::sin(m_blinkTimer * 15.f) * 0.5f + 0.5f;
        m_body.setFillColor(sf::Color(
            static_cast<sf::Uint8>(80 + 100 * blink),
            static_cast<sf::Uint8>(180 + 50 * blink),
            static_cast<sf::Uint8>(255),
            static_cast<sf::Uint8>(128 + 127 * blink)
        ));
    }

    window.draw(m_needle);
    window.draw(m_needleTip);
    window.draw(m_body);
}

void Player::takeDamage() {
    if (m_invincible) return;

    m_hp--;
    m_invincible = true;
    m_invincibleTimer = INVINCIBLE_TIME;
    m_blinkTimer = 0.f;
}

void Player::heal() {
    if (m_hp < MAX_HP) {
        m_hp++;
    }
}

sf::Vector2f Player::getNeedleTip() const {
    float len = getCurrentNeedleLength();
    return {
        m_position.x + std::cos(m_needleAngle) * len,
        m_position.y + std::sin(m_needleAngle) * len
    };
}

std::pair<sf::Vector2f, sf::Vector2f> Player::getNeedleLine() const {
    // 针从球体内部中心伸出
    sf::Vector2f start = m_position;
    sf::Vector2f end = getNeedleTip();
    return {start, end};
}

float Player::getCurrentNeedleLength() const {
    float baseLen = NEEDLE_LENGTH;  // 从球心算起的针长
    float extendLen = NEEDLE_EXTEND * m_needleAnimProgress;
    return baseLen + extendLen;
}

void Player::addKill() {
    m_kills++;
    int newLevel = m_kills / KILLS_PER_LEVEL + 1;
    if (newLevel > m_level) {
        m_level = newLevel;
        healFull();  // 升级回满血
    }
}

void Player::applyRecoil(sf::Vector2f from, float strength) {
    sf::Vector2f dir = m_position - from;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.0001f) {
        dir.x /= len;
        dir.y /= len;
    }
    m_position.x += dir.x * strength;
    m_position.y += dir.y * strength;
    clampToScreen(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
    m_body.setPosition(m_position);
}

void Player::setNeedleAngle(float angle) {
    m_needleAngle = angle;
}
