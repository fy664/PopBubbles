#pragma once
#include "core/State.h"
#include "core/Button.h"
#include <SFML/Graphics.hpp>
#include <vector>

/**
 * 排行榜状态
 * 显示排名、姓名和得分（前10条）
 */
class LeaderboardState : public State {
public:
    LeaderboardState();
    ~LeaderboardState() override = default;

    void handleInput(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    void refreshEntries();
    void onBack();

    sf::Text m_titleText;
    std::vector<sf::Text> m_entryTexts;
    sf::Text m_emptyText;
    Button m_backBtn;
};
