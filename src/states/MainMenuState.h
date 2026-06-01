#pragma once
#include "core/State.h"
#include "core/Button.h"
#include <vector>
#include <SFML/Graphics.hpp>

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

    // 装饰性背景泡泡
    struct DecoBubble {
        sf::CircleShape shape;
        sf::Vector2f drift;
    };
    std::vector<DecoBubble> m_decoBubbles;
    void initDecoBubbles();

    void onStartGame();
    void onLeaderboard();
    void onTutorial();
    void onQuit();
};
