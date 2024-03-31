#pragma once
#include <cstdint>
#include <d3d9.h>
#include <gl/GL.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "reverse.h"

// client_input_pak
//
#define OFF_SWAP_BUFFERS 0x6FD138
//
#define OFF_ENT_XLAT 0x1112260
//
#define OFF_DB_COMM_LINK 0x1588CA0
//
#define OFF_CLIENT_INPUT_PAK 0x158C470
// .
#define OFF_PKT_CREATE_EX 0x6b2750
// .
#define OFF_PKT_SEND_STRING 0x671ea0
// .
#define OFF_PKT_SEND_BITS 0x671c40
// .
#define OFF_PKT_SEND_BITS_PACK 0x671d70
// .
#define OFF_PKT_SEND 0x6afed0
// .
#define OFF_ENGINE_UPDATE 0x1c59f0
// .
#define OFF_SEQ_GET_SIDES 0x2183b0
// .
#define OFF_AUCTION_ADD_ITEM 0x46aab0
// .
#define OFF_COMM_SEND_INPUT 0x109c50

/*
#define OFF_SWAP_BUFFERS 0x6FC138

#define OFF_ENT_XLAT 0x1110240

#define OFF_DB_COMM_LINK 0x1586C60
#define OFF_CLIENT_INPUT_PAK 0x158A430

#define OFF_PKT_CREATE_EX 0x6B22E0
#define OFF_PKT_SEND_STRING 0x671A30
#define OFF_PKT_SEND_BITS 0x6717D0
#define OFF_PKT_SEND_BITS_PACK 0x671900
#define OFF_PKT_SEND 0x6AFA60

#define OFF_ENGINE_UPDATE 0x1C59F0

#define OFF_SEQ_GET_SIDES 0x2197A0

#define OFF_AUCTION_ADD_ITEM 0x46A640

#define OFF_COMM_SEND_INPUT 0x109C50
*/
typedef LRESULT(*fn_wndproc)(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
typedef BOOL(*fn_swap_buffers)(HDC hdc);

typedef void* (*fn_pkt_create_ex)(void* net_link, int32_t opcode);
typedef uint64_t(*fn_pkt_send_string)(const char* file, uint64_t unk_1, void* pkt, const char* str);
typedef uint64_t(*fn_pkt_send_bits)(const char* file, uint64_t unk_1, void* pkt, uint32_t bits, uint32_t v);
typedef uint64_t(*fn_pkt_send_bits_pack)(const char* file, uint64_t unk_1, void* pkt, uint32_t bits, uint32_t v);
typedef uint64_t(*fn_pkt_send)(void** pkt, void* net_link);
typedef void (*fn_engine_update)();
typedef BOOLEAN(*fn_seq_get_sides)(void* seq, void* fork, glm::vec2* pix_ul, glm::vec2* pix_lr, void*, void*);
typedef void(*fn_auction_add_item)(const char* pch_ident, int index, int amt, int price, int status, int id);
typedef void(*fn_comm_send_input)();

struct CachedEntity
{
	char name[256];
	glm::vec2 screen_pos;
};

struct CityOfHeroes
{
	char* game_base = NULL;

	fn_wndproc orig_wndproc = NULL;
	fn_swap_buffers orig_swap_buffers = NULL;

	fn_pkt_send_string orig_pkt_send_string = NULL;
	fn_pkt_send orig_pkt_send = NULL;

	fn_engine_update orig_engine_update = NULL;

	fn_comm_send_input orig_comm_send_input = NULL;

	CRITICAL_SECTION sync_sec;
	std::vector<CachedEntity> cached_entities;

	BOOLEAN menu_open = FALSE;

	BOOLEAN opengl_initialized = FALSE;
	HWND hwnd;

	std::vector<std::string> logs;

	bool run_fuzzing = FALSE;
	int fuzz_packets = 8;
	int fuzz_max_opcode = 400;
	int fuzz_max_iterations = 4;

	__forceinline void log(std::string msg)
	{
		logs.push_back(msg);
	}
};

extern CityOfHeroes CoH;

//
// Hooking.
//
void create_and_enable_inline_hook(void* target, void* detour, void* orig);
void create_and_enable_ptr_hook(void* target, void* detour, void* orig);
void create_and_enable_patch(void* target, void* patch, size_t patch_size);
void install_hooks();
void init_hooking();

//
// Hook handlers.
//
LRESULT CALLBACK hk_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL hk_swap_buffers(HDC hdc);

uint64_t hk_pkt_send_string(char* file, uint64_t unk_1, void* pkt, const char* str);
uint64_t hk_pkt_send(void** packet, void* net_link);

void hk_engine_update();
void hk_comm_send_input();

//
// Game interopability.
//
Entity** int_get_ent_xlat();
void* int_get_db_comm_link();
void* int_get_client_input_pak();
void* int_pkt_create_ex(void* net_link, int32_t id);
uint64_t int_pkt_send_string(void* packet, const char* string);
uint64_t int_pkt_send_bits(void* packet, uint32_t bits, uint32_t value);
uint64_t int_pkt_send_bits_pack(void* packet, uint32_t bits, uint32_t value);
uint64_t int_pkt_send(void** packet, void* net_link);
BOOLEAN int_seq_get_sides(void* seq, void* fork, glm::vec2* pix_ul, glm::vec2* pix_lr);
void int_auction_add_item(const char* pch_ident, int index, int amt, int price, int status, int id);