#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <memory>

/**
 * 单例资源管理器
 * 管理字体、纹理、音效的加载和访问
 * 音效接口已预留，替换音效文件的方法见各 loadSound 注释
 */
class ResourceManager {
public:
    static ResourceManager& getInstance();

    // 禁止拷贝
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // 字体
    bool loadFont(const std::string& key, const std::string& path);
    sf::Font& getFont(const std::string& key = "default");
    bool hasFont(const std::string& key) const;

    // 纹理
    bool loadTexture(const std::string& key, const std::string& path);
    sf::Texture& getTexture(const std::string& key);
    bool hasTexture(const std::string& key) const;

    // 音效（预留接口）
    // 替换音效文件：将 .wav/.ogg 文件放入 assets/audio/ 目录，
    // 然后调用 loadSound("key", "assets/audio/yourfile.wav")
    bool loadSound(const std::string& key, const std::string& path);
    sf::SoundBuffer& getSound(const std::string& key);
    bool hasSound(const std::string& key) const;

    // 清空所有资源
    void clear();

private:
    ResourceManager();
    std::map<std::string, sf::Font> m_fonts;
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::SoundBuffer> m_sounds;
};
