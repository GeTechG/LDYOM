#pragma once

#include <d3d9.h>
#include <functional>

#include "imgui.h"

using f_EndScene = HRESULT(CALLBACK *)(IDirect3DDevice9 *);
using f_Reset = HRESULT(CALLBACK *)(IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


class ImGuiHook {
private:
	static inline WNDPROC oWndProc;
	static inline f_EndScene oEndScene;
	static inline f_Reset oReset;
	static inline bool m_bMouseVisibility;

	static void CALLBACK RenderFrame(void *ptr);
	static HRESULT CALLBACK Dx9Handler(IDirect3DDevice9 *pDevice);
	static HRESULT CALLBACK Reset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void ShowMouse(bool state);

public:
	static inline bool m_bShowMouse = false;
	static inline float scaleUi = 1.0f;
	static inline std::function<void()> windowCallback = nullptr;
	static inline std::function<void()> preRenderCallback = nullptr;

	static void ApplyMouseFix();
	static ImVec2 Rescale();

	static void Hook();

	ImGuiHook() = default;
	~ImGuiHook();
};
