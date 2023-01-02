// P4Boy.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "P4Boy.h"
#include <chrono>

int main()
{
    unsigned int tick = 0;
    auto on_date = std::chrono::high_resolution_clock::now();
    auto p4boy = P4Boy::P4Boy();

    // passed
    //p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/01-special.gb");

    //p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/03-op sp,hl.gb");

    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/11-op a,(hl).gb");
    // 

    p4boy.LoadRom("Roms/Tetris.gb");
    while (true)
    {
        ++tick;
        p4boy.Tick();
        if (tick == 150000)
        {
            auto current_date = std::chrono::high_resolution_clock::now();
            std::cout << "Megahertz: " << (tick * 1000.0f / (std::chrono::duration_cast<std::chrono::nanoseconds>(current_date - on_date).count())) << '\n';
            on_date = std::chrono::high_resolution_clock::now();
            tick = 0;
        }
    }
}

