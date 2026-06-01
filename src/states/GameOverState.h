#pragma once
#include "core/State.h"
#include "core/TextBox.h"
#include "core/Button.h"
#include <SFML/Graphics.hpp>

/**
 * 游戏结束状态
 * 显示最终得分、姓名输入框、提交按钮
 */
class GameOverState : public State {
public:
    GameOverState();
    ~GameOverState() override = default;

    void handleEvent(const sf::Event& event) override;
    void handleInput(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void setScore(int score) { m_score = score; }

private:
    void onSubmitScore();
    void onReturnToMenu();

    int m_score;
    sf::Text m_titleText;
    sf::Text m_scoreText;
    sf::Text m_statusText;

    TextBox m_nameInput;
    Button m_submitBtn;
    Button m_returnBtn;

    bool m_submitted;
    bool m_hasShownStatus;
};
