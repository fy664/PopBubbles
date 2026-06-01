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
            m_titleText.setString(L"攻击方式 - 锥形尖针");
            m_contentText.setString(
                L"一根锥形尖针从球体中心伸出，始终指向鼠标方向\n\n"
                L"被动碰撞：尖针碰到泡泡即破，无需点击！\n"
                L"  - 静止时尖针长度约 55 像素\n"
                L"  - 靠近泡泡即可自动戳破\n\n"
                L"主动戳刺：点击鼠标左键\n"
                L"  - 尖针向前延伸 35 像素，持续 0.3 秒\n"
                L"  - 获得更远射程，攻击更远处的泡泡\n"
                L"  - 针体整体伸缩，收放流畅\n\n"
                L"提示：移动鼠标瞄准，靠近即可戳破！"
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
                L"普通泡泡（随机彩色）：\n"
                L"  持续向玩家移动，+10分\n"
                L"  泡泡之间会互相碰撞弹开\n\n"
                L"射手泡泡（紫色）：\n"
                L"  与玩家保持距离，每2.5秒发射子弹\n"
                L"  子弹也会对玩家造成伤害，+30分\n\n"
                L"巨型泡泡（橙色，2倍大）：\n"
                L"  需要攻击两次才能击败，+50分\n"
                L"  近身会冲刺追击\n"
                L"  戳刺时双方都会弹开（回弹效果）"
            );
            m_hintText.setString(L"按 空格键 继续");
            {
                auto bounds = m_titleText.getLocalBounds();
                m_titleText.setOrigin(bounds.left + bounds.width / 2.f, 0.f);
            }
            break;

        case 3: // 游戏目标与机制
            m_titleText.setString(L"游戏目标与机制");
            m_contentText.setString(
                L"尽可能多地戳破泡泡，获得高分！\n\n"
                L"等级系统：\n"
                L"  - 每击杀 10 个泡泡升一级\n"
                L"  - 升级提升血量上限 +2（基础5HP）\n"
                L"  - 升级回复 2 点生命值\n"
                L"  - 连续击杀触发 combo 加分\n\n"
                L"游戏规则：\n"
                L"  - 被泡泡/子弹碰到损失 1 点生命\n"
                L"  - 受伤后 1.5 秒无敌（角色闪烁）\n"
                L"  - 生命归零则游戏结束\n"
                L"  - 每 7.5 秒难度升级，敌人更多更快\n"
                L"  - 按 ESC 可暂停/返回菜单\n\n"
                L"死亡后提交成绩到排行榜！Top 10 永久保存"
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
