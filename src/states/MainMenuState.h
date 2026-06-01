#pragma once
#include "core/State.h"
#include "core/Button.h"
#include <vector>

/**
 * 主菜单状态
 * 包含"开始游戏"、"排行榜"、"退出游戏"三个按钮
 */
class MainMenuState : public State {
public:
    MainMenuState();
    ~MainMenuState() override = default;

    void handleInput(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;

private:
    sf::Text m_titleText;
    std::vector<Button> m_buttons;

    void onStartGame();
    void onLeaderboard();
    void onTutorial();
    void onQuit();
};
