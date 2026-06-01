#include "core/Game.h"
#include "core/ResourceManager.h"
#include "core/Input.h"
#include <iostream>

Game* Game::instance = nullptr;

Game::Game()
    : m_window()
    , m_running(false) {
    instance = this;
}

Game::~Game() {
    instance = nullptr;
}

bool Game::init() {
    m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                    L"Pop Bubbles - 戳泡泡", sf::Style::Titlebar | sf::Style::Close);
    m_window.setFramerateLimit(60);
    m_window.setVerticalSyncEnabled(false);

    auto& rm = ResourceManager::getInstance();

    std::cout << "[Game] Initialized: " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
    return true;
}

void Game::run() {
    m_running = true;

    while (m_window.isOpen() && m_running) {
        float deltaTime = m_clock.restart().asSeconds();
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        Input::update();
        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_window.close();
                m_running = false;
                break;
            case sf::Event::Resized:
                m_window.setView(sf::View(sf::FloatRect(0, 0,
                    static_cast<float>(event.size.width),
                    static_cast<float>(event.size.height))));
                break;
            default:
                if (!m_stateStack.empty()) {
                    m_stateStack.back()->handleEvent(event);
                }
                break;
        }
    }
}

void Game::update(float deltaTime) {
    if (!m_stateStack.empty()) {
        m_stateStack.back()->update(deltaTime);
    }
}

void Game::render() {
    m_window.clear(sf::Color(20, 20, 40));

    if (!m_stateStack.empty()) {
        // Only render the topmost opaque state, plus any transparent ones below
        auto& top = m_stateStack.back();
        if (!top->pausesBelow() || top->isTransparent()) {
            // Render from bottom to top
            for (auto& state : m_stateStack) {
                state->render(m_window);
                if (state->pausesBelow() && !state->isTransparent()) {
                    break;
                }
            }
        } else {
            top->render(m_window);
        }
    }

    m_window.display();
}

void Game::pushState(std::unique_ptr<State> state) {
    if (!m_stateStack.empty()) {
        m_stateStack.back()->onExit();
    }
    m_stateStack.push_back(std::move(state));
    m_stateStack.back()->onEnter();
    std::cout << "[Game] Pushed state, stack size: " << m_stateStack.size() << std::endl;
}

void Game::popState() {
    if (!m_stateStack.empty()) {
        m_stateStack.back()->onExit();
        m_stateStack.pop_back();
    }
    if (!m_stateStack.empty()) {
        m_stateStack.back()->onEnter();
    }
    std::cout << "[Game] Popped state, stack size: " << m_stateStack.size() << std::endl;
}

void Game::changeState(std::unique_ptr<State> state) {
    if (!m_stateStack.empty()) {
        m_stateStack.back()->onExit();
        m_stateStack.pop_back();
    }
    m_stateStack.push_back(std::move(state));
    m_stateStack.back()->onEnter();
    std::cout << "[Game] Changed state, stack size: " << m_stateStack.size() << std::endl;
}

State* Game::peekState() {
    if (m_stateStack.empty()) return nullptr;
    return m_stateStack.back().get();
}
