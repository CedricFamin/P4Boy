// P4Boy.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "P4Boy.h"
#include <chrono>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <sstream>
#include <iomanip>

#include <thread>

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

class ShowCPURegisters
{
public:

    ShowCPURegisters(P4Boy::CPU::shared_ptr cpu) : _cpu(cpu) {}

    void MenuToolBarUpdate()
    {
        if (ImGui::BeginMenu("CPU"))
        {
            if (ImGui::MenuItem("Registers", nullptr, _show))
            {
                _show = !_show;
            }
            if (ImGui::MenuItem("Logs"))
            {
                _cpu->debugShowInstructions = !_cpu->debugShowInstructions;
            }
            ImGui::EndMenu();
        }
    }

    void WindowUpdate()
    {
        if (_show)
        {
            ImGui::Begin("CPU Registers");
            ImGui::Button("CPU");
            ImGui::End();
        }
    }
private:
    P4Boy::CPU::shared_ptr _cpu;
    bool _show = false;
};

class ShowMemory
{
public:

    ShowMemory(P4Boy::MainBus& mainBus) : _mainBus(mainBus) {}

    void MenuToolBarUpdate()
    {

        if (ImGui::BeginMenu("Memory"))
        {
            if (ImGui::MenuItem("Show Memory", nullptr, _show))
            {
                _show = !_show;
            }
            ImGui::EndMenu();
        }
    }

    void UpdateMemoryContent(std::shared_ptr<P4Boy::AddressRange> range)
    {
        _memoryContent.clear();
        if (!range)
        {
            _currentRange= "";
           
        }
        std::stringstream ss;
        uint8_t nbHex = 0;
        uint32_t addr = range->From();
        for (; addr <= range->To(); ++addr)
        {
            if (nbHex % 16 == 0)
            {
                ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << addr << ": ";
            }
            ss << std::setfill('0') << std::setw(2) << std::hex << uint16_t(_mainBus.Get_8b(addr)) << " ";
            ++nbHex;

            if (nbHex % 16 == 0)
            {
                _memoryContent.push_back(ss.str());
                ss.str("");
            }
        }
        _memoryContent.push_back(ss.str());

        std::stringstream currentRange;
        currentRange << "" << std::setfill('0') << std::setw(4) << std::hex << range->From() << " - ";
        currentRange << "" << std::setfill('0') << std::setw(4) << std::hex << range->To() << " : ";
        currentRange << range->Name();
        _currentRange = currentRange.str();
    }
    void WindowUpdate()
    {
        if (_show)
        {
            ImGui::Begin("Show Memory");

            
            if (ImGui::BeginCombo("Address type", _currentRange.c_str()))
            {
                for (auto const& range : _mainBus.Ranges())
                {
                    std::stringstream ss;
                    ss << "" << std::setfill('0') << std::setw(4) << std::hex << range->From() << " - ";
                    ss << "" << std::setfill('0') << std::setw(4) << std::hex << range->To() << " : ";
                    ss << range->Name();

                    if (ImGui::Selectable(ss.str().c_str()))
                    {
                        UpdateMemoryContent(range);
                    }
                }
                ImGui::EndCombo();
            }
            
            for (auto& line : _memoryContent)
                ImGui::Text(line.c_str());
                

            ImGui::End();
        }
    }
private:
    P4Boy::MainBus& _mainBus;
    std::vector<std::string> _memoryContent;
    std::string _currentRange;
    bool _show = false;
};


int main()
{
   
    auto p4boy = P4Boy::P4Boy();
    std::thread p4BoyLoop(std::bind(P4Boy_Loop, p4boy));

    // Passed
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/01-special.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/03-op sp,hl.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/04-op r,imm.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/05-op rp.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/06-ld r,r.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/08-misc instrs.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/09-op r,r.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/10-bit ops.gb");
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/11-op a,(hl).gb");

    // Todo
    // p4boy.LoadRom("Roms/gb-test-roms-master/cpu_instrs/individual/02-interrupts.gb");
    


    // p4boy.LoadRom("Roms/Tetris.gb");

    sf::RenderWindow window(sf::VideoMode(640, 480), "P4Boy - Tools");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;
    ShowCPURegisters showCPURegisters(p4boy.GetCPU());
    ShowMemory showMemory(p4boy.GetMainBus());

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

        ImGui::End();
        
        
        
        ImGui::PopStyleVar(1);

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

