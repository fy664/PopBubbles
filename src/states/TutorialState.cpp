#include "states/TutorialState.h"
#include "states/GameState.h"
#include "core/Game.h"
#include "core/ResourceManager.h"
#include "core/Input.h"

TutorialState::TutorialState()
    : m_currentStep(0) {

    auto& font = ResourceManager::getInstance().getFont("default");

    m_titleText.setFont(font);
    m_titleText.setCharacterSize(36);
    m_titleText.setFillColor(sf::Color(255, 220, 100));
    m_titleText.setPosition(Game::WINDOW_WIDTH / 2.f, 50.f);

    m_contentText.setFont(font);
    m_contentText.setCharacterSize(22);
    m_contentText.setFillColor(sf::Color::White);
    m_contentText.setPosition(50.f, 120.f);

    m_hintText.setFont(font);
    m_hintText.setCharacterSize(20);
    m_hintText.setFillColor(sf::Color(180, 180, 180));
    m_hintText.setPosition(Game::WINDOW_WIDTH / 2.f, Game::WINDOW_HEIGHT - 80.f);

    m_stepIndicator.setFont(font);
    m_stepIndicator.setCharacterSize(18);
    m_stepIndicator.setFillColor(sf::Color(150, 150, 255));
    m_stepIndicator.setPosition(Game::WINDOW_WIDTH - 150.f, 10.f);

    // 演示元素
    m_demoPlayer.setRadius(25.f);
    m_demoPlayer.setOrigin(25.f, 25.f);
    m_demoPlayer.setFillColor(sf::Color(80, 180, 255));
    m_demoPlayer.setPosition(Game::WINDOW_WIDTH / 2.f, Game::WINDOW_HEIGHT - 150.f);

    m_demoBubble.setRadius(20.f);
    m_demoBubble.setOrigin(20.f, 20.f);
    m_demoBubble.setFillColor(sf::Color(255, 100, 100));
    m_demoBubble.setPosition(Game::WINDOW_WIDTH / 2.f, 250.f);

    m_demoShooter.setRadius(20.f);
    m_demoShooter.setOrigin(20.f, 20.f);
    m_demoShooter.setFillColor(sf::Color(160, 50, 200));
    m_demoShooter.setPosition(Game::WINDOW_WIDTH / 2.f - 80.f, 280.f);

    m_demoGiant.setRadius(40.f);
    m_demoGiant.setOrigin(40.f, 40.f);
    m_demoGiant.setFillColor(sf::Color(255, 140, 30));
    m_demoGiant.setPosition(Game::WINDOW_WIDTH / 2.f + 80.f, 280.f);

    nextStep();
}

void TutorialState::handleInput(sf::RenderWindow& window) {
    if (Input::isKeyPressed(sf::Keyboard::Space)) {
        if (m_currentStep < 4) {
            m_currentStep++;
            nextStep();
        } else {
            // 最后一步：直接进入游戏
            Game::instance->changeState(std::make_unique<GameState>());
        }
    }

    if (Input::isKeyPressed(sf::Keyboard::Escape)) {
        Game::instance->popState();
    }
}

void TutorialState::update(float deltaTime) {
    // 教程不需要实时更新
}

void TutorialState::render(sf::RenderWindow& window) {
    window.draw(m_titleText);
    window.draw(m_contentText);
    window.draw(m_hintText);
    window.draw(m_stepIndicator);

    // 根据步骤绘制演示元素
    if (m_currentStep == 0) {
        window.draw(m_demoPlayer);
    } else if (m_currentStep == 1) {
        window.draw(m_demoPlayer);
        window.draw(m_demoBubble);
    } else if (m_currentStep == 2) {
        window.draw(m_demoShooter);
        window.draw(m_demoGiant);
    } else if (m_currentStep == 3) {
        window.draw(m_demoPlayer);
        window.draw(m_demoBubble);
        window.draw(m_demoShooter);
        window.draw(m_demoGiant);
    }
}

void TutorialState::nextStep() {
    m_stepIndicator.setString(L"步骤 " + std::to_wstring(m_currentStep + 1) + L" / 4");

    switch (m_currentStep) {
        case 0: // 移动控制
            m_titleText.setString(L"移动控制");
            m_contentText.setString(
                L"使用 W/A/S/D 键控制角色在屏幕中移动\n\n"
                L"W - 向上移动\n"
                L"A - 向左移动\n"
                L"S - 向下移动\n"
                L"D - 向右移动\n\n"
                L"角色不能移出屏幕边界"
            );
            m_hintText.setString(L"按 空格键 继续");
            // 居中标题
            {
                auto bounds = m_titleText.getLocalBounds();
                m_titleText.setOrigin(bounds.left + bounds.width / 2.f, 0.f);
            }
            break;

        case 1: // 攻击方式
            m_titleText.setString(L"攻击方式 - 尖针戳刺");
            m_contentText.setString(
                L"玩家手持一根尖针，尖针始终指向鼠标方向\n\n"
                L"点击 鼠标左键 时，尖针向前快速伸出\n"
                L"持续 0.3 秒后收回\n\n"
                L"尖针碰到泡泡即可将其戳破！\n"
                L"注意：这不是发射子弹，而是近战攻击\n\n"
                L"提示：移动鼠标瞄准，左键点击戳刺"
            );
            m_hintText.setString(L"按 空格键 继续");
            {
                auto bounds = m_titleText.getLocalBounds();
                m_titleText.setOrigin(bounds.left + bounds.width / 2.f, 0.f);
            }
            break;

        case 2: // 三种敌人介绍
            m_titleText.setString(L"敌人类型");
            m_contentText.setString(
                L"游戏中有三种泡泡敌人：\n\n"
                L"普通泡泡（彩色）：\n"
                L"  缓慢向玩家移动，碰到玩家造成伤害\n"
                L"  被尖针刺中即消失，+10分\n\n"
                L"射手泡泡（紫色）：\n"
                L"  与玩家保持200-300像素距离\n"
                L"  每3秒发射一颗子弹\n"
                L"  被消灭+30分\n\n"
                L"巨型泡泡（橙色，2倍大小）：\n"
                L"  血量x2，需要攻击两次才能击败\n"
                L"  接近时会冲刺，有冷却时间\n"
                L"  被消灭+50分"
            );
            m_hintText.setString(L"按 空格键 继续");
            {
                auto bounds = m_titleText.getLocalBounds();
                m_titleText.setOrigin(bounds.left + bounds.width / 2.f, 0.f);
            }
            break;

        case 3: // 游戏目标
            m_titleText.setString(L"游戏目标");
            m_contentText.setString(
                L"尽可能多地戳破泡泡，获得高分！\n\n"
                L"游戏规则：\n"
                L"  - 初始 5 点生命值\n"
                L"  - 被泡泡碰到损失 1 点生命\n"
                L"  - 受伤后有 1.5 秒无敌时间（角色闪烁）\n"
                L"  - 生命归零则游戏结束\n"
                L"  - 随着时间推移，难度会不断增加\n"
                L"  - 更多种类的泡泡会出现\n\n"
                L"死亡后可以提交你的成绩到排行榜！\n"
                L"排行榜只保留前 10 名"
            );
            m_hintText.setString(L"按 空格键 开始游戏！");
            {
                auto bounds = m_titleText.getLocalBounds();
                m_titleText.setOrigin(bounds.left + bounds.width / 2.f, 0.f);
            }
            break;

        case 4: // 进入游戏
            Game::instance->changeState(std::make_unique<GameState>());
            break;
    }
}
