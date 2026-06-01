#pragma once
#include <SFML/Graphics.hpp>

/**
 * 输入管理器
 * 统一处理键盘和鼠标输入，支持按下/释放/持续状态查询
 */
class Input {
public:
    // 每帧开始时调用，更新状态
    static void update();

    // 键盘：当前帧是否按下
    static bool isKeyDown(sf::Keyboard::Key key);

    // 键盘：当前帧是否刚按下（仅一帧为 true）
    static bool isKeyPressed(sf::Keyboard::Key key);

    // 键盘：当前帧是否刚释放
    static bool isKeyReleased(sf::Keyboard::Key key);

    // 鼠标：当前帧是否按下
    static bool isMouseDown(sf::Mouse::Button button);

    // 鼠标：当前帧是否刚按下
    static bool isMousePressed(sf::Mouse::Button button);

    // 鼠标：当前帧是否刚释放
    static bool isMouseReleased(sf::Mouse::Button button);

    // 获取鼠标位置（相对于窗口）
    static sf::Vector2i getMousePosition(const sf::RenderWindow& window);

    // 获取鼠标在世界坐标中的位置
    static sf::Vector2f getMouseWorld(const sf::RenderWindow& window);

private:
    Input() = default;

    static bool m_prevKeys[sf::Keyboard::KeyCount];
    static bool m_currKeys[sf::Keyboard::KeyCount];
    static bool m_prevMouse[sf::Mouse::ButtonCount];
    static bool m_currMouse[sf::Mouse::ButtonCount];
};
