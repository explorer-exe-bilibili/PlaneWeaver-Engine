#pragma once
#include <string>
#include <map>
#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>

namespace core {

class Audio {
public:
    Audio();
    ~Audio();
    
    // Initialize the audio system
    bool init();
    
    // Music control (background music)
    bool loadMusic(const std::string& id, const std::string& path);
    bool playMusic(const std::string& id, int loop = -1);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    void setMusicVolume(int volume); // 0-128
    bool isMusicPlaying() const;
    bool isMusicPaused() const;
    
    // Sound effect control (short sound effects)
    bool loadSound(const std::string& id, const std::string& path);
    int playSound(const std::string& id, int loop = 0); // Returns the channel ID
    void stopSound(int channel);
    void stopAllSounds();
    void pauseSound(int channel);
    void resumeSound(int channel);
    void pauseAllSounds();
    void resumeAllSounds();
    void setSoundVolume(const std::string& id, int volume); // 0-128
    void setChannelVolume(int channel, int volume); // 0-128
    bool isSoundPlaying(int channel) const;
    bool isSoundPaused(int channel) const;
    
    // System control
    void pauseAll(); // Pause all music and sound effects
    void resumeAll(); // Resume all music and sound effects
    void stopAll(); // Stop all music and sound effects
    void cleanup();

    void unloadMusic(const std::string& id);
    void unloadSound(const std::string& id);
    
    // Check if the system is initialized
    bool isInitialized() const { return initialized; }

private:
    std::map<std::string, Mix_Music*> musics;
    std::map<std::string, Mix_Chunk*> sounds;
    std::map<std::string, std::vector<int>> activeChannels; // Tracks the channels being used by each sound ID
    bool initialized = false;
    
    // Used for channel completion callback
    void removeChannelFromTracking(int channel);
};

} // namespace core
