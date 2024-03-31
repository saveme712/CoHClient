#include "coh.h"
#include <imgui_impl_opengl2.h>
#include <imgui_impl_win32.h>

#include <glm/glm.hpp>

LRESULT CALLBACK hk_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_KEYDOWN)
	{
		if (wparam == VK_INSERT)
		{
			CoH.menu_open = !CoH.menu_open;
		}
	}

	if (CoH.menu_open)
	{
		auto result = ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
		if (result != 2)
		{
			return 0;
		}
	}

	return CoH.orig_wndproc(hwnd, msg, wparam, lparam);
}

void init_opengl2(HDC hdc)
{
	if (CoH.opengl_initialized)
	{
		return;
	}

	CoH.hwnd = WindowFromDC(hdc);
	CoH.orig_wndproc = (decltype(CoH.orig_wndproc))SetWindowLongPtrW(CoH.hwnd, GWLP_WNDPROC, (LONG_PTR)hk_wndproc);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(CoH.hwnd);
	ImGui_ImplOpenGL2_Init();

	offsetof(RdrViewState, gfxdebug);

	CoH.opengl_initialized = TRUE;
}

static void render_coh_menu()
{

	if (ImGui::Begin("CoH Client"))
	{
		if (ImGui::BeginTabBar("Main_Tabs"))
		{
			if (ImGui::BeginTabItem("Options"))
			{
				ImGui::Checkbox("Fuzzing", &CoH.run_fuzzing);
				ImGui::SliderInt("Fuzz Packets", &CoH.fuzz_packets, 1, 100);
				ImGui::SliderInt("Fuzzing Max Opcode", &CoH.fuzz_max_opcode, 1, 1200);
				ImGui::SliderInt("Fuzzing Max Iterations", &CoH.fuzz_max_iterations, 1, 100);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Logs"))
			{
				for (auto& msg : CoH.logs)
				{
					ImGui::Text(msg.c_str());
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	EnterCriticalSection(&CoH.sync_sec);
	for (auto& ent : CoH.cached_entities)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(ent.screen_pos.x, ent.screen_pos.y), 0xff0000ff, ent.name);
	}
	LeaveCriticalSection(&CoH.sync_sec);
}

static void render_opengl2(HDC hDc)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (CoH.menu_open)
	{
		render_coh_menu();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

BOOL hk_swap_buffers(HDC hdc)
{
	init_opengl2(hdc);
	render_opengl2(hdc);

	return CoH.orig_swap_buffers(hdc);
}