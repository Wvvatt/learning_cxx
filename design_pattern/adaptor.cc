#include <iostream>
#include <string>

// 旧的音频播放器类
class OldAudioPlayer {
public:
    void playMp3(const std::string& filename) {
        std::cout << "Playing mp3 file: " << filename << std::endl;
    }
};

// 新的音频播放器接口
class NewAudioPlayer {
public:
    virtual void play(const std::string& audioType, const std::string& filename) = 0;
};

// 适配器类
class AudioPlayerAdapter : public NewAudioPlayer {
private:
    OldAudioPlayer* oldPlayer;

public:
    AudioPlayerAdapter(OldAudioPlayer* player) : oldPlayer(player) {}

    void play(const std::string& audioType, const std::string& filename) override {
        if (audioType == "mp3") {
            oldPlayer->playMp3(filename);
        } else if (audioType == "wav") {
            std::cout << "Playing wav file: " << filename << std::endl;
        } else {
            std::cout << "Invalid audio type: " << audioType << std::endl;
        }
    }
};

// 使用适配器
int main() {
    OldAudioPlayer* oldPlayer = new OldAudioPlayer();
    NewAudioPlayer* adapter = new AudioPlayerAdapter(oldPlayer);

    adapter->play("mp3", "song.mp3");
    adapter->play("wav", "song.wav");
    adapter->play("flac", "song.flac");

    delete oldPlayer;
    delete adapter;

    return 0;
}