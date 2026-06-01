#include "states/MainMenuState.h"
#include "states/GameState.h"
#include "states/LeaderboardState.h"
#include "states/TutorialState.h"
#include "core/Game.h"
#include "core/ResourceManager.h"
#include "core/Input.h"
#include <random>
#include <cmath>

MainMenuState::MainMenuState() {
    auto& font = ResourceManager::getInstance().getFont("default");

    // 标题文字
    m_titleText.setFont(font);
    m_titleText.setString(L"Pop Bubbles - 戳泡泡");
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color(255, 220, 100));
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(titleBounds.left + titleBounds.width / 2.f,
                          titleBounds.top + titleBounds.height / 2.f);
    m_titleText.setPosition(Game::WINDOW_WIDTH / 2.f, 180.f);

    // 创建按钮
    float centerX = Game::WINDOW_WIDTH / 2.f;
    float startY = 320.f;
    float spacing = 85.f;

    // 开始游戏按钮 — 绿色泡泡
    Button startBtn(L"开始游戏", font, 28,
                    sf::Vector2f(centerX, startY),
                    sf::Vector2f(260.f, 65.f));
    startBtn.setColors(sf::Color(80, 200, 120), sf::Color(120, 240, 160), sf::Color(50, 150, 90));
    startBtn.setTextColor(sf::Color(20, 60, 30));
    startBtn.setOutlineThickness(3.f);
    startBtn.setCallback([this]() { onStartGame(); });
    m_buttons.push_back(std::move(startBtn));

    // 新手教程按钮 — 蓝色泡泡
    Button tutorialBtn(L"新手教程", font, 28,
                       sf::Vector2f(centerX, startY + spacing),
                       sf::Vector2f(260.f, 65.f));
    tutorialBtn.setColors(sf::Color(80, 180, 240), sf::Color(130, 210, 255), sf::Color(50, 130, 190));
    tutorialBtn.setTextColor(sf::Color(20, 40, 80));
    tutorialBtn.setOutlineThickness(3.f);
    tutorialBtn.setCallback([this]() { onTutorial(); });
    m_buttons.push_back(std::move(tutorialBtn));

    // 排行榜按钮 — 金色泡泡
    Button leaderBtn(L"排行榜", font, 28,
                     sf::Vector2f(centerX, startY + spacing * 2.f),
                     sf::Vector2f(260.f, 65.f));
    leaderBtn.setColors(sf::Color(240, 200, 80), sf::Color(255, 230, 130), sf::Color(190, 150, 50));
    leaderBtn.setTextColor(sf::Color(80, 50, 10));
    leaderBtn.setOutlineThickness(3.f);
    leaderBtn.setCallback([this]() { onLeaderboard(); });
    m_buttons.push_back(std::move(leaderBtn));

    // 退出按钮 — 粉色泡泡
    Button quitBtn(L"退出游戏", font, 28,
                   sf::Vector2f(centerX, startY + spacing * 3.f),
                   sf::Vector2f(260.f, 65.f));
    quitBtn.setColors(sf::Color(240, 120, 140), sf::Color(255, 160, 180), sf::Color(190, 80, 100));
    quitBtn.setTextColor(sf::Color(80, 20, 30));
    quitBtn.setOutlineThickness(3.f);
    quitBtn.setCallback([this]() { onQuit(); });
    m_buttons.push_back(std::move(quitBtn));

    initDecoBubbles();
}

void MainMenuState::onEnter() {
    // 状态进入时无特殊操作
}

void MainMenuState::handleInput(sf::RenderWindow& window) {
    sf::Vector2f mousePos = Input::getMouseWorld(window);

    if (Input::isMousePressed(sf::Mouse::Left)) {
        for (auto& btn : m_buttons) {
            if (btn.contains(mousePos)) {
                btn.handleClick(mousePos);
                return;  // State may have changed, stop processing
            }
        }
    }
}

void MainMenuState::update(float deltaTime) {
    auto& window = Game::instance->getWindow();
    sf::Vector2f mousePos = Input::getMouseWorld(window);

    for (auto& btn : m_buttons) {
        btn.update(mousePos);
    }

    // Drift decorative bubbles
    for (auto& db : m_decoBubbles) {
        auto pos = db.shape.getPosition();
        pos.x += db.drift.x * deltaTime;
        pos.y += db.drift.y * deltaTime;
        // Wrap around screen
        if (pos.x > Game::WINDOW_WIDTH + 60.f) pos.x = -60.f;
        if (pos.x < -60.f) pos.x = Game::WINDOW_WIDTH + 60.f;
        if (pos.y > Game::WINDOW_HEIGHT + 60.f) pos.y = -60.f;
        if (pos.y < -60.f) pos.y = Game::WINDOW_HEIGHT + 60.f;
        db.shape.setPosition(pos);
    }
}

void MainMenuState::render(sf::RenderWindow& window) {
    // 背景装饰泡泡
    for (const auto& db : m_decoBubbles) {
        window.draw(db.shape);
    }

    window.draw(m_titleText);

    for (auto& btn : m_buttons) {
        btn.render(window);
    }
}

void MainMenuState::initDecoBubbles() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> xDist(0.f, static_cast<float>(Game::WINDOW_WIDTH));
    std::uniform_real_distribution<float> yDist(0.f, static_cast<float>(Game::WINDOW_HEIGHT));
    std::uniform_real_distribution<float> rDist(15.f, 55.f);
    std::uniform_real_distribution<float> driftDist(-25.f, 25.f);
    std::uniform_int_distribution<int> alphaDist(20, 60);

    sf::Color colors[] = {
        sf::Color(255, 150, 180, 40),
        sf::Color(150, 200, 255, 40),
        sf::Color(180, 255, 180, 40),
        sf::Color(255, 220, 150, 40),
        sf::Color(200, 160, 255, 40),
        sf::Color(150, 255, 220, 40),
    };

    for (int i = 0; i < 20; ++i) {
        DecoBubble db;
        float r = rDist(rng);
        db.shape.setRadius(r);
        db.shape.setOrigin(r, r);
        db.shape.setPosition({xDist(rng), yDist(rng)});
        db.shape.setFillColor(colors[i % 6]);
        db.shape.setOutlineThickness(1.f);
        db.shape.setOutlineColor(sf::Color(255, 255, 255, 20));
        db.drift = {driftDist(rng), driftDist(rng)};
        m_decoBubbles.push_back(db);
    }
}

void MainMenuState::onStartGame() {
    Game::instance->changeState(std::make_unique<GameState>());
}

void MainMenuState::onLeaderboard() {
    Game::instance->pushState(std::make_unique<LeaderboardState>());
}

void MainMenuState::onTutorial() {
    Game::instance->pushState(std::make_unique<TutorialState>());
}

void MainMenuState::onQuit() {
    Game::instance->quit();
}
