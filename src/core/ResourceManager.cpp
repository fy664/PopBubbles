#include "core/ResourceManager.h"
#include <iostream>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

ResourceManager::ResourceManager() {
    // 注册默认字体（SFML 不含内置字体，需要从文件加载）
    // 用户可将字体文件放入 assets/ 目录后修改路径
    // 如果不存在，会尝试加载 Arial 或其他系统字体
}

bool ResourceManager::loadFont(const std::string& key, const std::string& path) {
    sf::Font font;
    if (!font.loadFromFile(path)) {
        std::cerr << "[ResourceManager] Failed to load font: " << path << std::endl;
        return false;
    }
    m_fonts[key] = std::move(font);
    std::cout << "[ResourceManager] Loaded font: " << key << " from " << path << std::endl;
    return true;
}

sf::Font& ResourceManager::getFont(const std::string& key) {
    auto it = m_fonts.find(key);
    if (it != m_fonts.end()) {
        return it->second;
    }
    // 如果请求的字体不存在，尝试返回默认字体
    // 如果默认字体也不存在，尝试使用 SFML 空字体
    auto defIt = m_fonts.find("default");
    if (defIt != m_fonts.end()) {
        return defIt->second;
    }
    // 最后手段：返回第一个可用字体，或创建占位空字体
    if (!m_fonts.empty()) {
        return m_fonts.begin()->second;
    }
    // 如果完全没有字体，尝试从文件加载
    static sf::Font fallback;
    static bool triedLoad = false;
    if (!triedLoad) {
        triedLoad = true;
        // 尝试常见系统字体路径
        if (fallback.loadFromFile("C:/Windows/Fonts/msyh.ttc") ||     // 微软雅黑（支持中文）
            fallback.loadFromFile("C:/Windows/Fonts/simhei.ttf") ||   // 黑体
            fallback.loadFromFile("C:/Windows/Fonts/arial.ttf") ||
            fallback.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") ||
            fallback.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            std::cout << "[ResourceManager] Loaded fallback system font" << std::endl;
        }
    }
    return fallback;
}

bool ResourceManager::hasFont(const std::string& key) const {
    return m_fonts.find(key) != m_fonts.end();
}

bool ResourceManager::loadTexture(const std::string& key, const std::string& path) {
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cerr << "[ResourceManager] Failed to load texture: " << path << std::endl;
        return false;
    }
    m_textures[key] = std::move(texture);
    return true;
}

sf::Texture& ResourceManager::getTexture(const std::string& key) {
    auto it = m_textures.find(key);
    if (it != m_textures.end()) {
        return it->second;
    }
    static sf::Texture dummy;
    return dummy;
}

bool ResourceManager::hasTexture(const std::string& key) const {
    return m_textures.find(key) != m_textures.end();
}

// 音效：预留接口
// 替换音效文件的方法：
//   1. 将 .wav 或 .ogg 文件放入 assets/audio/ 目录
//   2. 在代码中调用：
//      ResourceManager::getInstance().loadSound("shoot", "assets/audio/shoot.wav");
//   3. 播放音效：
//      sf::Sound sound(ResourceManager::getInstance().getSound("shoot"));
//      sound.play();
bool ResourceManager::loadSound(const std::string& key, const std::string& path) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(path)) {
        std::cerr << "[ResourceManager] Failed to load sound: " << path << std::endl;
        return false;
    }
    m_sounds[key] = std::move(buffer);
    return true;
}

sf::SoundBuffer& ResourceManager::getSound(const std::string& key) {
    auto it = m_sounds.find(key);
    if (it != m_sounds.end()) {
        return it->second;
    }
    static sf::SoundBuffer dummy;
    return dummy;
}

bool ResourceManager::hasSound(const std::string& key) const {
    return m_sounds.find(key) != m_sounds.end();
}

void ResourceManager::clear() {
    m_fonts.clear();
    m_textures.clear();
    m_sounds.clear();
    std::cout << "[ResourceManager] All resources cleared" << std::endl;
}
