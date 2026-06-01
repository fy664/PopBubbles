#include "core/Audio.h"
#include <iostream>

Audio& Audio::getInstance() {
    static Audio instance;
    return instance;
}

void Audio::loadAll() {
    // Pre-allocate sound players for overlapping playback
    m_sounds.resize(MAX_SOUNDS);

    // ========================================
    // Load sound effects
    // To use real sounds, place .wav/.ogg files in assets/audio/
    // and update the filenames below.
    // Example:
    //   m_hasPop = m_popBuffer.loadFromFile("assets/audio/pop.wav");
    // ========================================

    // m_hasPop = m_popBuffer.loadFromFile("assets/audio/pop.wav");
    // m_hasHurt = m_hurtBuffer.loadFromFile("assets/audio/hurt.wav");
    // m_hasClick = m_clickBuffer.loadFromFile("assets/audio/click.wav");
    // m_hasHover = m_hoverBuffer.loadFromFile("assets/audio/hover.wav");
    // m_hasGameOver = m_gameOverBuffer.loadFromFile("assets/audio/gameover.wav");
    // m_hasShoot = m_shootBuffer.loadFromFile("assets/audio/shoot.wav");

    // m_hasMusic = m_music.openFromFile("assets/audio/music.ogg");
    // if (m_hasMusic) {
    //     m_music.setLoop(true);
    //     m_music.setVolume(m_musicVolume);
    // }

    // Print loaded status
    std::cout << "[Audio] Sound system initialized" << std::endl;
    if (!m_hasPop && !m_hasHurt && !m_hasClick) {
        std::cout << "[Audio] No sound files found - running silently." << std::endl;
        std::cout << "[Audio] Place .wav/.ogg files in assets/audio/ and uncomment load lines in Audio.cpp." << std::endl;
    }
}

void Audio::playBubblePop() {
    if (m_hasPop) playSound(m_popBuffer);
}

void Audio::playPlayerHurt() {
    if (m_hasHurt) playSound(m_hurtBuffer);
}

void Audio::playButtonClick() {
    if (m_hasClick) playSound(m_clickBuffer);
}

void Audio::playButtonHover() {
    if (m_hasHover) playSound(m_hoverBuffer);
}

void Audio::playGameOver() {
    if (m_hasGameOver) playSound(m_gameOverBuffer);
}

void Audio::playShoot() {
    if (m_hasShoot) playSound(m_shootBuffer);
}

void Audio::playMusic() {
    if (m_hasMusic && m_music.getStatus() != sf::Music::Playing) {
        m_music.play();
    }
}

void Audio::stopMusic() {
    if (m_hasMusic) {
        m_music.stop();
    }
}

void Audio::setMusicVolume(float volume) {
    m_musicVolume = volume;
    if (m_hasMusic) {
        m_music.setVolume(m_muted ? 0.f : m_musicVolume);
    }
}

void Audio::setSfxVolume(float volume) {
    m_sfxVolume = volume;
    for (auto& sound : m_sounds) {
        sound.setVolume(m_muted ? 0.f : m_sfxVolume);
    }
}

void Audio::toggleMute() {
    m_muted = !m_muted;
    setSfxVolume(m_sfxVolume);
    setMusicVolume(m_musicVolume);
}

void Audio::playSound(const sf::SoundBuffer& buffer) {
    if (m_muted) return;

    // Round-robin sound players for overlapping playback
    auto& sound = m_sounds[m_nextSound];
    m_nextSound = (m_nextSound + 1) % MAX_SOUNDS;

    sound.stop();
    sound.setBuffer(buffer);
    sound.setVolume(m_sfxVolume);
    sound.play();
}
