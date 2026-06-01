#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

/**
 * 文本输入框
 * 支持键盘输入、退格删除、焦点管理
 */
class TextBox {
public:
    TextBox();
    TextBox(const sf::Font& font, unsigned int charSize,
            sf::Vector2f position, sf::Vector2f size,
            size_t maxChars = 10);

    void setFont(const sf::Font& font);
    void setPosition(sf::Vector2f position);
    void setSize(sf::Vector2f size);
    void setMaxChars(size_t max);
    void setPlaceholder(const std::string& text);

    // 处理文字输入事件
    void handleTextEntered(uint32_t unicode);

    // 处理键盘（退格等）
    void handleKeyInput();

    // 更新（焦点、闪烁光标等）
    void update(sf::Vector2f mousePos, bool clicked);

    // 渲染
    void render(sf::RenderWindow& window);

    std::string getText() const;
    void setText(const std::string& text);
    void clear();

    bool contains(sf::Vector2f point) const;
    bool isFocused() const { return m_focused; }

    void setCallback(std::function<void(const std::string&)> callback) { m_callback = callback; }

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    sf::Text m_placeholder;
    std::string m_content;
    std::string m_placeholderStr;
    size_t m_maxChars;
    bool m_focused;
    sf::Clock m_cursorClock;
    bool m_showCursor;

    sf::Color m_borderColor;
    sf::Color m_focusColor;

    std::function<void(const std::string&)> m_callback;
};
