#pragma once

#include "d3d9.h"
#include "d3dx9.h"

#include "libs/fa.h"
#include "GUI.h"
#include "imnodes.h"
#include "NodeGraph.h"

extern bool mission_started;
extern bool storyline_started;

VMTHookManager *vmtHooks;

typedef HRESULT(WINAPI* _Present)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
_Present oPresent; 

typedef HRESULT(WINAPI* _Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
_Reset oReset;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static bool openFrame = false;

bool demo = true;
bool init_ImGui = false;
bool load_theme = false;
bool off_gui = false;
bool draw_lines = false;
HRESULT WINAPI Present(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hdest, const RGNDATA* pDirtyRegion)
{
	
	if (!init_ImGui)
	{
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		ImGui_ImplWin32_Init(GetActiveWindow());
		ImGui_ImplDX9_Init(pDevice);

		ImNodes::CreateContext();
		ImNodes::SetNodeGridSpacePos(1, ImVec2(200.0f, 200.0f));

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows


		io.ConfigWindowsMoveFromTitleBarOnly = true;

		static const ImWchar lang_ranges[] = { 0x0020,0xFFFF,0 };
		io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 14.0f, NULL, &lang_ranges[0]);
		
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		static const ImWchar fa_ranges[] = { ICON_MIN_FA,ICON_MAX_FA,0 };
		io.Fonts->AddFontFromMemoryCompressedTTF(fa_data, fa_data_compressed_size, 14.0f, &icons_config, &fa_ranges[0]);

		IM_ASSERT(LoadTextureFromResource(IDB_PNG1, &weaponsAtlas, nullptr, nullptr));
		IM_ASSERT(LoadTextureFromResource(IDB_PNG2, &pedsSkinAtlas, nullptr, nullptr));
		IM_ASSERT(LoadTextureFromResource(IDB_PNG3, &blipsAtlas, nullptr, nullptr));

		init_ImGui = true;
	}
	else
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		if (memsafe::read<int>((void*)0xBA67A4) == 0) {
			if (openFrame && !mission_started && !storyline_started && !off_gui) {
				if (bMainMenu) { fMainMenu(); }
				if (bTargets) { fTargets(); }
				if (bActors) { fActors(); }
				if (bCars) { fCars(); }
				if (bTrains) { fTrains(); }
				if (carSelector::bShow) { carSelector::fShow(); }
				if (bGroupRelations) { fGroupRelations(); }
				if (bObjects) { fObjects(); }
				if (bParticles) { fParticles(); }
				if (bPickups) { fPickups(); }
				if (bExplosions) { fExplosions(); }
				if (bAudios) { fAudios(); }
				if (bMissionPacks) { fMissionPacks(); }
				if (bPlayer) { fPlayer(); }
				if (bMissionSettings) { fMissionSettings(); }
				if (bStorylines) { fStorylineList(); }
				if (bTools) { fTools(); }
				if (bSettings) { fSettings(); }
				if (bInfo) { fInfo(); }
				if (bStorylineMainMenu) { fStorylineMainMenu(); }
				if (bStorylineCheckpoints) { fStorylineCheckpoints(); }
				if (bScriptsSettings) { fScriptsSettings(); }
				if (bNodeEditor) { NodeGraph::render(); }
				//ImGui::ShowDemoWindow(&openFrame);
			}
			if (draw_lines && editmodeCamera)
			{
				ImVec2& screenRes = ImGui::GetIO().DisplaySize;
				for (float i = 1.0f; i < 3.0f; i++)
				{
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(0.0f, (screenRes.y / 3.0f) * i), ImVec2(screenRes.x, (screenRes.y / 3.0f) * i), ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2((screenRes.x / 3.0f) * i, 0.0f), ImVec2((screenRes.x / 3) * i, screenRes.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
				}
			}
		}
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	if (!load_theme) {
		std::string lang;
		lang.append("LDYOM/Themes/");
		lang.append(curr_theme_string);
		lang.append(".toml");
		ImGui::StyleLoader::LoadFile(lang);
		load_theme = true;
	}
	
	return oPresent(pDevice, pSourceRect, pDestRect, hdest, pDirtyRegion);
}

HRESULT WINAPI Reset(IDirect3DDevice9* m_pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	return oReset(m_pDevice, pPresentationParameters);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool pCursor = false;

	if (KeyJustPressed(VK_U) && !ImGui::GetIO().WantTextInput) {
		if (openFrame) {
			bMainMenu = false;
			bTargets = false;
			bActors = false;
			bCars = false;
			bTrains = false;
			bGroupRelations = false;
			bObjects = false;
			bParticles = false;
			bPickups = false;
			bExplosions = false;
			bAudios = false;
			bMissionPacks = false;
			bPlayer = false;
			bMissionSettings = false;
			bStorylines = false;
			bTools = false;
			bSettings = false;
			bInfo = false;
			bStorylineMainMenu = false;
			bStorylineCheckpoints = false;
			bScriptsSettings = false;
			bNodeEditor = false;
			if (!mission_started)
			{
				Command<Commands::FREEZE_CHAR_POSITION>(playerPed, false);
				TheCamera.Restore();
			}
		} else {
			if (!storylineMode)
				bMainMenu = true;
			else
				bStorylineMainMenu = true;
			Command<0x0AB3>(700, 1);

			resolution.x = plugin::screen::GetScreenWidth();
			resolution.y = plugin::screen::GetScreenHeight();
		}
	}

	openFrame = bMainMenu || bTargets || bActors || bCars || bTrains || carSelector::bShow || bGroupRelations || bObjects || bParticles || bPickups || bExplosions || bAudios || bMissionPacks || bPlayer || bMissionSettings || bStorylines || bTools || bSettings || bInfo || bStorylineMainMenu || bStorylineCheckpoints || bScriptsSettings || bNodeEditor;

	if (openFrame && !mission_started && !storyline_started && !off_gui) {
		Command<0x0AB3>(700, 1);
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	} else
		Command<0x0AB3>(700, 0);

	return CallWindowProcA((WNDPROC)prevWndProc, hWnd, uMsg, wParam, lParam);
}

void InitHooks()
{
	void** vTableDevice = *(void***)(*(DWORD*)0xC97C28); 
	vmtHooks = new VMTHookManager(vTableDevice);
	oPresent = (_Present)vmtHooks->Hook(17, (void*)Present);
	oReset = (_Reset)vmtHooks->Hook(16, (void*)Reset);
	prevWndProc = SetWindowLongA(FindWindowA(NULL, "GTA: San Andreas"), GWL_WNDPROC, (LONG)WindowProc);
}

void DeleteHooks()
{
	ImNodes::DestroyContext();
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	vmtHooks->UnhookAll();
}