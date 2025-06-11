#include <iostream>
#include <SDL2/SDL.h>
#include "emu_sdl.h"
#include "emu_chip8.h"

using namespace std;

Emu_SDL chip8;

int main(int argc, char *argv[]){

    chip8.intro();
    chip8.main_loop();

}
