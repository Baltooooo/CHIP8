#include "emu_sdl.h"
#include "emu_chip8.h"
#include <time.h>
#include <iostream>
#include <limits>
#include <fstream>

using namespace std;

string Emu_SDL::romPath = "";
uint32_t Emu_SDL::screen_width = 64;
uint32_t Emu_SDL::screen_height = 32;
uint32_t Emu_SDL::scale_factor = 20;
uint32_t Emu_SDL::fg_color = 0x00FF0000;
uint32_t Emu_SDL::bg_color = 0x0000000;
uint32_t Emu_SDL::square_wave_freq = 440;
uint32_t Emu_SDL::audio_sample_rate = 44100;
float Emu_SDL::volume = 3000;
SDL_AudioDeviceID Emu_SDL::device = 0;


void Emu_SDL::intro(){
    ifstream config("CHIP8-config.txt", ios::in);
    string line;
    while(getline(config, line)){
        if(line.rfind("romPath=", 0) == 0)
            romPath = line.substr(8);
        if(line.rfind("model=", 0) == 0)
            model = line.substr(6);
    }
    cout << romPath << endl;
    if(model == "chip8")
        emulator.setMode(CHIP8);
    if(model == "superLegacy")
        emulator.setMode(SUPERCHIP_LEGACY);
    if(model == "superModern")
        emulator.setMode(SUPERCHIP_MODERN);
    if(model == "xochip")
        emulator.setMode(XOCHIP);


}

void audio_callback(void* userdata, unsigned char* stream, int len){

    uint16_t *audio_data = (uint16_t*)stream;

    uint32_t square_wave_period = Emu_SDL::getAudioSampleRate()/Emu_SDL::getFreq();
    uint32_t half_square_wave_period = square_wave_period/2;

    static uint32_t wave_counter = 0;

    for(int i = 0; i < len/2; i++){

        if(wave_counter > half_square_wave_period)
            audio_data[i] = Emu_SDL::getVolume();
        else
            audio_data[i] = -Emu_SDL::getVolume();

        wave_counter++;
        if(wave_counter >= square_wave_period){
            wave_counter = 0;
        }
    }
}

void Emu_SDL::init_sdl(){



    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0){
        SDL_Log("Could not start SDL subsystems.", SDL_GetError());
        exit(0);
    }

    Emu_SDL::window = SDL_CreateWindow("CHIP++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        screen_width * scale_factor, screen_height * scale_factor, 0);

    if(!window){
        SDL_Log("Could not create window", SDL_GetError());
        exit(0);
    }

    Emu_SDL::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!renderer){
        SDL_Log("Could not create renderer", SDL_GetError());
        exit(0);
    }


    Emu_SDL::want.freq = 44100;
    Emu_SDL::want.format = AUDIO_S16LSB;
    Emu_SDL::want.channels = 1;
    Emu_SDL::want.samples = 1;
    Emu_SDL::want.callback = audio_callback;

    Emu_SDL::device = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

}

void Emu_SDL::input_handler(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                            case SDLK_ESCAPE:
                                state = QUIT;
                                break;
                            case SDLK_SPACE:
                                if(state == RUNNING){
                                    state = PAUSED;
                                }else{
                                    state = RUNNING;
                                }
                                break;

                            case SDLK_1: emulator.setKeyState(0x1, true); break;
                            case SDLK_2: emulator.setKeyState(0x2, true); break;
                            case SDLK_3: emulator.setKeyState(0x3, true); break;
                            case SDLK_4: emulator.setKeyState(0xC, true); break;

                            case SDLK_q: emulator.setKeyState(0x4, true); break;
                            case SDLK_w: emulator.setKeyState(0x5, true); break;
                            case SDLK_e: emulator.setKeyState(0x6, true); break;
                            case SDLK_r: emulator.setKeyState(0xD, true); break;

                            case SDLK_a: emulator.setKeyState(0x7, true); break;
                            case SDLK_s: emulator.setKeyState(0x8, true); break;
                            case SDLK_d: emulator.setKeyState(0x9, true); break;
                            case SDLK_f: emulator.setKeyState(0xE, true); break;

                            case SDLK_z: emulator.setKeyState(0xA, true); break;
                            case SDLK_x: emulator.setKeyState(0x0, true); break;
                            case SDLK_c: emulator.setKeyState(0xB, true); break;
                            case SDLK_v: emulator.setKeyState(0xF, true); break;

                            case SDLK_SEMICOLON: emulator.toggleWrapX(); break;
                            case SDLK_l: emulator.toggleWrapY(); break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym){

                            case SDLK_1: emulator.setKeyState(0x1, false); break;
                            case SDLK_2: emulator.setKeyState(0x2, false); break;
                            case SDLK_3: emulator.setKeyState(0x3, false); break;
                            case SDLK_4: emulator.setKeyState(0xC, false); break;

                            case SDLK_q: emulator.setKeyState(0x4, false); break;
                            case SDLK_w: emulator.setKeyState(0x5, false); break;
                            case SDLK_e: emulator.setKeyState(0x6, false); break;
                            case SDLK_r: emulator.setKeyState(0xD, false); break;

                            case SDLK_a: emulator.setKeyState(0x7, false); break;
                            case SDLK_s: emulator.setKeyState(0x8, false); break;
                            case SDLK_d: emulator.setKeyState(0x9, false); break;
                            case SDLK_f: emulator.setKeyState(0xE, false); break;

                            case SDLK_z: emulator.setKeyState(0xA, false); break;
                            case SDLK_x: emulator.setKeyState(0x0, false); break;
                            case SDLK_c: emulator.setKeyState(0xB, false); break;
                            case SDLK_v: emulator.setKeyState(0xF, false); break;
                    }
                    break;

                }
            }

    }



void Emu_SDL::cleanup(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Emu_SDL::clear_screen(){
    uint32_t r = (bg_color >> 24) & 0xFF;
    uint32_t g = (bg_color >> 16) & 0xFF;
    uint32_t b = (bg_color >> 8) & 0xFF;
    uint32_t a = (bg_color) & 0xFF;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}

void Emu_SDL::update_screen(){
    SDL_Rect rect;

    uint32_t bg_r = (bg_color >> 24) & 0xFF;
    uint32_t bg_g = (bg_color >> 16) & 0xFF;
    uint32_t bg_b = (bg_color >> 8) & 0xFF;
    uint32_t bg_a = (bg_color) & 0xFF;

    uint32_t fg_r = (fg_color >> 24) & 0xFF;
    uint32_t fg_g = (fg_color >> 16) & 0xFF;
    uint32_t fg_b = (fg_color >> 8) & 0xFF;
    uint32_t fg_a = (fg_color) & 0xFF;

    for(int i = 0; i < getHeight(); i++){
        for(int j = 0; j < getWidth(); j++){
            rect.x = j * (emulator.getDisplayMode() ? scale_factor / 2 : scale_factor);
            rect.y = i * (emulator.getDisplayMode() ? scale_factor / 2 : scale_factor);
            rect.w = scale_factor;
            rect.h = scale_factor;

            if(emulator.getDisplayPixel(i, j)){

                SDL_SetRenderDrawColor(renderer, fg_r, fg_g, fg_b, fg_a);
                SDL_RenderFillRect(renderer, &rect);
            }else{
                SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, bg_a);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }


    SDL_RenderPresent(renderer);
}

void Emu_SDL::main_loop(){
    srand(time(NULL) ^ getpid() ^ (clock() & 0xFFFF));
    int speedHandler = 60;

    if(emulator.getMode() == SUPERCHIP_MODERN || emulator.getMode() == XOCHIP)
        speedHandler = 50;

    init_sdl();
    emulator.init_emulator();


    while(state != QUIT){
        input_handler();

        if(state == PAUSED) continue;


        uint64_t start_time = SDL_GetPerformanceCounter();

        for(uint32_t i = 0; i < emulator.instructions_per_second / speedHandler; i++){
            emulator.emulate_instruction();
        }

        uint64_t end_time = SDL_GetPerformanceCounter();

        double time_elapsed = (end_time - start_time) * 1000 / SDL_GetPerformanceFrequency();


        SDL_Delay(16.67 > time_elapsed ? 16.67 - time_elapsed : 0);
        if(emulator.getDraw())
            update_screen();

        emulator.update_timers();
    }

    cleanup();

}

