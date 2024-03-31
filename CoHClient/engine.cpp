#include "coh.h"
#include <imgui.h>

static void update_cached_entities()
{
	CoH.cached_entities.clear();
	for (auto i = 0; i < MAX_ENTITIES; i++)
	{
		if (auto ent = int_get_ent_xlat()[i])
		{
			glm::vec2 pix_ul, pix_lr;

			if (ent->seq && int_seq_get_sides(ent->seq, &ent->fork, &pix_ul, &pix_lr))
			{
				CachedEntity ce;
				strcpy_s(ce.name, ent->name);
				ce.screen_pos = glm::vec2(pix_ul[0], ImGui::GetIO().DisplaySize.y - pix_ul[1]);
				CoH.cached_entities.push_back(ce);
			}
		}
	}
}

void hk_engine_update()
{
	EnterCriticalSection(&CoH.sync_sec);
	update_cached_entities();
	LeaveCriticalSection(&CoH.sync_sec);

	return CoH.orig_engine_update();
}

void hk_comm_send_input()
{
	if (CoH.run_fuzzing)
	{
		CoH.log("Fuzzing");

		for (auto k = 0; k < 8; k++)
		{
			auto pak = int_get_client_input_pak();
			auto num_iterations = (rand() % 4);

			int_pkt_send_bits_pack(pak, 1, rand() % 700);
			for (auto i = 0; i < num_iterations; i++)
			{
				int_pkt_send_bits_pack(pak, 1, rand());
			}
		}
	}

	return CoH.orig_comm_send_input();
}