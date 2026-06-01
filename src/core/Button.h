#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

/**
 * 通用按钮类
 * 支持普通、悬停、点击三种视觉状态
 */
class Button {
public:
    enum class State {
        Normal,
        Hovered,
        Clicked
    };

    Button();
    Button(const std::string& text, const sf::Font& font,
           unsigned int charSize, sf::Vector2f position,
           sf::Vector2f size = {200.f, 50.f});

    void setText(const std::string& text);
    void setFont(const sf::Font& font);
    void setCharacterSize(unsigned int size);
    void setPosition(sf::Vector2f position);
    void setSize(sf::Vector2f size);
    void setColors(sf::Color normal, sf::Color hovered, sf::Color clicked);
    void setTextColor(sf::Color color);
    void setOutlineThickness(float thickness);
    void setCallback(std::function<void()> callback);

    // 更新按钮状态（每帧调用）
    void update(sf::Vector2f mousePos);

    // 处理鼠标点击事件
    void handleClick(sf::Vector2f mousePos);

    // 渲染按钮
    void render(sf::RenderWindow& window);

    bool contains(sf::Vector2f point) const;

    State getState() const { return m_state; }

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    State m_state;

    sf::Color m_colorNormal;
    sf::Color m_colorHovered;
    sf::Color m_colorClicked;

    std::function<void()> m_callback;
};
