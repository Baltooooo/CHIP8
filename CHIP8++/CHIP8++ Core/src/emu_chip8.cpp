#include "emu_chip8.h"
#include "emu_sdl.h"
#include <string.h>
#include <iostream>
#include <fstream>


using namespace std;



void Emu_chip8::init_emulator(){
    const uint32_t starting_point = 0x200;
    const uint8_t font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0
        0x20, 0x60, 0x20, 0x20, 0x70,   // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,   // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,   // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
        0xF0, 0x80, 0xF0, 0x80, 0x80,   // F
    };
    if(display_mode){
        Emu_SDL::setWidth(128);
        Emu_SDL::setHeight(64);
    }else{
        Emu_SDL::setWidth(64);
        Emu_SDL::setHeight(32);
    }
    Emu_chip8::PC = starting_point;
    Emu_chip8::stack_ptr = &stack[0];


    memcpy(&ram[0], font, sizeof(font));


    ifstream romFile(Emu_SDL::getPath(), ios::binary);
    if(!romFile){
        cout << "Could not open file" << endl;
        exit(0);
    }

    romFile.seekg(0, std::ios::end);
    size_t rom_size = romFile.tellg();
    romFile.seekg(0, std::ios::beg);


    romFile.read(reinterpret_cast<char*>(&ram[starting_point]), rom_size);


    if(!romFile){
        cout << "Could not find ROM file" << endl;
        exit(0);

    }

    romFile.close();

}

void Emu_chip8::update_timers(){
    if(delay_timer > 0)
        delay_timer--;
    if(sound_timer > 0){
        sound_timer--;
        SDL_PauseAudioDevice(Emu_SDL::getDevice(), 0);
    }else
        SDL_PauseAudioDevice(Emu_SDL::getDevice(), 1);

}

void Emu_chip8::emulate_instruction(){
    opcode = ram[PC] << 8 | ram[PC+1];
    PC+=2;
    printf("PC: %04X , Opcode: %04X, Description:", PC, opcode);

    NNN = opcode & 0x0FFF;
    NN = opcode & 0x00FF;
    N = opcode & 0x000F;
    X = opcode >> 8 & 0x0F;
    Y = opcode >> 4 & 0x0F;

    bool carry = 0;


    switch((opcode >> 12) & 0x0F){

        case 0x00:
            switch(NN){

                case 0xE0:
                    //0x00E0: Clear the screen
                    printf("Clearing the screen... \n");
                    draw = 1;
                    memset(&display, false, sizeof display);
                    memset(&hi_display, false, sizeof hi_display);
                    break;

                case 0xEE:
                    //0x00EE: Return from subroutine
                    printf("Returning from subroutine... \n");
                    *stack_ptr--;
                    PC = *stack_ptr;
                    break;

                case 0xFE:
                    //0x00FE: Switch to lores mode
                    printf("Switching to lores mode... \n");
                    display_mode = 0;
                    draw = 1;
                    Emu_SDL::setWidth(64);
                    Emu_SDL::setHeight(32);
                    break;

                case 0xFF:
                    //0x00FF: Switch to hires mode
                    printf("Switching to hires mode... \n");
                    display_mode = 1;
                    draw = 1;
                    Emu_SDL::setWidth(128);
                    Emu_SDL::setHeight(64);
                    break;

                case 0xFB:{
                    //0x00FB: Scroll 4 pixels to the right
                    int pixel_offset = 4;

                    if(extension == SUPERCHIP_LEGACY)
                        pixel_offset = 2;

                    printf("Scrolling %d pixels to the right... \n", pixel_offset);

                    int counter = 0;

                    while(counter < pixel_offset) {
                        for(int i = 0; i < Emu_SDL::getHeight(); i++) {
                            for(int j = Emu_SDL::getWidth() - 1; j >= 0; j--) {
                                display[i][j] = display[i][j-1];
                                hi_display[i][j] = hi_display[i][j-1];
                            }

                        }
                        for(int k = 0; k < Emu_SDL::getHeight(); k++) {
                        display[k][0] = 0;
                        hi_display[k][0] = 0;
                        }
                        counter++;
                    }

                    draw = 1;
                }

                break;

                case 0xFC:{
                    //0x00FB: Scroll 4 pixels to the left
                   int pixel_offset = 4;

                    if(extension == SUPERCHIP_LEGACY)
                        pixel_offset = 2;

                    printf("Scrolling %d pixels to the left... \n", pixel_offset);

                    int counter = 0;

                    while(counter < pixel_offset) {
                        for(int i = 0; i < Emu_SDL::getHeight(); i++) {
                            for(int j = 0; j < Emu_SDL::getWidth() - 1; j++) {
                                display[i][j] = display[i][j+1];
                                hi_display[i][j] = hi_display[i][j+1];

                            }
                            for(int k = 0; k < Emu_SDL::getHeight(); k++) {
                            display[k][Emu_SDL::getWidth() - 1] = 0;
                            hi_display[k][Emu_SDL::getWidth() - 1] = 0;
                        }

                        }
                        counter++;
                    }

                    draw = 1;
                }

                break;

                default:

                    switch(Y){

                    case 0x0C:{
                        printf("Scrolling N(%02X) pixels down... \n", N);


                        int height = N;
                        int counter = 0;

                        if(extension == SUPERCHIP_LEGACY)
                            height = N/2;

                        while(counter < height) {
                            for (int i = Emu_SDL::getHeight() - 1; i > 0; i--) {
                                for(int j = 0; j < Emu_SDL::getWidth(); j++) {
                                    display[i][j] = display[i-1][j];
                                    hi_display[i][j] = hi_display[i-1][j];
                                }
                            }
                            for(int k = 0; k < Emu_SDL::getWidth(); k++) {
                            display[0][k] = 0;
                            hi_display[0][k] = 0;
                            }
                            counter++;
                        }

                        draw = 1;
                    }
                    break;

                    case 0x0D:{
                        printf("Scrolling N(%02X) pixels up... \n", N);

                        int height = N;
                        int counter = 0;

                        if(extension == SUPERCHIP_LEGACY)
                            height = N/2;

                        while(counter < height) {
                            for(int i = 0; i < Emu_SDL::getHeight() - 1; i++) {
                                for(int j = 0; j < Emu_SDL::getWidth(); j++) {
                                    display[i][j] = display[i+1][j];
                                    hi_display[i][j] = hi_display[i+1][j];
                                    }
                                }
                            for(int k = 0; k < Emu_SDL::getWidth(); k++) {
                            display[Emu_SDL::getHeight() - 1][k] = 0;
                            hi_display[Emu_SDL::getHeight() - 1][k] = 0;
                            }
                            counter++;
                        }

                        draw = 1;

                    }
                    break;
                }
                break;
            }
            break;

        case 0x01:
            //0x1NNN: Jump to address NNN
            printf("Jumping to address NNN(%03X)... \n", NNN);
            PC = NNN;
            break;

        case 0x02:
            //0x2NNN: Call subroutine at NNN
            printf("Calling subroutine at address NNN(%03X)... \n", NNN);
            *stack_ptr++ = PC;
            PC = NNN;
            break;

        case 0x03:
            //0x3XNN: Skip next instruction if VX == NN
            if(V[X] == NN){
                printf("V[X](%02X) and NN(%02X) are equal. Skipping next instruction... \n", V[X], NN);
                PC+=2;
            }else{
                printf("V[X](%02X) and NN(%02X) are not equal.\n", V[X], NN);
            }
                break;

        case 0x04:
            //0x4NN: Skip next instruction if VX != NN
            if(V[X] != NN){
                printf("V[X](%02X) and NN(%02X) are not equal. Skipping next instruction... \n", V[X], NN);
                PC+=2;
            }else{
                printf("V[X](%02X) and NN(%02X) are equal.\n", V[X], NN);
            }
                break;

        case 0x05:
            //0x5XY0: Checks if VX and VY are equal. If so, skip the next instruction
            if(V[X] == V[Y]){
                printf("V[X](%02X) and V[Y](%02X) are equal. Skipping next instruction... \n", V[X], V[Y]);
                PC+=2;
            }else{
                printf("V[X](%02X) and NN(%02X) are not equal.\n", V[X], V[Y]);
            }
                break;

        case 0x06:
            //0x6XNN: Sets VX to NN
            printf("Setting V[X](%02X) to NN(%02X)... \n", V[X], NN);
            V[X] = NN;
            break;

        case 0x07:
            //0x7XNN: Add NN to VX
            printf("Adding NN(%02X) to V[X](%02X)... \n", NN, V[X]);
            V[X] += NN;
            break;

        case 0x08:
            //0x8---
            switch(N){
            case 0:
                //0x8XY0: Set V[X] to V[Y]
                printf("Setting V[X](%02X) to V[Y](%02X)... \n", V[X], V[Y]);
                V[X] = V[Y];
                break;

            case 1:
                //0x8XY1: V[X] |= V[Y]
                printf("Performing bitwise OR on V[X](%02X) and V[Y](%02X)... \n", V[X], V[Y]);
                V[X] |= V[Y];
                if(extension == CHIP8)
                    V[0xF] = 0;
                break;

            case 2:
                //0x8XY2: V[X] &= V[Y]
                printf("Performing bitwise AND on V[X](%02X) and V[Y](%02X)... \n", V[X], V[Y]);
                V[X] &= V[Y];
                if(extension == CHIP8)
                    V[0xF] = 0;
                break;

            case 3:
                //0x8XY3: V[X] ^= V[Y]
                printf("Performing bitwise XOR on V[X](%02X) and V[Y](%02X)... \n", V[X], V[Y]);
                V[X] ^= V[Y];
                if(extension == CHIP8)
                    V[0xF] = 0;
                break;

            case 4:
                //0x8XY4: V[X] += V[Y]. V[F] = 1 if there is an overflow and 0 if not
                printf("Adding V[X](%02X) to V[Y](%02X).", V[X], V[Y]);
                carry = ((uint16_t)V[X] + V[Y]) > 255;
                if(carry)
                    printf("There was no overflow... \n");
                else
                    printf("There was an overflow...\n");
                V[X] += V[Y];
                V[0xF] = carry;
                break;

            case 5:
                //0x8XY5: V[X] -= V[Y]. V[F] = 0 if there is an underflow and 1 if not
                printf("Subtracting V[Y](%02X) from V[X](%02X). \n", V[Y], V[X]);
                carry = V[X] >= V[Y];
                V[X] -= V[Y];
                V[0xF] = carry;
                break;

            case 6:
                //08XY6: Shift V[X] to the right by 1 and store rightmost bit before the shift to V[F].
                printf("Storing rightmost bit of V[X](%02X) to V[F](%02X).", V[X], V[0xF]);
                carry = V[X] & 1;
                printf("V[F] is now %02X.", V[0xF]);
                printf("Shifting V[X](%02X) to the right by 1 bit... \n", V[X]);
                if(extension == CHIP8 || extension == XOCHIP)
                    V[X] = V[Y] >> 1;
                else
                    V[X] >>= 1;
                V[0xF] = carry;
                break;

            case 7:
                //0x8XY7: V[X] = V[Y] - V[X]. V[F] = 0 if there is an underflow and 1 if not
                printf("Setting V[X] to V[X](%02X) - V[Y](%02X).", V[X], V[Y]);
                carry = V[Y] >= V[X];
                V[X] = V[Y] - V[X];
                V[0xF] = carry;
                break;

            case 0x0E:
                //08XYE: Shift V[X] to the left by 1 and store leftmost bit before the shift to V[F]
                printf("Storing leftmost bit of V[X](%02X) to V[F](%02X).", V[X], V[0xF]);
                carry = V[X] & 128;
                printf("V[F] is now %02X.", V[0xF]);
                printf("Shifting V[X](%02X) to the left by 1 bit... \n", V[X]);
                if(extension == CHIP8 || extension == XOCHIP)
                    V[X] = V[Y] << 1;
                else
                    V[X] <<= 1;
                V[0xF] = carry;
                break;
            }
            break;

        case 0x09:
            //0x9XY0: If V[X] != V[Y], skip next instruction.
            if(V[X] != V[Y]){
                printf("V[X](%02X) and V[Y](%02X) are not equal. Skipping next instruction... \n", V[X], V[Y]);
                PC+=2;
            }else{
                printf("V[X](%02X) and V[Y](%02X) are equal.\n", V[X], V[Y]);
            }
            break;


        case 0x0A:
            //0xANNN: Sets I to NNN
            printf("Setting I(%01X) to NNN(%03X)... \n", I, NNN);
            I = NNN;
            break;

        case 0x0B:
            //0xBNNN: Jump to address V0 + NNN
            printf("Jumping to address %03X... \n", V[0]+NNN);
            if(extension == CHIP8 || extension == XOCHIP)
                PC = V[0] + NNN;
            else{
                X = NNN % 0x00F;
                PC = V[X] + NNN;
            }
            break;

        case 0x0C:
            //0xCXNN: Set V[X] to bitwise and operation between random number(0-255) and NN
            printf("Setting V[X](%02X) to *random* & NN(%02X)... \n", V[X], NN);
            V[X] = (rand() % 256) & NN;
            break;

        case 0x0D:{
            //0xDXYN: Draw an N high sprite at coords X and Y
            printf("Drawing sprite N(%01X) tall at coordinates X(%01X) and Y(%01X)... \n", N, V[X], V[Y]);
            uint8_t X_coord = V[X] % Emu_SDL::getWidth();
            uint8_t Y_coord = V[Y] % Emu_SDL::getHeight();

            const uint8_t X_original = X_coord;

            V[0xF] = 0;

            for(int i = 0; i < N; i++){
                const uint8_t sprite_data = ram[I + i];
                X_coord = X_original;
                for(int j = 7; j >= 0; j--){
                    const bool sprite_bit = sprite_data & (1 << j);

                    if(display[Y_coord][X_coord] && sprite_bit || hi_display[Y_coord][X_coord] && sprite_bit)
                        V[0xF] = 1;

                    if(display_mode){
                        hi_display[Y_coord][X_coord] ^= sprite_bit;
                    }else{
                        display[Y_coord][X_coord] ^= sprite_bit;
                    }


                    if(++X_coord >= Emu_SDL::getWidth()){
                        if(extension == XOCHIP && wrapX)
                            X_coord %= Emu_SDL::getWidth();
                        else
                            break;
                    }
                }
                if(++Y_coord >= Emu_SDL::getHeight()){
                    if(extension == XOCHIP && wrapY)
                        Y_coord %= Emu_SDL::getHeight();
                    else
                        break;
                }
            }
            draw = 1;
        }
        break;

        case 0x0E:
            switch(NN){
                case 0x9E:
                    //0xEX9E: Skips the next instruction if the key stored in V[X] is pressed
                    if(keypad[V[X]]){
                        printf("The key in V[X](%01X) was pressed.Skipping next instruction... \n", V[X]);
                        PC+=2;
                    }else{
                        printf("The key in V[X](%01X) was NOT pressed. \n", V[X]);

                    }
                    break;

                case 0xA1:
                    //0xEX9E: Skips the next instruction if the key stored in V[X] is NOT pressed
                    if(!keypad[V[X]]){
                        printf("The key in V[X](%01X) was NOT pressed.Skipping next instruction... \n", V[X]);
                        PC+=2;
                    }else{
                        printf("The key in V[X](%01X) was pressed. \n", V[X]);

                    }
                    break;
            }
            break;

        case 0x0F:
            switch(NN){
                case 0x07:
                    //0xFX07: Sets V[X] to the value of the delay timer;
                    printf("Setting V[X](%02X) to delay timer(%02X)... \n", V[X], delay_timer);
                    V[X] = delay_timer;
                    break;

                case 0x0A:{
                    //0xFX0A: Awaiting key input
                    printf("Awaiting key input... \n");
                    bool pressed = false;
                    uint8_t key = -1;
                    for(int i = 0; key == -1 && i < 16; i++){
                        if(keypad[i]){
                            pressed = true;
                            key = i;
                            break;
                        }
                    }

                    if(!pressed){
                        PC-=2;
                    }else{
                        if(keypad[key]){
                            PC-=2;
                        }
                        else{
                            V[X] = key;
                            key = -1;
                            pressed = false;
                        }
                    }

                }
                    break;

                case 0x15:
                    //0xFX15: Sets delay timer to V[X]
                    printf("Setting V[X](%02X) to delay timer(%02X)... \n", V[X], delay_timer);
                    delay_timer = V[X];
                    break;

                case 0x18:
                    //0xFX18: Sets sound timer to V[X]
                    printf("Setting V[X](%02X) to sound timer(%02X)... \n", V[X], sound_timer);
                    sound_timer = V[X];
                    break;

                case 0x1E:
                    //0xFX1E: Adds VX to I
                    printf("Adding V[X](%02X) to I(%02X)... \n");
                    I += V[X];
                    break;

                case 0x29:
                    //0xFX29: Sets I to the location of the sprite for the character in V[X]. 0-F are represented in a 4x5 format.
                    printf("Setting I to location of character sprite in V[X](%02X)... \n", V[X]);
                    I = V[X] * 5;
                    break;

                case 0x33:{
                    //0xFX33: Stores the binary-coded decimal representation of VX, with
                    //the hundreds digit in memory at location in I, the tens digit at location I+1,
                    // and the ones digit at location I+2.
                    printf("Setting BCD of V[X](%02X) to I offset... \n");
                    uint8_t bcd = V[X];
                    ram[I+2] = bcd % 10;
                    bcd /= 10;
                    ram[I+1] = bcd % 10;
                    bcd /= 10;
                    ram[I] = bcd;
                }
                    break;

                case 0x55:
                    //0xFX55: Stores from V[0] to V[X] (including V[X]) in memory, starting at address I.
                    //The offset from I is increased by 1 for each value written, but I itself is left unmodified.
                    printf("Storing V[0] to V[X] in I offset... \n");
                    for(int i = 0; i <= X; i++){
                        if(extension == CHIP8 || extension == XOCHIP)
                            ram[I++] = V[i];
                        else
                            ram[I + i] = V[i];
                    }
                    break;

                case 0x65:
                    //0xF65:Fills from V[0] to V[X] (including V[X]) with values from memory, starting at address I.
                    //The offset from I is increased by 1 for each value read, but I itself is left unmodified.
                    printf("Filling V[0] to V[X] with I offset... \n");
                    for(int i = 0; i <= X; i++){
                        if(extension == CHIP8 || extension == XOCHIP)
                            V[i] = ram[I++];
                        else
                            V[i] = ram[I + i];
                    }
                    break;

            }
            break;



        default:
            cout << "Unimplemented/Nonexistent opcode" << endl;


    }

}
