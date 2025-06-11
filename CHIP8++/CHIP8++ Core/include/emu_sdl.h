#ifndef EMU_SDL_H
#define EMU_SDL_H

#include <SDL2/SDL.h>
#include "emu_chip8.h"
#include <iostream>

using namespace std;

enum Emu_state{
    RUNNING,
    PAUSED,
    QUIT,
};

class Emu_SDL
{
    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_AudioSpec want,have;
        bool debug;
        string model;
        static string romPath;
        static SDL_AudioDeviceID device;
        static uint32_t screen_width;
        static uint32_t screen_height;
        static uint32_t scale_factor;
        static uint32_t fg_color;
        static uint32_t bg_color;
        static uint32_t square_wave_freq;
        static uint32_t audio_sample_rate;
        static float volume;
        Emu_state state = RUNNING;
        Emu_chip8 emulator;

    public:
        void init_sdl();
        void input_handler();
        void cleanup();
        void clear_screen();
        void update_screen();
        void intro();
        void main_loop();
        static string getPath() {return romPath;}
        static SDL_AudioDeviceID getDevice() {return Emu_SDL::device;}
        static float getVolume() {return volume;}
        static uint32_t getFreq() {return square_wave_freq;}
        static uint32_t getAudioSampleRate() {return audio_sample_rate;}
        static uint32_t getWidth() {return screen_width;}
        static uint32_t getHeight() {return screen_height;}
        static uint32_t getScale() {return scale_factor;}
        static uint32_t setScale(uint32_t i) {scale_factor = i;}
        static uint32_t setWidth(uint32_t i) {screen_width = i;}
        static uint32_t setHeight(uint32_t i) {screen_height = i;}


};



#endif // EMU_SDL_H
