
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
#include "ClockDebug.h"
#include "LCDDebug.h"

#include <windows.h>
#include <shobjidl.h> 

std::string OpenFile()
{
    std::string filePath = "";
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        std::wstring wfilePath = pszFilePath;
                        filePath = std::string(wfilePath.begin(), wfilePath.end()); // not sure about this but it works :)
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return filePath;
}

void P4Boy_Loop(P4Boy::P4Boy & p4boy)
{
    unsigned int tick = 0;
    auto on_date = std::chrono::high_resolution_clock::now();

    while (true)
    {
        p4boy.Tick();
        ++tick;
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
    ClockDebug showClock(p4boy.GetClock());
    LCDDebug showLCD(p4boy.GetLCD());

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
                if (ImGui::MenuItem("Load ROM"))
                {
                    std::string fileToOpen = OpenFile();
                    if (fileToOpen != "")
                    {
                        p4boy.Stop();
                        p4boy.LoadRom(fileToOpen.c_str());
                        p4boy.Resume();
                    }
                }
                if (ImGui::MenuItem("Reset"))
                {
                    p4boy.Stop();
                    p4boy.Reset();
                    p4boy.Resume();
                }
                ImGui::EndMenu();
            }

            showClock.MenuToolBarUpdate();
            showCPURegisters.MenuToolBarUpdate();
            showMemory.MenuToolBarUpdate();
            showCartridge.MenuToolBarUpdate();
            showLCD.MenuToolBarUpdate();

            if (ImGui::BeginMenu("Demo"))
            {
                if (ImGui::MenuItem("Show", nullptr, showDemo)) showDemo = !showDemo;
                    
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        if (showDemo) ImGui::ShowDemoWindow();
        showClock.WindowUpdate();
        showCPURegisters.WindowUpdate();
        showMemory.WindowUpdate();
        showCartridge.WindowUpdate();
        showLCD.WindowUpdate();

        ImGui::End();
        
        
        
        ImGui::PopStyleVar(1);

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

