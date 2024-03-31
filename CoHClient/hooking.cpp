#include "coh.h"
#include <MinHook.h>

//
// Creates and enables a new hook using minhook.
//
void create_and_enable_inline_hook(void* target, void* detour, void* orig)
{
    if (MH_CreateHook(target, detour, (void**)orig) != MH_OK)
    {
        return;
    }

    if (MH_EnableHook(target) != MH_OK)
    {
        return;
    }
}

//
// Creates and enables a new hook using minhook.
//
void create_and_enable_ptr_hook(void* target, void* detour, void* orig)
{
    *((void**)orig) = *((void**)target);

    DWORD old_protect;
    VirtualProtect(target, sizeof(void*), PAGE_READWRITE, &old_protect);


    *((void**)target) = detour;
}

void create_and_enable_patch(void* target, void* patch, size_t patch_size)
{
    DWORD old_protect;
    VirtualProtect(target, patch_size, PAGE_EXECUTE_READWRITE, &old_protect);
     
    memcpy(target, patch, patch_size);
}

//
// Installs all hooks needed for the client to function.
//
void install_hooks()
{
    create_and_enable_ptr_hook(CoH.game_base + OFF_SWAP_BUFFERS, hk_swap_buffers, &CoH.orig_swap_buffers);
    create_and_enable_inline_hook(CoH.game_base + OFF_PKT_SEND_STRING, hk_pkt_send_string, &CoH.orig_pkt_send_string);
    create_and_enable_inline_hook(CoH.game_base + OFF_PKT_SEND, hk_pkt_send, &CoH.orig_pkt_send);
    create_and_enable_inline_hook(CoH.game_base + OFF_ENGINE_UPDATE, hk_engine_update, &CoH.orig_engine_update);
    create_and_enable_inline_hook(CoH.game_base + OFF_COMM_SEND_INPUT, hk_comm_send_input, &CoH.orig_comm_send_input);

    // send comm updates faster
    static uint8_t patch[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    create_and_enable_patch(CoH.game_base + 0x109CAF, patch, sizeof(patch));
    
    // don't generate 'hardware' ids
    static uint64_t hwid_patch_offsets[] = { 0x11160E };
    static uint8_t call_patch[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
    for (auto offset : hwid_patch_offsets)
    {
        create_and_enable_patch(CoH.game_base + offset, call_patch, sizeof(call_patch));
    }

    auto hwids = (int32_t*)(CoH.game_base + 0xC77F60);
    hwids[0] = rand();
    hwids[1] = rand();
    hwids[2] = rand();
    hwids[3] = rand();
    hwids[4] = rand();
    hwids[5] = rand();
    hwids[6] = rand();
    hwids[7] = rand();
    hwids[8] = rand();
}

void init_hooking()
{
    if (MH_Initialize() != MH_OK)
    {
        return;
    }
}