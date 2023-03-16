
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "P4Boy.h"

#include "imgui.h"
#include "imgui-SFML.h"
#include "MemoryDebug.h"
#include "CPUDebug.h"
#include "CartridgeDebug.h"

void P4Boy_Loop(P4Boy::P4Boy & p4boy)
{
    unsigned int tick = 0;
    auto on_date = std::chrono::high_resolution_clock::now();

    while (true)
    {
        ++tick;
        p4boy.Tick();
        if (tick == 150000)
        {
            auto current_date = std::chrono::high_resolution_clock::now();
            //std::cout << "Megahertz: " << (tick * 1000.0f / (std::chrono::duration_cast<std::chrono::nanoseconds>(current_date - on_date).count())) << '\n';
            on_date = std::chrono::high_resolution_clock::now();
            tick = 0;
        }
    }
}


int main()
{
   
    auto p4boy = P4Boy::P4Boy();

    // Passed
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/01-special.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/02-interrupts.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/03-op sp,hl.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/04-op r,imm.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/05-op rp.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/06-ld r,r.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/08-misc instrs.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/09-op r,r.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/10-bit ops.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/11-op a,(hl).gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/cpu_instrs.gb");
    
    // Todo
    
    
    //p4boy.LoadRom("Roms/Tetris.gb");
    p4boy.LoadRom("Roms/links_awakening.gb");
    //p4boy.LoadRom("Roms/pokemon_yellow.gb");
    //p4boy.LoadRom("Roms/Mario's_Picross(FR).gb");
    

    std::thread p4BoyLoop(std::bind(P4Boy_Loop, p4boy));
    sf::RenderWindow window(sf::VideoMode(640, 480), "P4Boy - Tools");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;
    ShowCPURegisters showCPURegisters(p4boy.GetCPU());
    ShowMemory showMemory(p4boy.GetMainBus());
    ShowCartridge showCartridge(p4boy.GetCartridge());

    bool showDemo = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }



        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
        
        // Menu
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("P4Boy"))
            {
                ImGui::MenuItem("Load ROM");
                ImGui::EndMenu();
            }

            showCPURegisters.MenuToolBarUpdate();
            showMemory.MenuToolBarUpdate();
            showCartridge.MenuToolBarUpdate();

            if (ImGui::BeginMenu("Demo"))
            {
                if (ImGui::MenuItem("Show", nullptr, showDemo)) showDemo = !showDemo;
                    
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        if (showDemo) ImGui::ShowDemoWindow();
        showCPURegisters.WindowUpdate();
        showMemory.WindowUpdate();
        showCartridge.WindowUpdate();

        ImGui::End();
        
        
        
        ImGui::PopStyleVar(1);

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

