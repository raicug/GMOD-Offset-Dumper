#include "sdk/interfaces.h"
#include "sdk/utils/netvars.h"

#include <iostream>
#include <windows.h>
#include <thread>
#include <fstream>
#include <chrono>

#ifdef BUILDING_DLL
    #define DLLEXPORT __declspec(dllexport)
#else
    #define DLLEXPORT __declspec(dllimport)
#endif

void attach_console() {
    FreeConsole();

    if (!AllocConsole()) {
        MessageBoxA(nullptr, "Failed to allocate console.", "Error", MB_ICONERROR);
        return;
    }

    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$",  "r", stdin);

    SetConsoleTitleA("GMod NetVar Dumper");

    std::ios::sync_with_stdio(true);

    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    std::cout << "[+] Console initialized!\n";
}

void uninject_dll(HMODULE hModule) {
    std::cout << "[INFO] Uninjecting DLL...\n";
    FreeLibraryAndExitThread(hModule, 0);
}

void entry_thread(HMODULE hModule) {
    attach_console();

    std::cout << "[INFO] DLL injected, waiting for game to initialize...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "[INFO] Initializing interfaces...\n";
    interfaces::init_interfaces();

    std::cout << "[INFO] Initializing netvar manager...\n";
    netvar_manager::init_netvar_manager();

    std::cout << "[INFO] Dump complete. Output also saved to netvars_dump.txt\n";
    netvar_manager::dump_netvars_to_file("C:\\R-GMOD\\Dumps");

    MessageBoxA(NULL, "dump complete", "GMod NetVar Dumper", MB_ICONINFORMATION);

    std::cout << "[INFO] Press any key to close console...\n";
    std::cin.get();

    FreeConsole(); // Clean up if desired

    uninject_dll(hModule);
}

extern "C" {
    DLLEXPORT BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
        if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
            DisableThreadLibraryCalls(hModule);

            MessageBoxA(NULL, "GMod NetVar Dumper injected", "GMod NetVar Dumper", MB_ICONINFORMATION);

            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)entry_thread, nullptr, 0, nullptr);
        }
        return TRUE;
    }
}