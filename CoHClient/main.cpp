#include "coh.h"
#include <cstdint>
#include <stdio.h>

#include <Windows.h>

#include <imgui.h>
#include <imgui_impl_win32.h>

CityOfHeroes CoH;

//
// Initializes the client.
//
static void init()
{
    CoH.game_base = (char*)GetModuleHandleA(NULL);
    InitializeCriticalSection(&CoH.sync_sec);

    init_hooking();
    install_hooks();
}

BOOL APIENTRY DllMain(HMODULE mod, DWORD reason_for_call, LPVOID reserved)
{
    if (reason_for_call == DLL_PROCESS_ATTACH)
    {
        init();
    }

    return TRUE;
}

