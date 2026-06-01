#include "system/Leaderboard.h"
#include "nlohmann-json.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

std::string Leaderboard::getFilePath() {
    // 排行榜文件存储在可执行文件所在目录
    return FILENAME;
}

std::vector<Leaderboard::Entry> Leaderboard::load() {
    std::vector<Entry> entries;

    std::ifstream file(getFilePath());
    if (!file.is_open()) {
        // 文件不存在则返回空列表
        return entries;
    }

    try {
        json data = json::parse(file);
        for (const auto& item : data) {
            Entry entry;
            entry.name = item.value("name", "???");
            entry.score = item.value("score", 0);
            entry.date = item.value("date", "");
            entries.push_back(entry);
        }
    } catch (const std::exception& e) {
        std::cerr << "[Leaderboard] Failed to parse leaderboard.json: " << e.what() << std::endl;
    }

    file.close();
    return entries;
}

void Leaderboard::save(const std::vector<Entry>& entries) {
    json data = json::array();
    for (const auto& entry : entries) {
        json item;
        item["name"] = entry.name;
        item["score"] = entry.score;
        item["date"] = entry.date;
        data.push_back(item);
    }

    std::ofstream file(getFilePath());
    if (file.is_open()) {
        file << data.dump(2);
        file.close();
        std::cout << "[Leaderboard] Saved " << entries.size() << " entries" << std::endl;
    } else {
        std::cerr << "[Leaderboard] Failed to save leaderboard.json" << std::endl;
    }
}

void Leaderboard::addEntry(const std::string& name, int score) {
    // 获取当前日期字符串
    auto now = std::time(nullptr);
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &now);
#else
    localtime_r(&now, &tm);
#endif
    std::ostringstream dateStream;
    dateStream << std::setfill('0')
               << (tm.tm_year + 1900) << "-"
               << std::setw(2) << (tm.tm_mon + 1) << "-"
               << std::setw(2) << tm.tm_mday;

    auto entries = load();

    Entry newEntry;
    newEntry.name = name;
    newEntry.score = score;
    newEntry.date = dateStream.str();
    entries.push_back(newEntry);

    sortEntries(entries);

    // 只保留前10条
    if (entries.size() > MAX_ENTRIES) {
        entries.resize(MAX_ENTRIES);
    }

    save(entries);
}

std::vector<Leaderboard::Entry> Leaderboard::getTopEntries() {
    auto entries = load();
    sortEntries(entries);
    if (entries.size() > MAX_ENTRIES) {
        entries.resize(MAX_ENTRIES);
    }
    return entries;
}

void Leaderboard::sortEntries(std::vector<Entry>& entries) {
    std::sort(entries.begin(), entries.end(),
        [](const Entry& a, const Entry& b) {
            return a.score > b.score; // 降序
        });
}
