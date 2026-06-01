#include "core/TextBox.h"
#include "core/Input.h"

TextBox::TextBox()
    : m_maxChars(10)
    , m_focused(false)
    , m_showCursor(true)
    , m_borderColor(sf::Color(150, 150, 150))
    , m_focusColor(sf::Color(100, 100, 255)) {

    m_shape.setSize({300.f, 40.f});
    m_shape.setFillColor(sf::Color(40, 40, 40));
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(m_borderColor);
    m_text.setFillColor(sf::Color::White);
    m_placeholder.setFillColor(sf::Color(120, 120, 120));
}

TextBox::TextBox(const sf::Font& font, unsigned int charSize,
                 sf::Vector2f position, sf::Vector2f size,
                 size_t maxChars)
    : TextBox() {

    setFont(font);
    m_text.setCharacterSize(charSize);
    m_placeholder.setCharacterSize(charSize);
    setPosition(position);
    setSize(size);
    m_maxChars = maxChars;
}

void TextBox::setFont(const sf::Font& font) {
    m_text.setFont(font);
    m_placeholder.setFont(font);
}

void TextBox::setPosition(sf::Vector2f position) {
    m_shape.setPosition(position);
    m_text.setPosition(position.x + 8.f, position.y + 5.f);
    m_placeholder.setPosition(position.x + 8.f, position.y + 5.f);
}

void TextBox::setSize(sf::Vector2f size) {
    m_shape.setSize(size);
}

void TextBox::setMaxChars(size_t max) {
    m_maxChars = max;
}

void TextBox::setPlaceholder(const std::string& text) {
    m_placeholderStr = text;
    m_placeholder.setString(m_placeholderStr);
}

void TextBox::handleTextEntered(uint32_t unicode) {
    if (!m_focused) return;

    // 退格键在 handleKeyInput 中处理
    if (unicode == 8) return;
    // 回车
    if (unicode == 13) {
        if (m_callback) m_callback(m_content);
        return;
    }
    // 过滤控制字符
    if (unicode < 32 && unicode != 0) return;

    // UTF-8 编码处理：对于 ASCII 范围内的字符直接处理
    if (m_content.size() < m_maxChars) {
        if (unicode < 128) {
            m_content += static_cast<char>(unicode);
        }
        // 注意：这里简单处理，完整的中文输入需要 TextEntered 事件
        // SFML 的 TextEntered 事件已经提供了完整的 UTF-32 码点
        // 我们将其转换为 UTF-8
        else if (unicode < 0x800) {
            m_content += static_cast<char>(0xC0 | (unicode >> 6));
            m_content += static_cast<char>(0x80 | (unicode & 0x3F));
        } else if (unicode < 0x10000) {
            m_content += static_cast<char>(0xE0 | (unicode >> 12));
            m_content += static_cast<char>(0x80 | ((unicode >> 6) & 0x3F));
            m_content += static_cast<char>(0x80 | (unicode & 0x3F));
        } else if (unicode < 0x110000) {
            m_content += static_cast<char>(0xF0 | (unicode >> 18));
            m_content += static_cast<char>(0x80 | ((unicode >> 12) & 0x3F));
            m_content += static_cast<char>(0x80 | ((unicode >> 6) & 0x3F));
            m_content += static_cast<char>(0x80 | (unicode & 0x3F));
        }
    }
    m_text.setString(m_content);
}

void TextBox::handleKeyInput() {
    if (!m_focused) return;

    if (Input::isKeyPressed(sf::Keyboard::Backspace) || Input::isKeyPressed(sf::Keyboard::Delete)) {
        if (!m_content.empty()) {
            // 处理 UTF-8 多字节删除
            // 从末尾删除一个完整的 UTF-8 字符
            size_t len = 1;
            if (!m_content.empty()) {
                unsigned char last = static_cast<unsigned char>(m_content.back());
                // 计算需要删除的字节数（追溯到该字符的起始字节）
                if ((last & 0x80) == 0) len = 1;          // ASCII
                else {
                    // 向前找起始字节
                    for (size_t i = m_content.size() - 1; i > 0; --i) {
                        unsigned char ch = static_cast<unsigned char>(m_content[i]);
                        if ((ch & 0xC0) != 0x80) {
                            len = m_content.size() - i;
                            break;
                        }
                    }
                }
            }
            m_content.erase(m_content.size() - len, len);
            m_text.setString(m_content);
        }
    }

    if (Input::isKeyPressed(sf::Keyboard::Return) || Input::isKeyPressed(sf::Keyboard::Enter)) {
        if (m_callback) m_callback(m_content);
    }
}

void TextBox::update(sf::Vector2f mousePos, bool clicked) {
    if (clicked) {
        m_focused = contains(mousePos);
    }

    if (m_focused) {
        m_shape.setOutlineColor(m_focusColor);
    } else {
        m_shape.setOutlineColor(m_borderColor);
    }

    // 光标闪烁
    if (m_cursorClock.getElapsedTime().asSeconds() > 0.5f) {
        m_showCursor = !m_showCursor;
        m_cursorClock.restart();
    }
}

void TextBox::render(sf::RenderWindow& window) {
    window.draw(m_shape);

    if (m_content.empty() && !m_focused) {
        window.draw(m_placeholder);
    } else {
        window.draw(m_text);

        // 绘制光标
        if (m_focused && m_showCursor) {
            sf::RectangleShape cursor;
            sf::FloatRect bounds = m_text.getGlobalBounds();
            float cursorX = bounds.left + bounds.width + 2.f;
            float cursorY = bounds.top + 2.f;
            float cursorH = bounds.height - 4.f;
            cursor.setPosition(cursorX, cursorY);
            cursor.setSize({2.f, cursorH});
            cursor.setFillColor(sf::Color::White);
            window.draw(cursor);
        }
    }
}

std::string TextBox::getText() const {
    return m_content;
}

void TextBox::setText(const std::string& text) {
    m_content = text;
    m_text.setString(m_content);
}

void TextBox::clear() {
    m_content.clear();
    m_text.setString(m_content);
}

bool TextBox::contains(sf::Vector2f point) const {
    return m_shape.getGlobalBounds().contains(point);
}
