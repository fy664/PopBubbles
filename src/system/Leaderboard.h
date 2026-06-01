#pragma once
#include <string>
#include <vector>

/**
 * 排行榜数据管理
 * 本地 JSON 文件存储（leaderboard.json）
 * 自动按得分降序排序，只保留前10条
 */
class Leaderboard {
public:
    struct Entry {
        std::string name;
        int score;
        std::string date;
    };

    static constexpr const char* FILENAME = "leaderboard.json";
    static constexpr size_t MAX_ENTRIES = 10;

    // 加载排行榜
    static std::vector<Entry> load();

    // 保存排行榜
    static void save(const std::vector<Entry>& entries);

    // 添加新记录（自动排序，保留前10）
    static void addEntry(const std::string& name, int score);

    // 获取排序后的排行榜（从文件读取）
    static std::vector<Entry> getTopEntries();

private:
    // 按得分降序排序
    static void sortEntries(std::vector<Entry>& entries);

    static std::string getFilePath();
};
