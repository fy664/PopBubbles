#include "core/Input.h"

bool Input::m_prevKeys[sf::Keyboard::KeyCount] = {};
bool Input::m_currKeys[sf::Keyboard::KeyCount] = {};
bool Input::m_prevMouse[sf::Mouse::ButtonCount] = {};
bool Input::m_currMouse[sf::Mouse::ButtonCount] = {};

void Input::update() {
    // 保存上一帧状态
    for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
        m_prevKeys[i] = m_currKeys[i];
        m_currKeys[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
    }
    for (int i = 0; i < sf::Mouse::ButtonCount; ++i) {
        m_prevMouse[i] = m_currMouse[i];
        m_currMouse[i] = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i));
    }
}

bool Input::isKeyDown(sf::Keyboard::Key key) {
    return m_currKeys[key];
}

bool Input::isKeyPressed(sf::Keyboard::Key key) {
    return m_currKeys[key] && !m_prevKeys[key];
}

bool Input::isKeyReleased(sf::Keyboard::Key key) {
    return !m_currKeys[key] && m_prevKeys[key];
}

bool Input::isMouseDown(sf::Mouse::Button button) {
    return m_currMouse[button];
}

bool Input::isMousePressed(sf::Mouse::Button button) {
    return m_currMouse[button] && !m_prevMouse[button];
}

bool Input::isMouseReleased(sf::Mouse::Button button) {
    return !m_currMouse[button] && m_prevMouse[button];
}

sf::Vector2i Input::getMousePosition(const sf::RenderWindow& window) {
    return sf::Mouse::getPosition(window);
}

sf::Vector2f Input::getMouseWorld(const sf::RenderWindow& window) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    return window.mapPixelToCoords(pixelPos);
}
