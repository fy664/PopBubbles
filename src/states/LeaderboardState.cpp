#include "states/LeaderboardState.h"
#include "system/Leaderboard.h"
#include "core/Game.h"
#include "core/ResourceManager.h"
#include "core/Input.h"
#include <SFML/System/String.hpp>
#include <iomanip>
#include <sstream>

LeaderboardState::LeaderboardState() {
    auto& font = ResourceManager::getInstance().getFont("default");

    m_titleText.setFont(font);
    m_titleText.setString(L"排行榜 - Top 10");
    m_titleText.setCharacterSize(36);
    m_titleText.setFillColor(sf::Color(255, 220, 100));
    sf::FloatRect tb = m_titleText.getLocalBounds();
    m_titleText.setOrigin(tb.left + tb.width / 2.f, 0.f);
    m_titleText.setPosition(Game::WINDOW_WIDTH / 2.f, 30.f);

    m_emptyText.setFont(font);
    m_emptyText.setString(L"暂无记录，快去玩游戏吧！");
    m_emptyText.setCharacterSize(24);
    m_emptyText.setFillColor(sf::Color(150, 150, 150));
    sf::FloatRect eb = m_emptyText.getLocalBounds();
    m_emptyText.setOrigin(eb.left + eb.width / 2.f, 0.f);
    m_emptyText.setPosition(Game::WINDOW_WIDTH / 2.f, 200.f);

    m_backBtn = Button(L"返回", font, 24,
                       sf::Vector2f(Game::WINDOW_WIDTH / 2.f, Game::WINDOW_HEIGHT - 80.f),
                       sf::Vector2f(200.f, 50.f));
    m_backBtn.setColors(sf::Color(80, 80, 80), sf::Color(110, 110, 110), sf::Color(50, 50, 50));
    m_backBtn.setTextColor(sf::Color::White);
    m_backBtn.setCallback([this]() { onBack(); });

    refreshEntries();
}

void LeaderboardState::handleInput(sf::RenderWindow& window) {
    sf::Vector2f mousePos = Input::getMouseWorld(window);

    if (Input::isMousePressed(sf::Mouse::Left)) {
        m_backBtn.handleClick(mousePos);
    }

    if (Input::isKeyPressed(sf::Keyboard::Escape)) {
        onBack();
    }
}

void LeaderboardState::update(float deltaTime) {
    auto& window = Game::instance->getWindow();
    sf::Vector2f mousePos = Input::getMouseWorld(window);
    m_backBtn.update(mousePos);
}

void LeaderboardState::render(sf::RenderWindow& window) {
    window.draw(m_titleText);

    if (m_entryTexts.empty()) {
        window.draw(m_emptyText);
    } else {
        for (auto& text : m_entryTexts) {
            window.draw(text);
        }
    }

    m_backBtn.render(window);
}

void LeaderboardState::refreshEntries() {
    m_entryTexts.clear();
    auto entries = Leaderboard::getTopEntries();

    auto& font = ResourceManager::getInstance().getFont("default");
    float startY = 100.f;
    float rowHeight = 55.f;

    // 表头
    sf::Text header;
    header.setFont(font);
    header.setCharacterSize(22);
    header.setFillColor(sf::Color(200, 200, 200));

    std::wstring headerStr = L"排名    姓名              得分        日期";
    header.setString(headerStr);
    header.setPosition(Game::WINDOW_WIDTH / 2.f - 250.f, startY);
    m_entryTexts.push_back(header);

    // 分隔线
    sf::Text separator;
    separator.setFont(font);
    separator.setCharacterSize(18);
    separator.setFillColor(sf::Color(80, 80, 80));
    separator.setString(L"──────────────────────────────────────");
    separator.setPosition(Game::WINDOW_WIDTH / 2.f - 250.f, startY + 28.f);
    m_entryTexts.push_back(separator);

    for (size_t i = 0; i < entries.size(); ++i) {
        sf::Text entryText;
        entryText.setFont(font);
        entryText.setCharacterSize(22);

        // 名次颜色
        if (i == 0) entryText.setFillColor(sf::Color(255, 215, 0));       // 金色
        else if (i == 1) entryText.setFillColor(sf::Color(192, 192, 192)); // 银色
        else if (i == 2) entryText.setFillColor(sf::Color(205, 127, 50));  // 铜色
        else entryText.setFillColor(sf::Color::White);

        // Convert UTF-8 name/date from leaderboard to wide strings
        std::wstring wname = sf::String::fromUtf8(entries[i].name.begin(), entries[i].name.end());
        std::wstring wdate = sf::String::fromUtf8(entries[i].date.begin(), entries[i].date.end());

        // Fixed-width formatting with wide strings
        std::wostringstream wss;
        wss << L"#" << (i + 1);

        std::wstring rankPart = wss.str();
        std::wstring namePart = wname;
        if (namePart.size() > 12) {
            namePart = namePart.substr(0, 11) + L"...";
        }

        wss.str(L"");
        wss.clear();
        wss << rankPart << L"     " << namePart;
        size_t currentLen = rankPart.size() + 5 + namePart.size();
        while (currentLen < 30) {
            wss << L" ";
            currentLen++;
        }
        wss << entries[i].score;
        std::wstring scoreStr = std::to_wstring(entries[i].score);
        currentLen += scoreStr.size();
        while (currentLen < 42) {
            wss << L" ";
            currentLen++;
        }
        wss << wdate;

        entryText.setString(wss.str());
        entryText.setPosition(Game::WINDOW_WIDTH / 2.f - 250.f,
                              startY + 40.f + i * rowHeight);
        m_entryTexts.push_back(entryText);
    }
}

void LeaderboardState::onBack() {
    Game::instance->popState();
}
