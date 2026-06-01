#include "states/GameOverState.h"
#include "states/MainMenuState.h"
#include "system/Leaderboard.h"
#include "core/Game.h"
#include "core/ResourceManager.h"
#include "core/Input.h"

GameOverState::GameOverState()
    : m_score(0)
    , m_submitted(false)
    , m_hasShownStatus(false) {

    auto& font = ResourceManager::getInstance().getFont("default");

    m_titleText.setFont(font);
    m_titleText.setString(L"游戏结束");
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color(255, 100, 100));
    sf::FloatRect tb = m_titleText.getLocalBounds();
    m_titleText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    m_titleText.setPosition(Game::WINDOW_WIDTH / 2.f, 100.f);

    m_scoreText.setFont(font);
    m_scoreText.setCharacterSize(32);
    m_scoreText.setFillColor(sf::Color(255, 220, 100));
    m_scoreText.setPosition(Game::WINDOW_WIDTH / 2.f, 160.f);

    m_statusText.setFont(font);
    m_statusText.setCharacterSize(20);
    m_statusText.setFillColor(sf::Color(100, 255, 100));
    m_statusText.setPosition(Game::WINDOW_WIDTH / 2.f, 420.f);

    // 姓名输入框
    m_nameInput = TextBox(font, 28,
                          sf::Vector2f(Game::WINDOW_WIDTH / 2.f - 200.f, 230.f),
                          sf::Vector2f(400.f, 50.f), 10);
    m_nameInput.setPlaceholder(L"输入你的名字...");

    m_nameInput.setCallback([this](const std::string&) {
        onSubmitScore();
    });

    // 提交按钮
    m_submitBtn = Button(L"提交成绩", font, 24,
                         sf::Vector2f(Game::WINDOW_WIDTH / 2.f, 310.f),
                         sf::Vector2f(240.f, 50.f));
    m_submitBtn.setColors(sf::Color(40, 120, 40), sf::Color(50, 160, 50), sf::Color(30, 80, 30));
    m_submitBtn.setTextColor(sf::Color::White);
    m_submitBtn.setCallback([this]() { onSubmitScore(); });

    // 返回主菜单按钮
    m_returnBtn = Button(L"返回主菜单", font, 24,
                         sf::Vector2f(Game::WINDOW_WIDTH / 2.f, 490.f),
                         sf::Vector2f(240.f, 50.f));
    m_returnBtn.setColors(sf::Color(100, 60, 30), sf::Color(140, 80, 40), sf::Color(70, 40, 20));
    m_returnBtn.setTextColor(sf::Color::White);
    m_returnBtn.setCallback([this]() { onReturnToMenu(); });
}

void GameOverState::handleEvent(const sf::Event& event) {
    if (!m_submitted && event.type == sf::Event::TextEntered && m_nameInput.isFocused()) {
        m_nameInput.handleTextEntered(event.text.unicode);
    }
}

void GameOverState::handleInput(sf::RenderWindow& window) {
    sf::Vector2f mousePos = Input::getMouseWorld(window);

    if (!m_submitted) {
        m_nameInput.handleKeyInput();

        if (Input::isMousePressed(sf::Mouse::Left) && m_submitBtn.contains(mousePos)) {
            m_submitBtn.handleClick(mousePos);
            return;
        }
    }

    if (Input::isMousePressed(sf::Mouse::Left) && m_returnBtn.contains(mousePos)) {
        m_returnBtn.handleClick(mousePos);
        return;
    }
}

void GameOverState::update(float deltaTime) {
    auto& window = Game::instance->getWindow();
    sf::Vector2f mousePos = Input::getMouseWorld(window);

    m_nameInput.update(mousePos, Input::isMousePressed(sf::Mouse::Left));
    m_submitBtn.update(mousePos);
    m_returnBtn.update(mousePos);

    m_scoreText.setString(L"最终得分: " + std::to_wstring(m_score));
    sf::FloatRect sb = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(sb.left + sb.width / 2.f, 0.f);
}

void GameOverState::render(sf::RenderWindow& window) {
    window.draw(m_titleText);
    window.draw(m_scoreText);

    if (!m_submitted) {
        m_nameInput.render(window);
        m_submitBtn.render(window);
    } else {
        window.draw(m_statusText);
    }

    m_returnBtn.render(window);
}

void GameOverState::onSubmitScore() {
    if (m_submitted) return;

    std::string name = m_nameInput.getText();
    if (name.empty()) {
        name = "Player";
    }

    Leaderboard::addEntry(name, m_score);
    m_submitted = true;

    m_statusText.setString(L"成绩已提交！排名前10的记录已保存");
    sf::FloatRect sb = m_statusText.getLocalBounds();
    m_statusText.setOrigin(sb.left + sb.width / 2.f, 0.f);
}

void GameOverState::onReturnToMenu() {
    Game::instance->changeState(std::make_unique<MainMenuState>());
}
