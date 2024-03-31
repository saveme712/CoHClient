#include "coh.h"

Entity** int_get_ent_xlat()
{
	return (Entity**)(CoH.game_base + OFF_ENT_XLAT);
}

void* int_get_db_comm_link()
{
	return CoH.game_base + OFF_DB_COMM_LINK;
}

void* int_get_client_input_pak()
{
	return *((void**)(CoH.game_base + OFF_CLIENT_INPUT_PAK));
}

void* int_pkt_create_ex(void* net_link, int32_t id)
{
	return ((fn_pkt_create_ex)(CoH.game_base + OFF_PKT_CREATE_EX))(net_link, id);
}

uint64_t int_pkt_send_string(void* packet, const char* string)
{
	return CoH.orig_pkt_send_string("", 0, packet, string);
}

uint64_t int_pkt_send_bits(void* packet, uint32_t bits, uint32_t value)
{
	return ((fn_pkt_send_bits)(CoH.game_base + OFF_PKT_SEND_BITS))("", 0, packet, bits, value);
}

uint64_t int_pkt_send_bits_pack(void* packet, uint32_t bits, uint32_t value)
{
	return ((fn_pkt_send_bits_pack)(CoH.game_base + OFF_PKT_SEND_BITS_PACK))("", 0, packet, bits, value);
}

uint64_t int_pkt_send(void** packet, void* net_link)
{
	return CoH.orig_pkt_send(packet, net_link);
}

BOOLEAN int_seq_get_sides(void* seq, void* fork, glm::vec2* pix_ul, glm::vec2* pix_lr)
{
	return ((fn_seq_get_sides)(CoH.game_base + OFF_SEQ_GET_SIDES))(seq, fork, pix_ul, pix_lr, NULL, NULL);
}

void int_auction_add_item(const char* pch_ident, int index, int amt, int price, int status, int id)
{
	return ((fn_auction_add_item)(CoH.game_base + OFF_AUCTION_ADD_ITEM))(pch_ident, index, amt, price, status, id);
}