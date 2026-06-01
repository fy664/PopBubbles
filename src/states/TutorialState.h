#pragma once
#include "core/State.h"
#include <SFML/Graphics.hpp>
#include <vector>

/**
 * 新手教程状态
 * 分4步：移动控制→攻击方式→三种敌人介绍→游戏目标
 * 每步显示文字提示，按空格键继续
 */
class TutorialState : public State {
public:
    TutorialState();
    ~TutorialState() override = default;

    void handleInput(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    void nextStep();

    int m_currentStep;
    sf::Text m_titleText;
    sf::Text m_contentText;
    sf::Text m_hintText;
    sf::Text m_stepIndicator;

    // 教程中的演示元素
    sf::CircleShape m_demoPlayer;
    sf::CircleShape m_demoBubble;
    sf::CircleShape m_demoShooter;
    sf::CircleShape m_demoGiant;
};
