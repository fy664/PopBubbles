#include "core/Button.h"
#include "core/Input.h"

Button::Button()
    : m_state(State::Normal)
    , m_colorNormal(sf::Color(70, 70, 70))
    , m_colorHovered(sf::Color(100, 100, 100))
    , m_colorClicked(sf::Color(50, 50, 150)) {

    m_shape.setSize({200.f, 50.f});
    m_shape.setFillColor(m_colorNormal);
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(sf::Color::White);
}

Button::Button(const std::wstring& text, const sf::Font& font,
               unsigned int charSize, sf::Vector2f position,
               sf::Vector2f size)
    : Button() {

    setText(text);
    setFont(font);
    setCharacterSize(charSize);
    setPosition(position);
    setSize(size);
}

void Button::setText(const std::wstring& text) {
    m_textStr = text;
    m_text.setString(m_textStr);
    // Center text
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setOrigin(textBounds.left + textBounds.width / 2.f,
                     textBounds.top + textBounds.height / 2.f);
}

void Button::setFont(const sf::Font& font) {
    m_text.setFont(font);
    // Re-center
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setOrigin(textBounds.left + textBounds.width / 2.f,
                     textBounds.top + textBounds.height / 2.f);
}

void Button::setCharacterSize(unsigned int size) {
    m_text.setCharacterSize(size);
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setOrigin(textBounds.left + textBounds.width / 2.f,
                     textBounds.top + textBounds.height / 2.f);
}

void Button::setPosition(sf::Vector2f position) {
    m_shape.setPosition(position);
    // Center text in button
    sf::Vector2f shapeSize = m_shape.getSize();
    m_text.setPosition(position.x + shapeSize.x / 2.f,
                       position.y + shapeSize.y / 2.f);
}

void Button::setSize(sf::Vector2f size) {
    m_shape.setSize(size);
    // Re-center text
    m_text.setPosition(m_shape.getPosition().x + size.x / 2.f,
                       m_shape.getPosition().y + size.y / 2.f);
}

void Button::setColors(sf::Color normal, sf::Color hovered, sf::Color clicked) {
    m_colorNormal = normal;
    m_colorHovered = hovered;
    m_colorClicked = clicked;
}

void Button::setTextColor(sf::Color color) {
    m_text.setFillColor(color);
}

void Button::setOutlineThickness(float thickness) {
    m_shape.setOutlineThickness(thickness);
}

void Button::setCallback(std::function<void()> callback) {
    m_callback = callback;
}

void Button::update(sf::Vector2f mousePos) {
    bool hovering = contains(mousePos);
    bool clicking = hovering && sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (clicking) {
        m_state = State::Clicked;
    } else if (hovering) {
        m_state = State::Hovered;
    } else {
        m_state = State::Normal;
    }
}

void Button::handleClick(sf::Vector2f mousePos) {
    if (contains(mousePos) && m_callback) {
        m_callback();
    }
}

void Button::render(sf::RenderWindow& window) {
    switch (m_state) {
        case State::Normal:
            m_shape.setFillColor(m_colorNormal);
            m_shape.setScale(1.f, 1.f);
            break;
        case State::Hovered:
            m_shape.setFillColor(m_colorHovered);
            m_shape.setScale(1.05f, 1.05f);
            break;
        case State::Clicked:
            m_shape.setFillColor(m_colorClicked);
            m_shape.setScale(0.95f, 0.95f);
            break;
    }
    window.draw(m_shape);
    window.draw(m_text);
}

bool Button::contains(sf::Vector2f point) const {
    return m_shape.getGlobalBounds().contains(point);
}
