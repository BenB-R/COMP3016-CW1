// AudioHandler.h
#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <SDL.h>

class AudioHandler {
public:
    AudioHandler();
    ~AudioHandler();

    void loadAudio(const char* filename);
    void playAudio();
    void stopAudio();
    void resetAudioBuffer();

private:
    SDL_AudioDeviceID audioDevice;
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8* wavBuffer;
    Uint32 originalWavLength;

    static void audioCallback(void* userdata, Uint8* stream, int len);
};

#endif // AUDIOHANDLER_H
