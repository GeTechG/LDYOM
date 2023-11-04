#include "ImGuiHook.h"

#include <CMenuManager.h>

#include "fa.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "kiero.h"
#include "plugin.h"

using namespace plugin;

extern unsigned int fa_compressed_size;
extern unsigned int fa_compressed_data[];
extern unsigned int firaSans_compressed_data_base85_size;
extern char firaSans_compressed_data_base85[];

bool isInitImgui = false;

LRESULT ImGuiHook::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	if (ImGui::GetIO().WantTextInput) {
		Call<0x53F1E0>(); // CPad::ClearKeyboardHistory
		return 1;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT ImGuiHook::Reset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters) {
	ImGui_ImplDX9_InvalidateDeviceObjects();

	return oReset(pDevice, pPresentationParameters);
}

void ImGuiHook::RenderFrame(void *ptr) {
	if (!ImGui::GetCurrentContext()) {
		return;
	}

	ImGuiIO &io = ImGui::GetIO();

	if (preRenderCallback != nullptr) {
		preRenderCallback();
	}

	if (isInitImgui) {
		ShowMouse(m_bShowMouse);

		// handle window scaling here
		static auto fScreenSize = ImVec2(-1, -1);
		const ImVec2 size(screen::GetScreenWidth(), screen::GetScreenHeight());
		if (fScreenSize.x != size.x && fScreenSize.y != size.y) {
			const int fontSize = static_cast<int>(size.y / 70.85f); // manually tested

			const auto allocate_fira_regular = new char[firaSans_compressed_data_base85_size];
			memcpy(allocate_fira_regular, firaSans_compressed_data_base85, firaSans_compressed_data_base85_size);

			static constexpr ImWchar lang_ranges[] = {0x0020, 0xFFFF, 0};
			ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(allocate_fira_regular,
			                                                           static_cast<float>(fontSize), nullptr,
			                                                           lang_ranges);

			const auto allocate_fa = new unsigned int[fa_compressed_size / 4];
			memcpy(allocate_fa, fa_compressed_data, fa_compressed_size);

			static constexpr ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
			ImFontConfig icons_config;
			icons_config.MergeMode = true;
			ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(allocate_fa, static_cast<int>(fa_compressed_size),
			                                                     static_cast<float>(fontSize), &icons_config,
			                                                     icons_ranges);

			ImGui::GetIO().Fonts->Build();

			ImGui_ImplDX9_InvalidateDeviceObjects();

			ImGuiStyle *style = &ImGui::GetStyle();
			const float scaleX = size.x / 1366.0f;
			const float scaleY = size.y / 768.0f;

			style->FramePadding = ImVec2(5 * scaleX, 3 * scaleY);
			style->ItemSpacing = ImVec2(8 * scaleX, 4 * scaleY);
			style->ScrollbarSize = 12 * scaleX;
			style->IndentSpacing = 20 * scaleX;
			style->ItemInnerSpacing = ImVec2(4 * scaleX, 4 * scaleY);

			fScreenSize = size;
		}

		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX9_NewFrame();

		ImGui::NewFrame();

		if (windowCallback != nullptr) {
			windowCallback();
		}

		ImGui::EndFrame();
		ImGui::Render();

		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	} else {
		isInitImgui = true;
		ImGui::CreateContext();

		ImGuiStyle &style = ImGui::GetStyle();

		ImGui_ImplWin32_Init(RsGlobal.ps->window);


		// shift trigger fix
		patch::Nop(0x00531155, 5);

		ImGui_ImplDX9_Init(static_cast<IDirect3DDevice9*>(ptr));

		ImGui_ImplWin32_EnableDpiAwareness();

		io.IniFilename = nullptr;
		io.LogFilename = nullptr;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		style.WindowTitleAlign = ImVec2(0.5, 0.5);
		oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(RsGlobal.ps->window, GWL_WNDPROC,
		                                                      reinterpret_cast<LRESULT>(WndProc)));
	}
}

HRESULT ImGuiHook::Dx9Handler(IDirect3DDevice9 *pDevice) {
	if (!FrontEndMenuManager.m_bMenuActive) {
		RenderFrame(pDevice);
	}
	return oEndScene(pDevice);
}

void ImGuiHook::ShowMouse(bool state) {
	if (m_bMouseVisibility != m_bShowMouse) {
		ImGui::GetIO().MouseDrawCursor = state;

		ApplyMouseFix(); // Reapply the patches

		if (m_bShowMouse) {
			patch::SetUChar(0x6020A0, 0xC3); // psSetMousePos
			patch::Nop(0x4AB6CA, 5); // don't call CPad::UpdateMouse()
		} else {
			patch::SetUChar(0x6020A0, 0x53);
			patch::SetRaw(0x4AB6CA, const_cast<char*>("\xE8\x51\x21\x00\x00"), 5);
		}

		CPad::NewMouseControllerState.x = 0;
		CPad::NewMouseControllerState.y = 0;
		CPad::ClearMouseHistory();
		CPad::UpdatePads();
		m_bMouseVisibility = m_bShowMouse;
	}
}

struct Mouse {
	unsigned int x, y;
	unsigned int wheelDelta;
	unsigned char buttons[8];
};

struct MouseInfo {
	int x, y, wheelDelta;
} mouseInfo;

static BOOL __stdcall _SetCursorPos(int X, int Y) {
	if (ImGuiHook::m_bShowMouse || GetActiveWindow() != RsGlobal.ps->window) {
		return 1;
	}

	mouseInfo.x = X;
	mouseInfo.y = Y;

	return SetCursorPos(X, Y);
}

static LRESULT __stdcall _DispatchMessage(MSG *lpMsg) {
	if (lpMsg->message == WM_MOUSEWHEEL && !ImGuiHook::m_bShowMouse) {
		mouseInfo.wheelDelta += GET_WHEEL_DELTA_WPARAM(lpMsg->wParam);
	}

	return DispatchMessageA(lpMsg);
}

static int _cdecl _GetMouseState(Mouse *pMouse) {
	if (ImGuiHook::m_bShowMouse) {
		return -1;
	}

	tagPOINT Point;

	pMouse->x = 0;
	pMouse->y = 0;
	pMouse->wheelDelta = mouseInfo.wheelDelta;
	GetCursorPos(&Point);

	if (mouseInfo.x >= 0) {
		pMouse->x = static_cast<int>(1.6f * (Point.x - mouseInfo.x)); // hacky fix
	}

	if (mouseInfo.y >= 0) {
		pMouse->y = static_cast<int>(Point.y - mouseInfo.y);
	}

	mouseInfo.wheelDelta = 0;

	pMouse->buttons[0] = (GetAsyncKeyState(1) >> 8);
	pMouse->buttons[1] = (GetAsyncKeyState(2) >> 8);
	pMouse->buttons[2] = (GetAsyncKeyState(4) >> 8);
	pMouse->buttons[3] = (GetAsyncKeyState(5) >> 8);
	pMouse->buttons[4] = (GetAsyncKeyState(6) >> 8);
	return 0;
}

void ImGuiHook::ApplyMouseFix() {
	patch::ReplaceFunctionCall(0x53F417, _GetMouseState);
	patch::Nop(0x57C59B, 1);
	patch::ReplaceFunctionCall(0x57C59C, _SetCursorPos);
	patch::Nop(0x81E5D4, 1);
	patch::ReplaceFunctionCall(0x81E5D5, _SetCursorPos);
	patch::Nop(0x74542D, 1);
	patch::ReplaceFunctionCall(0x74542E, _SetCursorPos);
	patch::Nop(0x748A7C, 1);
	patch::ReplaceFunctionCall(0x748A7D, _DispatchMessage);
	patch::SetChar(0x746A08, 32); // diMouseOffset
	patch::SetChar(0x746A58, 32); // diDeviceoffset
}

void ImGuiHook::Hook() {
	ImGui::CreateContext();

	// Nvidia Overlay crash fix
	if (init(kiero::RenderType::D3D9) == kiero::Status::Success) {
		kiero::bind(16, reinterpret_cast<void**>(&oReset), Reset);
		kiero::bind(42, reinterpret_cast<void**>(&oEndScene), Dx9Handler);
	}
}

ImGuiHook::~ImGuiHook() {
	SetWindowLongPtr(RsGlobal.ps->window, GWL_WNDPROC, reinterpret_cast<LRESULT>(oWndProc));
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	kiero::shutdown();
}
