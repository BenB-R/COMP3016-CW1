// AudioHandler.cpp
#include "AudioHandler.h"
#include <iostream>

AudioHandler::AudioHandler() : audioDevice(0), wavBuffer(nullptr), wavLength(0) {
    // Initialize SDL Audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
}

AudioHandler::~AudioHandler() {
    stopAudio();  // Ensure the audio is stopped
    if (audioDevice != 0) {
        SDL_CloseAudioDevice(audioDevice);
    }
    if (wavBuffer != nullptr) {
        SDL_FreeWAV(wavBuffer);
    }
}

void AudioHandler::loadAudio(const char* filename) {
    if (SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength) == nullptr) {
        std::cerr << "Failed to load WAV: " << SDL_GetError() << std::endl;
        return;
    }

    // Set the audio specification and callback function
    wavSpec.callback = audioCallback;
    wavSpec.userdata = this;  // Set 'this' pointer as userdata

    // Open the audio device with the specified callback function and userdata
    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (audioDevice == 0) {
        std::cerr << "Failed to open audio: " << SDL_GetError() << std::endl;
    }
    originalWavLength = wavLength;
}

void AudioHandler::playAudio() {
    if (audioDevice != 0) {
        SDL_PauseAudioDevice(audioDevice, 0);  // Start playing
    }
}

void AudioHandler::stopAudio() {
    if (audioDevice != 0) {
        SDL_PauseAudioDevice(audioDevice, 1);  // Stop playing
    }
}

void AudioHandler::audioCallback(void* userdata, Uint8* stream, int len) {
    AudioHandler* handler = static_cast<AudioHandler*>(userdata);
    if (!handler) {
        SDL_memset(stream, 0, len);  // Silence the output
        std::cout << "no work\n";
        return;
    }

    if (handler->wavLength == 0) {
        // If we have reached the end of the audio data, loop back to the start
        handler->resetAudioBuffer();
    }

    Uint32 length = (Uint32)len;
    length = (length > handler->wavLength ? handler->wavLength : length);

    SDL_memcpy(stream, handler->wavBuffer, length);  // Copy the audio data
    handler->wavBuffer += length;  // Move the buffer forward
    handler->wavLength -= length;  // Decrease the remaining length

    // If the buffer has been exhausted, loop back to the start
    if (handler->wavLength == 0) {
        handler->resetAudioBuffer();
    }
}

void AudioHandler::resetAudioBuffer() {
    wavBuffer -= wavLength;  // Reset buffer pointer to the start
    wavLength = originalWavLength;  // Reset length to the original value
}