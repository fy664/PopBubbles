#include "states/MainMenuState.h"
#include "states/GameState.h"
#include "states/LeaderboardState.h"
#include "states/TutorialState.h"
#include "core/Game.h"
#include "core/ResourceManager.h"
#include "core/Input.h"

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
    m_titleText.setPosition(Game::WINDOW_WIDTH / 2.f, 150.f);

    // 创建按钮
    float centerX = Game::WINDOW_WIDTH / 2.f;
    float startY = 280.f;
    float spacing = 80.f;

    // 开始游戏按钮（origin 在中心，直接用 centerX）
    Button startBtn(L"开始游戏", font, 28,
                    sf::Vector2f(centerX, startY),
                    sf::Vector2f(240.f, 60.f));
    startBtn.setColors(sf::Color(40, 120, 40), sf::Color(50, 160, 50), sf::Color(30, 80, 30));
    startBtn.setTextColor(sf::Color::White);
    startBtn.setCallback([this]() { onStartGame(); });
    m_buttons.push_back(std::move(startBtn));

    // 新手教程按钮
    Button tutorialBtn(L"新手教程", font, 28,
                       sf::Vector2f(centerX, startY + spacing),
                       sf::Vector2f(240.f, 60.f));
    tutorialBtn.setColors(sf::Color(40, 80, 140), sf::Color(50, 100, 180), sf::Color(30, 60, 100));
    tutorialBtn.setTextColor(sf::Color::White);
    tutorialBtn.setCallback([this]() { onTutorial(); });
    m_buttons.push_back(std::move(tutorialBtn));

    // 排行榜按钮
    Button leaderBtn(L"排行榜", font, 28,
                     sf::Vector2f(centerX, startY + spacing * 2.f),
                     sf::Vector2f(240.f, 60.f));
    leaderBtn.setColors(sf::Color(80, 60, 20), sf::Color(120, 80, 30), sf::Color(60, 40, 15));
    leaderBtn.setTextColor(sf::Color::White);
    leaderBtn.setCallback([this]() { onLeaderboard(); });
    m_buttons.push_back(std::move(leaderBtn));

    // 退出按钮
    Button quitBtn(L"退出游戏", font, 28,
                   sf::Vector2f(centerX, startY + spacing * 3.f),
                   sf::Vector2f(240.f, 60.f));
    quitBtn.setColors(sf::Color(140, 40, 40), sf::Color(180, 50, 50), sf::Color(90, 30, 30));
    quitBtn.setTextColor(sf::Color::White);
    quitBtn.setCallback([this]() { onQuit(); });
    m_buttons.push_back(std::move(quitBtn));
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
}

void MainMenuState::render(sf::RenderWindow& window) {
    window.draw(m_titleText);

    for (auto& btn : m_buttons) {
        btn.render(window);
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
