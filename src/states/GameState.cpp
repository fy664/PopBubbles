#include "states/GameState.h"
#include "states/GameOverState.h"
#include "core/Game.h"
#include "core/ResourceManager.h"
#include "core/Input.h"
#include "core/Audio.h"
#include "entities/Collision.h"
#include <random>
#include <cmath>

static std::random_device rd;
static std::mt19937 rng(rd());

// 在屏幕边缘外随机位置生成泡泡
static sf::Vector2f randomEdgePosition() {
    std::uniform_real_distribution<float> xDist(-50.f, Game::WINDOW_WIDTH + 50.f);
    std::uniform_real_distribution<float> yDist(-50.f, Game::WINDOW_HEIGHT + 50.f);
    std::uniform_int_distribution<int> edgeDist(0, 3);

    // 随机选择在哪个边缘生成
    int edge = edgeDist(rng);
    float x, y;
    switch (edge) {
        case 0: // 上边缘
            x = xDist(rng);
            y = -50.f;
            break;
        case 1: // 下边缘
            x = xDist(rng);
            y = Game::WINDOW_HEIGHT + 50.f;
            break;
        case 2: // 左边缘
            x = -50.f;
            y = yDist(rng);
            break;
        case 3: // 右边缘
        default:
            x = Game::WINDOW_WIDTH + 50.f;
            y = yDist(rng);
            break;
    }
    return {x, y};
}

GameState::GameState()
    : m_spawnTimer(0.f)
    , m_spawnInterval(1.5f)
    , m_difficultyTimer(0.f)
    , m_difficultyLevel(1)
    , m_score(0)
    , m_screenShake(0.f) {

    auto& font = ResourceManager::getInstance().getFont("default");

    m_scoreText.setFont(font);
    m_scoreText.setCharacterSize(24);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setPosition(10.f, 10.f);

    m_hpText.setFont(font);
    m_hpText.setCharacterSize(24);
    m_hpText.setFillColor(sf::Color(255, 100, 100));
    m_hpText.setPosition(10.f, 40.f);

    m_waveText.setFont(font);
    m_waveText.setCharacterSize(20);
    m_waveText.setFillColor(sf::Color(200, 200, 100));
    m_waveText.setPosition(Game::WINDOW_WIDTH - 200.f, 10.f);
}

void GameState::onEnter() {
    // 重新初始化玩家
    m_player = Player();
    m_bubbles.clear();
    m_shooterBubbles.clear();
    m_giantBubbles.clear();
    m_bulletPool.clear();
    m_particles.clear();
    m_score = 0;
    m_difficultyLevel = 1;
    m_spawnTimer = 0.f;
    m_spawnInterval = 1.5f;
    m_difficultyTimer = 0.f;
    m_screenShake = 0.f;
}

void GameState::handleInput(sf::RenderWindow& window) {
    // ESC 暂停/返回菜单
    if (Input::isKeyPressed(sf::Keyboard::Escape)) {
        Game::instance->popState();
    }
}

void GameState::update(float deltaTime) {
    // 玩家更新
    m_player.update(deltaTime);

    // 玩家死亡检测
    if (m_player.isDead()) {
        Audio::getInstance().playGameOver();
        Audio::getInstance().stopMusic();
        auto gameOverState = std::make_unique<GameOverState>();
        gameOverState->setScore(m_score);
        Game::instance->changeState(std::move(gameOverState));
        return;
    }

    // 敌人生成
    spawnEnemy(deltaTime);

    // 更新所有敌人
    updateEnemies(deltaTime);

    // 更新子弹
    updateBullets(deltaTime);

    // 碰撞检测
    checkCollisions();

    // 更新粒子特效
    for (auto it = m_particles.begin(); it != m_particles.end();) {
        it->lifetime -= deltaTime;
        it->shape.setPosition(
            it->shape.getPosition().x + it->velocity.x * deltaTime,
            it->shape.getPosition().y + it->velocity.y * deltaTime
        );
        // 淡出
        float alpha = it->lifetime / it->maxLifetime;
        sf::Color c = it->shape.getFillColor();
        c.a = static_cast<sf::Uint8>(255 * alpha);
        it->shape.setFillColor(c);

        if (it->lifetime <= 0.f) {
            it = m_particles.erase(it);
        } else {
            ++it;
        }
    }

    // Update floating texts
    for (auto it = m_floatingTexts.begin(); it != m_floatingTexts.end();) {
        it->lifetime -= deltaTime;
        it->text.move(0.f, -60.f * deltaTime);  // Float upward
        float alpha = it->lifetime / it->maxLifetime;
        sf::Color c = it->text.getFillColor();
        c.a = static_cast<sf::Uint8>(255 * alpha);
        it->text.setFillColor(c);

        if (it->lifetime <= 0.f) {
            it = m_floatingTexts.erase(it);
        } else {
            ++it;
        }
    }

    // Kill streak timer
    m_streakTimer -= deltaTime;
    if (m_streakTimer <= 0.f) {
        m_killStreak = 0;
    }

    // 屏幕震动衰减
    if (m_screenShake > 0.f) {
        m_screenShake -= deltaTime;
    }

    // 难度递增
    m_difficultyTimer += deltaTime;
    if (m_difficultyTimer >= 7.5f) {  // 难度翻倍速度
        m_difficultyTimer = 0.f;
        m_difficultyLevel++;
        m_spawnInterval = std::max(0.25f, m_spawnInterval - 0.15f);
    }

    updateUI();
}

void GameState::updateEnemies(float deltaTime) {
    sf::Vector2f playerPos = m_player.getPosition();

    // 更新普通泡泡
    for (auto& bubble : m_bubbles) {
        if (!bubble->isActive()) continue;
        bubble->setTarget(playerPos);
        bubble->update(deltaTime);
    }

    // 更新射手泡泡
    for (auto& sb : m_shooterBubbles) {
        if (!sb->isActive()) continue;
        sb->setTarget(playerPos);
        sb->update(deltaTime);

        // 射击逻辑
        if (sb->shouldShoot()) {
            Bullet* bullet = m_bulletPool.acquire();
            if (bullet) {
                sf::Vector2f dir = sb->directionTo(playerPos);
                bullet->fire(sb->getPosition(), dir);
                Audio::getInstance().playShoot();
            }
            sb->resetShootTimer();
        }
    }

    // 更新巨型泡泡
    for (auto& gb : m_giantBubbles) {
        if (!gb->isActive()) continue;
        gb->setTarget(playerPos);
        gb->update(deltaTime);
    }

    // 清理超出屏幕的泡泡
    auto cleanupInactive = [](auto& container) {
        container.erase(
            std::remove_if(container.begin(), container.end(),
                [](auto& ptr) {
                    return !ptr->isActive() || ptr->isOffScreen(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, 100.f);
                }),
            container.end()
        );
    };
    cleanupInactive(m_bubbles);
    cleanupInactive(m_shooterBubbles);
    cleanupInactive(m_giantBubbles);
}

void GameState::updateBullets(float deltaTime) {
    m_bulletPool.updateAll(deltaTime);
}

void GameState::checkCollisions() {
    sf::Vector2f playerPos = m_player.getPosition();
    float playerRadius = m_player.getRadius();

    // 玩家尖针攻击检测
    if (m_player.isAttacking()) {
        auto [needleStart, needleEnd] = m_player.getNeedleLine();

        auto onKill = [this](sf::Vector2f pos, int score, sf::Color color) {
            m_killStreak++;
            m_streakTimer = STREAK_WINDOW;
            m_player.addKill();  // 玩家击杀计数+等级系统
            int bonusScore = score;
            std::wstring text = L"+" + std::to_wstring(score);
            if (m_killStreak >= 5) {
                bonusScore += m_killStreak * 2;  // Streak bonus
                text = L"+" + std::to_wstring(score) + L" x" + std::to_wstring(m_killStreak) + L"!";
            }
            m_score += bonusScore;
            spawnEffect(pos, color);
            spawnFloatingText(pos + sf::Vector2f(0.f, -20.f), text,
                              m_killStreak >= 3 ? sf::Color(255, 255, 100) : sf::Color::White);
            Audio::getInstance().playBubblePop();
        };

        // 检查普通泡泡
        for (auto& bubble : m_bubbles) {
            if (!bubble->isActive() || !bubble->canBeHit()) continue;
            if (Collision::lineCircle(needleStart, needleEnd,
                                       bubble->getPosition(), bubble->getRadius())) {
                bubble->markHit();
                onKill(bubble->getPosition(), bubble->getScoreValue(), bubble->getColor());
                bubble->setActive(false);
            }
        }

        // 检查射手泡泡
        for (auto& sb : m_shooterBubbles) {
            if (!sb->isActive() || !sb->canBeHit()) continue;
            if (Collision::lineCircle(needleStart, needleEnd,
                                       sb->getPosition(), sb->getRadius())) {
                sb->markHit();
                onKill(sb->getPosition(), ShooterBubble::SCORE_VALUE, sb->getColor());
                sb->setActive(false);
            }
        }

        // 检查巨型泡泡
        for (auto& gb : m_giantBubbles) {
            if (!gb->isActive() || !gb->canBeHit()) continue;
            if (Collision::lineCircle(needleStart, needleEnd,
                                       gb->getPosition(), gb->getRadius())) {
                gb->takeDamage();
                // 巨型泡泡碰撞回弹
                m_player.applyRecoil(gb->getPosition(), 15.f);
                m_screenShake = std::max(m_screenShake, 0.15f);
                if (!gb->isActive()) {
                    onKill(gb->getPosition(), GiantBubble::SCORE_VALUE, gb->getColor());
                } else {
                    spawnEffect(gb->getPosition(), sf::Color(255, 200, 150));
                    spawnFloatingText(gb->getPosition(), L"1/2", sf::Color(255, 200, 150));
                }
            }
        }
    }

    // 泡泡与玩家碰撞检测
    if (!m_player.isInvincible()) {
        auto damagePlayer = [this](sf::Vector2f pos, sf::Color color, float shake) {
            m_player.takeDamage();
            Audio::getInstance().playPlayerHurt();
            spawnEffect(pos, color);
            m_screenShake = shake;
        };

        for (auto& bubble : m_bubbles) {
            if (!bubble->isActive()) continue;
            if (Collision::circleCircle(playerPos, playerRadius,
                                         bubble->getPosition(), bubble->getRadius())) {
                damagePlayer(bubble->getPosition(), bubble->getColor(), 0.2f);
                bubble->setActive(false);
                break;
            }
        }

        for (auto& sb : m_shooterBubbles) {
            if (!sb->isActive()) continue;
            if (Collision::circleCircle(playerPos, playerRadius,
                                         sb->getPosition(), sb->getRadius())) {
                damagePlayer(sb->getPosition(), sb->getColor(), 0.2f);
                sb->setActive(false);
                break;
            }
        }

        for (auto& gb : m_giantBubbles) {
            if (!gb->isActive()) continue;
            if (Collision::circleCircle(playerPos, playerRadius,
                                         gb->getPosition(), gb->getRadius())) {
                damagePlayer(gb->getPosition(), gb->getColor(), 0.3f);
                gb->setActive(false);
                break;
            }
        }
    }

    // 泡泡间碰撞检测（弹开效果）
    auto bubbleBounce = [](auto& bubbles) {
        for (size_t i = 0; i < bubbles.size(); ++i) {
            if (!bubbles[i]->isActive()) continue;
            for (size_t j = i + 1; j < bubbles.size(); ++j) {
                if (!bubbles[j]->isActive()) continue;
                if (Collision::circleCircle(bubbles[i]->getPosition(), bubbles[i]->getRadius(),
                                             bubbles[j]->getPosition(), bubbles[j]->getRadius())) {
                    // Push apart
                    sf::Vector2f diff = bubbles[i]->getPosition() - bubbles[j]->getPosition();
                    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                    if (dist < 0.001f) { diff = {1.f, 0.f}; dist = 1.f; }
                    sf::Vector2f normal = diff / dist;
                    float overlap = (bubbles[i]->getRadius() + bubbles[j]->getRadius()) - dist;
                    float push = overlap * 0.5f + 1.f;
                    bubbles[i]->setPosition(bubbles[i]->getPosition() + normal * push);
                    bubbles[j]->setPosition(bubbles[j]->getPosition() - normal * push);
                }
            }
        }
    };
    bubbleBounce(m_bubbles);
    bubbleBounce(m_shooterBubbles);
    bubbleBounce(m_giantBubbles);
    // Cross-type collisions
    for (auto& a : m_bubbles) {
        if (!a->isActive()) continue;
        for (auto& sb : m_shooterBubbles) {
            if (!sb->isActive()) continue;
            if (Collision::circleCircle(a->getPosition(), a->getRadius(),
                                         sb->getPosition(), sb->getRadius())) {
                sf::Vector2f diff = a->getPosition() - sb->getPosition();
                float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                if (dist < 0.001f) { diff = {1.f, 0.f}; dist = 1.f; }
                sf::Vector2f normal = diff / dist;
                float overlap = (a->getRadius() + sb->getRadius()) - dist;
                float push = overlap * 0.5f + 1.f;
                a->setPosition(a->getPosition() + normal * push);
                sb->setPosition(sb->getPosition() - normal * push);
            }
        }
    }

    // 子弹与玩家碰撞检测
    if (!m_player.isInvincible()) {
        auto activeBullets = m_bulletPool.getActiveBullets();
        for (auto* bullet : activeBullets) {
            if (!bullet->isActive()) continue;
            if (Collision::circleCircle(playerPos, playerRadius,
                                         bullet->getPosition(), bullet->getRadius())) {
                m_player.takeDamage();
                Audio::getInstance().playPlayerHurt();
                spawnEffect(bullet->getPosition(), sf::Color(200, 50, 200));
                bullet->deactivate();
                m_screenShake = 0.15f;
                break;
            }
        }
    }
}

void GameState::spawnEnemy(float deltaTime) {
    m_spawnTimer += deltaTime;

    if (m_spawnTimer >= m_spawnInterval) {
        m_spawnTimer = 0.f;

        sf::Vector2f pos = randomEdgePosition();
        sf::Vector2f playerPos = m_player.getPosition();

        // 根据难度等级决定生成什么类型的泡泡
        // 难度1-2: 只有普通泡泡
        // 难度3-4: 普通泡泡 + 射手泡泡
        // 难度5+: 普通泡泡 + 射手泡泡 + 巨型泡泡
        std::uniform_real_distribution<float> typeDist(0.f, 1.f);
        float roll = typeDist(rng);

        if (m_difficultyLevel >= 5 && roll < 0.1f) {
            // 10% 概率生成巨型泡泡
            auto gb = std::make_unique<GiantBubble>();
            gb->setPosition(pos);
            gb->setTarget(playerPos);
            m_giantBubbles.push_back(std::move(gb));
        } else if (m_difficultyLevel >= 3 && roll < 0.35f) {
            // 25% 概率生成射手泡泡（扣除巨型泡泡的概率后）
            auto sb = std::make_unique<ShooterBubble>();
            sb->setPosition(pos);
            sb->setTarget(playerPos);
            m_shooterBubbles.push_back(std::move(sb));
        } else {
            auto bubble = std::make_unique<Bubble>();
            bubble->setPosition(pos);
            bubble->setTarget(playerPos);
            m_bubbles.push_back(std::move(bubble));
        }
    }
}

void GameState::updateUI() {
    m_scoreText.setString(L"得分: " + std::to_wstring(m_score));
    m_hpText.setString(L"生命: " + std::to_wstring(m_player.getHP()) + L"/" + std::to_wstring(Player::MAX_HP) +
                       L"  Lv." + std::to_wstring(m_player.getLevel()));
    m_waveText.setString(L"难度: " + std::to_wstring(m_difficultyLevel));
}

void GameState::render(sf::RenderWindow& window) {
    // 屏幕震动偏移
    sf::View originalView = window.getView();
    float shakeX = 0.f, shakeY = 0.f;
    if (m_screenShake > 0.f) {
        std::uniform_real_distribution<float> shakeDist(-3.f, 3.f);
        shakeX = shakeDist(rng) * m_screenShake * 10.f;
        shakeY = shakeDist(rng) * m_screenShake * 10.f;
        sf::View shaken = originalView;
        shaken.move(shakeX, shakeY);
        window.setView(shaken);
    }

    // 渲染粒子
    for (const auto& p : m_particles) {
        window.draw(p.shape);
    }

    // 渲染浮动文字
    for (const auto& ft : m_floatingTexts) {
        window.draw(ft.text);
    }

    // 渲染普通泡泡
    for (auto& b : m_bubbles) {
        b->render(window);
    }

    // 渲染射手泡泡
    for (auto& sb : m_shooterBubbles) {
        sb->render(window);
    }

    // 渲染巨型泡泡
    for (auto& gb : m_giantBubbles) {
        gb->render(window);
    }

    // 渲染子弹
    m_bulletPool.renderAll(window);

    // 渲染玩家
    m_player.render(window);

    // 恢复视图
    window.setView(originalView);

    // 渲染UI（不受震动影响）
    window.draw(m_scoreText);
    window.draw(m_hpText);
    window.draw(m_waveText);
}

sf::Vector2f GameState::getPlayerPosition() const {
    return m_player.getPosition();
}

void GameState::spawnEffect(sf::Vector2f position, sf::Color color) {
    std::uniform_real_distribution<float> angleDist(0.f, 2.f * 3.14159265f);
    std::uniform_real_distribution<float> speedDist(100.f, 250.f);

    for (int i = 0; i < 8; ++i) {
        Particle p;
        p.shape.setRadius(3.f);
        p.shape.setOrigin(3.f, 3.f);
        p.shape.setFillColor(color);
        p.shape.setPosition(position);

        float angle = angleDist(rng);
        float speed = speedDist(rng);
        p.velocity = {std::cos(angle) * speed, std::sin(angle) * speed};
        p.lifetime = 0.5f;
        p.maxLifetime = 0.5f;

        m_particles.push_back(p);
    }
}

void GameState::spawnFloatingText(sf::Vector2f position, const std::wstring& text, sf::Color color) {
    FloatingText ft;
    ft.text.setFont(ResourceManager::getInstance().getFont("default"));
    ft.text.setString(text);
    ft.text.setCharacterSize(20);
    ft.text.setFillColor(color);
    ft.text.setPosition(position);
    ft.lifetime = 0.8f;
    ft.maxLifetime = 0.8f;
    m_floatingTexts.push_back(ft);
}
