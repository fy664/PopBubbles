#include "core/TextBox.h"
#include "core/Input.h"
#include <SFML/System/Utf.hpp>
#include <algorithm>

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

void TextBox::setPlaceholder(const std::wstring& text) {
    m_placeholderStr = text;
    m_placeholder.setString(m_placeholderStr);
}

void TextBox::handleTextEntered(uint32_t unicode) {
    if (!m_focused) return;

    if (unicode == 8) return;          // Backspace handled in handleKeyInput
    if (unicode == 13) {               // Enter
        if (m_callback) m_callback(getText());
        return;
    }
    if (unicode < 32 && unicode != 0) return;

    if (m_content.size() < m_maxChars) {
        // Convert UTF-32 codepoint to wchar_t (on Windows wchar_t is UTF-16)
        if (unicode < 0x10000) {
            m_content += static_cast<wchar_t>(unicode);
        } else {
            // Surrogate pair for codepoints > U+FFFF
            unicode -= 0x10000;
            m_content += static_cast<wchar_t>(0xD800 | (unicode >> 10));
            m_content += static_cast<wchar_t>(0xDC00 | (unicode & 0x3FF));
        }
        m_text.setString(m_content);
    }
}

void TextBox::handleKeyInput() {
    if (!m_focused) return;

    if (Input::isKeyPressed(sf::Keyboard::Backspace) || Input::isKeyPressed(sf::Keyboard::Delete)) {
        if (!m_content.empty()) {
            m_content.pop_back();
            m_text.setString(m_content);
        }
    }

    if (Input::isKeyPressed(sf::Keyboard::Return) || Input::isKeyPressed(sf::Keyboard::Enter)) {
        if (m_callback) m_callback(getText());
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

    // Cursor blink
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

        // Draw cursor
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
    // Convert wstring to UTF-8 using SFML
    std::string result;
    sf::Utf8::fromWide(m_content.begin(), m_content.end(), std::back_inserter(result));
    return result;
}

std::wstring TextBox::getWText() const {
    return m_content;
}

void TextBox::setText(const std::wstring& text) {
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
