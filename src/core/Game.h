#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "State.h"

/**
 * 游戏主类
 * 管理窗口、状态栈和全局游戏循环
 */
class Game {
public:
    static constexpr unsigned int WINDOW_WIDTH = 1024;
    static constexpr unsigned int WINDOW_HEIGHT = 768;

    Game();
    ~Game();

    // 初始化游戏
    bool init();

    // 运行主循环
    void run();

    // 状态栈管理
    void pushState(std::unique_ptr<State> state);
    void popState();
    void changeState(std::unique_ptr<State> state); // 替换当前状态
    State* peekState();

    // 获取窗口引用
    sf::RenderWindow& getWindow() { return m_window; }

    // 退出游戏
    void quit() { m_running = false; }

    // 静态实例访问（供状态使用）
    static Game* instance;

private:
    void handleEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow m_window;
    std::vector<std::unique_ptr<State>> m_stateStack;
    sf::Clock m_clock;
    bool m_running;
};
