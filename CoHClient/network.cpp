#include "coh.h"
#include <intrin.h>

uint64_t hk_pkt_send_string(char* file, uint64_t unk_1, void* pkt, const char* str)
{
    return CoH.orig_pkt_send_string(file, unk_1, pkt, str);
}


uint64_t hk_pkt_send(void** packet, void* net_link)
{
    auto ret_offset = (char*)_ReturnAddress() - CoH.game_base;
    char fmt[256];
    sprintf_s(fmt, "[" __FUNCTION__ "] Packet: %p, NetLink: %p, Ret: 0x%llx", packet, net_link, ret_offset);
    CoH.log(fmt);

    return int_pkt_send(packet, net_link);
}