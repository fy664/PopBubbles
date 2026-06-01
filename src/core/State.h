#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

/**
 * 抽象状态基类
 * 所有游戏状态（菜单、游戏、排行榜等）都继承此类
 */
class State {
public:
    State() = default;
    virtual ~State() = default;

    // 处理 SFML 事件（在 Game::handleEvents 中调用）
    virtual void handleEvent(const sf::Event& event) {}

    // 处理输入
    virtual void handleInput(sf::RenderWindow& window) = 0;

    // 更新逻辑
    virtual void update(float deltaTime) = 0;

    // 渲染
    virtual void render(sf::RenderWindow& window) = 0;

    // 状态进入/退出回调
    virtual void onEnter() {}
    virtual void onExit() {}

    // 是否需要透明度覆盖下层状态
    virtual bool isTransparent() const { return false; }

    // 是否暂停下层状态的更新
    virtual bool pausesBelow() const { return true; }
};
