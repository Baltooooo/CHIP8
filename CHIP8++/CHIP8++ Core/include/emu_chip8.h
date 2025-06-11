#ifndef EMU_CHIP8_H
#define EMU_CHIP8_H
#include <iostream>
#include <SDL2/SDL.h>
#include <cstring>

using namespace std;


enum Emu_extension{
    CHIP8,
    SUPERCHIP_MODERN,
    SUPERCHIP_LEGACY,
    XOCHIP,
};

class Emu_chip8
{
    private:
        SDL_AudioDeviceID device;
        uint8_t ram[4096];
        bool display[64][128];
        bool hi_display[64][128];
        bool display_mode = 0;
        bool keypad[16];
        bool draw = 0;
        bool wrapX = true;
        bool wrapY = true;
        uint8_t V[16];
        uint16_t PC;
        uint16_t I;
        uint16_t stack[12];
        uint16_t *stack_ptr;
        uint16_t opcode;
        uint16_t NNN;
        uint8_t NN;
        uint8_t N;
        uint8_t X;
        uint8_t Y;
        uint8_t delay_timer;
        uint8_t sound_timer;


        Emu_extension extension = CHIP8;

    public:
        void intro();
        void init_emulator();
        void update_timers();
        int instructions_per_second = 500;
        void emulate_instruction();
        void toggleWrapX() {wrapX = !wrapX;}
        void toggleWrapY() {wrapY = !wrapY;}
        void setMode(Emu_extension i) {extension = i;}
        void setDisplay(bool i) {display_mode = i;}
        bool setKeyState(int i, bool state) {keypad[i] = state;}
        bool getDisplayMode() {return display_mode;}
        bool getDisplayPixel(int i, int j) {return display_mode ? hi_display[i][j] : display[i][j];}
        bool getDraw() {return draw;}
        Emu_extension getMode() {return extension;}

    friend class Emu_SDL;

};

#endif // EMU_CHIP8_H
