#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <memory>

/**
 * Simple audio manager for sound effects and music.
 * Replace placeholder sound files in assets/audio/ with real .wav/.ogg files.
 *
 * To add real sounds:
 *   1. Place .wav or .ogg files in assets/audio/
 *   2. Update the loadAll() method with correct filenames
 *   3. Sounds will play automatically at the trigger points in game code
 */
class Audio {
public:
    static Audio& getInstance();

    Audio(const Audio&) = delete;
    Audio& operator=(const Audio&) = delete;

    // Load all available sound files (call once at startup)
    void loadAll();

    // Sound effects
    void playBubblePop();
    void playPlayerHurt();
    void playButtonClick();
    void playButtonHover();
    void playGameOver();
    void playShoot();

    // Background music
    void playMusic();
    void stopMusic();
    void setMusicVolume(float volume);   // 0-100
    void setSfxVolume(float volume);     // 0-100

    // Mute control
    void toggleMute();
    bool isMuted() const { return m_muted; }

private:
    Audio() = default;

    void playSound(const sf::SoundBuffer& buffer);

    // Sound buffers (loaded from files)
    sf::SoundBuffer m_popBuffer;
    sf::SoundBuffer m_hurtBuffer;
    sf::SoundBuffer m_clickBuffer;
    sf::SoundBuffer m_hoverBuffer;
    sf::SoundBuffer m_gameOverBuffer;
    sf::SoundBuffer m_shootBuffer;

    // Active sound players (multiple for overlapping sounds)
    std::vector<sf::Sound> m_sounds;
    size_t m_nextSound = 0;
    static constexpr size_t MAX_SOUNDS = 8;

    // Music
    sf::Music m_music;

    float m_sfxVolume = 50.f;
    float m_musicVolume = 30.f;
    bool m_muted = false;

    // Track which sounds loaded successfully
    bool m_hasPop = false;
    bool m_hasHurt = false;
    bool m_hasClick = false;
    bool m_hasHover = false;
    bool m_hasGameOver = false;
    bool m_hasShoot = false;
    bool m_hasMusic = false;
};
