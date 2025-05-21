#include "imgui_hook.h"
#include "MinHook.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "injector/injector.hpp"
#include "kiero.h"
#include "pad.h"
#include "plugin.h"
#include "utils/imgui_configurate.h"
#include <CMenuManager.h>

auto gRenderer = eRenderer::Unknown;
void* gD3DDevice = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool ImguiHook::GetMouseState() { return mouseVisible; }

void ImguiHook::SetMouseState(bool state) { mouseVisible = state; }

void ImguiHook::SetControlEnabled(bool state) { controlEnabled = state; }

void ImguiHook::dirtyObjectsFlag() { dirtyObjects = true; }

// Timer ID for window restoration
static const UINT_PTR RESTORE_TIMER_ID = 1001;
static bool isRestoreTimerActive = false;

// Callback function for the timer
static void CALLBACK RestoreTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	if (idEvent == RESTORE_TIMER_ID) {
		// Stop the timer after it fires
		KillTimer(hwnd, RESTORE_TIMER_ID);
		isRestoreTimerActive = false;
	}
}

LRESULT ImguiHook::hkWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// Handle timer messages if restore timer is active
	if (isRestoreTimerActive && uMsg != WM_TIMER) {
		// Redirect messages to original window procedure during the timer period
		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}

	// Обработка системных сообщений для минимизации/восстановления окна
	if (uMsg == WM_SYSCOMMAND && ((wParam & 0xFFF0) == SC_MINIMIZE || (wParam & 0xFFF0) == SC_RESTORE)) {
		// For window restoration, set up a timer for 100ms
		if ((wParam & 0xFFF0) == SC_RESTORE) {
			SetTimer(hWnd, RESTORE_TIMER_ID, 100, RestoreTimerProc);
			isRestoreTimerActive = true;
		}

		// Даем возможность обработать сообщение оригинальному процессору окна
		LRESULT result = CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);

		return result;
	}

	if (m_bInitialized) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

		const auto disableControl = !controlEnabled && mouseVisible && wParam != VK_ESCAPE;
		if ((ImGui::GetIO().WantTextInput || disableControl) && !FrontEndMenuManager.m_bMenuActive) {
			plugin::Call<0x53F1E0>(); // CPad::ClearKeyboardHistory
			return 1;
		}
		if (disableControl && wParam == VK_HOME)
			return 1;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT ImguiHook::hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
	ImGui_ImplDX9_InvalidateDeviceObjects();

	return oReset(pDevice, pPresentationParameters);
}

void ImguiHook::ProcessFrame(void* ptr) {
	if (m_bInitialized) {
		ProcessMouse();

		if (ImGui::IsKeyPressed(ImGuiKey_RightAlt, false)) {
			ImGui::GetIO().AddKeyEvent(ImGuiKey_Escape, true);
		}

		// Scale the menu if game resolution changed
		static int height, width, RsGlobal;

#ifndef _WIN64
		RsGlobal = 0xC17040;
		width = injector::ReadMemory<int>(RsGlobal + 4, false);  // width
		height = injector::ReadMemory<int>(RsGlobal + 8, false); // height
#else
		RECT rect;
		GetWindowRect(hwnd, &rect);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
#endif

		static auto fScreenSize = ImVec2(-1, -1);
		if ((fScreenSize.x != width && fScreenSize.y != height) || dirtyObjects) {
			if (gRenderer == eRenderer::Dx9) {
				ImGui_ImplDX9_InvalidateDeviceObjects();
			} else if (gRenderer == eRenderer::Dx11) {
				ImGui_ImplDX11_InvalidateDeviceObjects();
			}
			ImGuiConfigurate::update(width, height);
			fScreenSize = ImVec2(static_cast<float>(width), static_cast<float>(height));
			dirtyObjects = false;
		}

		// ScriptExData::InitRenderStates();

		ImGui_ImplWin32_NewFrame();
		if (gRenderer == eRenderer::Dx9) {
			ImGui_ImplDX9_NewFrame();
		} else if (gRenderer == eRenderer::Dx11) {
			ImGui_ImplDX11_NewFrame();
		} else {
			ImGui_ImplOpenGL3_NewFrame();
		}

		ImGui::NewFrame();

		if (renderFunc != nullptr) {
			renderFunc();
		}

		ImGui::EndFrame();
		ImGui::Render();

		if (gRenderer == eRenderer::Dx9) {
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		} else if (gRenderer == eRenderer::Dx11) {
			pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		} else {
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	} else {
		if (!ImGui::GetCurrentContext()) {
			ImGui::CreateContext();
		}

		injector::MakeNOP(0x531155, 5); // shift trigger fix

		if (gRenderer == eRenderer::Dx9) {
			hwnd = GetForegroundWindow();
			if (!ImGui_ImplWin32_Init(hwnd)) {
				return;
			}

			if (!ImGui_ImplDX9_Init(reinterpret_cast<IDirect3DDevice9*>(ptr))) {
				return;
			}
			gD3DDevice = ptr;
		} else if (gRenderer == eRenderer::Dx11) {
			auto pSwapChain = reinterpret_cast<IDXGISwapChain*>(ptr);
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), &ptr))) {
				auto pDevice = reinterpret_cast<ID3D11Device*>(ptr);
				pDevice->GetImmediateContext(&pDeviceContext);

				DXGI_SWAP_CHAIN_DESC Desc;
				pSwapChain->GetDesc(&Desc);
				hwnd = Desc.OutputWindow;

				ID3D11Texture2D* backBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
				pDevice->CreateRenderTargetView(backBuffer, nullptr, &pRenderTargetView);
				backBuffer->Release();

				if (!ImGui_ImplWin32_Init(hwnd)) {
					return;
				}
				ImGui_ImplDX11_Init(pDevice, pDeviceContext);
				ImGui_ImplDX11_CreateDeviceObjects();
				ImGui::GetMainViewport()->PlatformHandleRaw = static_cast<void*>(hwnd);
			}

			gD3DDevice = ptr;
		} else {
			hwnd = GetForegroundWindow();
			if (!ImGui_ImplWin32_Init(hwnd)) {
				return;
			}

			if (!ImGui_ImplOpenGL3_Init()) {
				return;
			}
		}

		ImGui_ImplWin32_EnableDpiAwareness();

		ShowCursor(false);
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;

		oWndProc = (WNDPROC)SetWindowLongPtr(hwnd, -4, (LRESULT)hkWndProc); // GWL_WNDPROC = -4
		m_bInitialized = true;
	}
}

HRESULT ImguiHook::hkEndScene(IDirect3DDevice9* pDevice) {
	ProcessFrame(pDevice);
	return oEndScene(pDevice);
}

HRESULT ImguiHook::hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	ProcessFrame(pSwapChain);
	return oPresent(pSwapChain, SyncInterval, Flags);
}

HRESULT ImguiHook::hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT a, UINT b, UINT c, DXGI_FORMAT d, UINT e) {
	if (pRenderTargetView) {
		pRenderTargetView->Release();
		pRenderTargetView = nullptr;
		pDeviceContext->Flush();
	}

	HRESULT hr = oResizeBuffers(pSwapChain, a, b, c, d, e);
	ID3D11Texture2D* back_buffer{};
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
	reinterpret_cast<ID3D11Device*>(pSwapChain)->CreateRenderTargetView(back_buffer, nullptr, &pRenderTargetView);
	back_buffer->Release();
	return hr;
}

bool ImguiHook::hkGlSwapBuffer(HDC unnamedParam1, UINT unnamedParam2) {
	ProcessFrame(nullptr);
	return oGlSwapBuffer(unnamedParam1, unnamedParam2);
}

void ImguiHook::ProcessMouse() {
	static bool curState = false;
	if (curState != mouseVisible) {
		CPlayerPed* player = FindPlayerPed();
		CPad* pad = player ? player->GetPadFromPlayer() : nullptr;

		if (pad) {
			pad->DisablePlayerControls = mouseVisible;
		}

		ImGui::GetIO().MouseDrawCursor = mouseVisible;

		if (ImGui::GetIO().MouseDrawCursor) {
			plugin::patch::SetUChar(0x6194A0, 0xC3);
			plugin::patch::Nop(0x541DD7, 5);
			// Fix bug with radio switching
			plugin::patch::SetUChar(0x4EB731,
			                        0xEB); // jz -> jmp, skip mouse checks
			plugin::patch::SetUChar(0x4EB75A,
			                        0xEB); // jz -> jmp, skip mouse checks
		} else {
			plugin::patch::SetUChar(0x6194A0, 0xE9);
			plugin::patch::SetRaw(0x541DD7, (char*)"\xE8\xE4\xD5\xFF\xFF", 5);
			plugin::patch::SetUChar(0x4EB731, 0x74); // jz
			plugin::patch::SetUChar(0x4EB75A, 0x74); // jz
		}

		// ClearMouseStates
		CPad::UpdatePads();
		CPad::NewMouseControllerState.x = 0;
		CPad::NewMouseControllerState.y = 0;

		CPad::ClearMouseHistory();
		CPad::GetPad(0)->NewState.DPadUp = 0;
		CPad::GetPad(0)->OldState.DPadUp = 0;
		CPad::GetPad(0)->NewState.DPadDown = 0;
		CPad::GetPad(0)->OldState.DPadDown = 0;

		curState = mouseVisible;
	}
}

static bool IsKeyPressed(int i, LPVOID data) { return reinterpret_cast<char*>(data)[i] & 0x80; }

BOOL CALLBACK ImguiHook::hkSetCursorPos(int x, int y) {
	if (ImGui::GetIO().MouseDrawCursor) {
		return true;
	}
	return oSetCursorPos(x, y);
}

BOOL CALLBACK ImguiHook::hkShowCursor(bool flag) {
	if (ImGui::GetIO().MouseDrawCursor) {
		return oShowCursor(TRUE);
	}
	return oShowCursor(flag);
}

bool ImguiHook::Inject() {
	static bool injected;
	if (injected) {
		return false;
	}

	ImGui::CreateContext();
	MH_Initialize();
	PVOID pSetCursorPos = GetProcAddress(GetModuleHandle("user32.dll"), "SetCursorPos");
	PVOID pShowCursor = GetProcAddress(GetModuleHandle("user32.dll"), "ShowCursor");
	MH_CreateHook(pSetCursorPos, hkSetCursorPos, reinterpret_cast<LPVOID*>(&oSetCursorPos));
	MH_CreateHook(pShowCursor, hkShowCursor, reinterpret_cast<LPVOID*>(&oShowCursor));
	MH_EnableHook(pSetCursorPos);
	MH_EnableHook(pShowCursor);

	/*
	    Must check for d3d9 first!
	    Seems to crash with nvidia geforce experience overlay
	    if anything else is checked before d3d9
	*/
	if (GetModuleHandle("_gtaRenderHook.asi")) {
		goto dx11;
	}

	if (init(kiero::RenderType::D3D9) == kiero::Status::Success) {
		gRenderer = eRenderer::Dx9;
		injected = true;
		kiero::bind(16, reinterpret_cast<LPVOID*>(&oReset), hkReset);
		kiero::bind(42, reinterpret_cast<LPVOID*>(&oEndScene), hkEndScene);
	}

	if (init(kiero::RenderType::OpenGL) == kiero::Status::Success) {
		gRenderer = eRenderer::OpenGL;
		injected = true;

		HMODULE hMod = GetModuleHandle("OPENGL32.dll");
		if (!hMod) {
			return false;
		}
		FARPROC addr = GetProcAddress(hMod, "wglSwapBuffers");
		MH_CreateHook(addr, hkGlSwapBuffer, reinterpret_cast<LPVOID*>(&oGlSwapBuffer));
		MH_EnableHook(addr);
	}

dx11:
	if (init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		gRenderer = eRenderer::Dx11;
		kiero::bind(8, reinterpret_cast<LPVOID*>(&oPresent), hkPresent);
		kiero::bind(13, reinterpret_cast<LPVOID*>(&oResizeBuffers), hkResizeBuffers);
		injected = true;
	}

	return injected;
}

void ImguiHook::setRenderFunc(std::function<void()> func) { renderFunc = func; }

void ImguiHook::Remove() {
	renderFunc = nullptr;
	SetWindowLongPtr(hwnd, -4, (LRESULT)oWndProc); // GWL_WNDPROC = -4

	if (gRenderer == eRenderer::Dx9) {
		ImGui_ImplDX9_Shutdown();
	} else if (gRenderer == eRenderer::Dx11) {
		ImGui_ImplDX11_Shutdown();
	} else {
		ImGui_ImplOpenGL3_Shutdown();
	}

	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	kiero::shutdown();
}