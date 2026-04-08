#include "imgui_hook.h"
#include "MinHook.h"
#ifdef LDYOM_BACKEND_DX11
#include "imgui_impl_dx11.h"
#endif
#include "imgui_impl_dx9.h"
#ifdef LDYOM_BACKEND_OPENGL
#include "imgui_impl_opengl3.h"
#endif
#include "imgui_impl_win32.h"
#include "injector/injector.hpp"
#ifdef LDYOM_BACKEND_DX11
#include <d3d11.h>
#include <dxgi.h>
#endif
#include "pad.h"
#include "plugin.h"
#include "utils/imgui_configurate.h"
#include <CMenuManager.h>
#include <logger.h>


auto gRenderer = eRenderer::Unknown;
void* gD3DDevice = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool ImguiHook::GetMouseState() { return mouseVisible; }

void ImguiHook::SetMouseState(bool state) { mouseVisible = state; }

void ImguiHook::SetControlEnabled(bool state) { controlEnabled = state; }

void ImguiHook::dirtyObjectsFlag() { dirtyObjects = true; }

// Timer ID for window restoration
static constexpr UINT_PTR RESTORE_TIMER_ID = 1001;
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
			}
#ifdef LDYOM_BACKEND_DX11
			else if (gRenderer == eRenderer::Dx11) {
				ImGui_ImplDX11_InvalidateDeviceObjects();
			}
#endif
			ImGuiConfigurate::update(width, height);
			fScreenSize = ImVec2(static_cast<float>(width), static_cast<float>(height));
			dirtyObjects = false;
		}

		// ScriptExData::InitRenderStates();

		ImGui_ImplWin32_NewFrame();
		if (gRenderer == eRenderer::Dx9) {
			ImGui_ImplDX9_NewFrame();
		}
#ifdef LDYOM_BACKEND_DX11
		else if (gRenderer == eRenderer::Dx11) {
			ImGui_ImplDX11_NewFrame();
		}
#endif
#ifdef LDYOM_BACKEND_OPENGL
		else if (gRenderer == eRenderer::OpenGL) {
			ImGui_ImplOpenGL3_NewFrame();
		}
#endif

		ImGui::NewFrame();

		if (renderFunc != nullptr) {
			renderFunc();
		}

		ImGui::EndFrame();
		ImGui::Render();

		if (gRenderer == eRenderer::Dx9) {
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		}
#ifdef LDYOM_BACKEND_DX11
		else if (gRenderer == eRenderer::Dx11) {
			pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
#endif
#ifdef LDYOM_BACKEND_OPENGL
		else if (gRenderer == eRenderer::OpenGL) {
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
#endif
	} else {
		if (!ImGui::GetCurrentContext()) {
			ImGui::CreateContext();
		}

		injector::MakeNOP(0x531155, 5); // shift trigger fix

		if (gRenderer == eRenderer::Dx9) {
			// Получаем HWND из DirectX device вместо GetForegroundWindow()
			// GetForegroundWindow() может вернуть неправильное окно если игра свернута
			auto pDevice = reinterpret_cast<IDirect3DDevice9*>(ptr);
			IDirect3DSwapChain9* pSwapChain = nullptr;

			if (SUCCEEDED(pDevice->GetSwapChain(0, &pSwapChain))) {
				D3DPRESENT_PARAMETERS presentParams;
				if (SUCCEEDED(pSwapChain->GetPresentParameters(&presentParams))) {
					hwnd = presentParams.hDeviceWindow;
				}
				pSwapChain->Release();
			}

			// Fallback к GetForegroundWindow если не удалось получить из device
			if (hwnd == nullptr) {
				hwnd = GetForegroundWindow();
			}

			if (!ImGui_ImplWin32_Init(hwnd)) {
				LDYOM_ERROR("ImGui_ImplWin32_Init failed!");
				return;
			}

			if (!ImGui_ImplDX9_Init(reinterpret_cast<IDirect3DDevice9*>(ptr))) {
				LDYOM_ERROR("ImGui_ImplDX9_Init failed!");
				return;
			}

			gD3DDevice = ptr;
		}
#ifdef LDYOM_BACKEND_DX11
		else if (gRenderer == eRenderer::Dx11) {
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
		}
#endif
#ifdef LDYOM_BACKEND_OPENGL
		else if (gRenderer == eRenderer::OpenGL) {
			hwnd = GetForegroundWindow();
			if (!ImGui_ImplWin32_Init(hwnd)) {
				return;
			}

			if (!ImGui_ImplOpenGL3_Init()) {
				return;
			}
		}
#endif

		ImGui_ImplWin32_EnableDpiAwareness();

		ShowCursor(false);
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;

		oWndProc = (WNDPROC)SetWindowLongPtr(hwnd, -4, (LRESULT)hkWndProc); // GWL_WNDPROC = -4

		m_bInitialized = true;
		LDYOM_INFO("ImGui initialized successfully");
	}
}

HRESULT ImguiHook::hkEndScene(IDirect3DDevice9* pDevice) {
	ProcessFrame(pDevice);
	return oEndScene(pDevice);
}

#ifdef LDYOM_BACKEND_DX11
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
#endif

#ifdef LDYOM_BACKEND_OPENGL
bool ImguiHook::hkGlSwapBuffer(HDC unnamedParam1, UINT unnamedParam2) {
	ProcessFrame(nullptr);
	return oGlSwapBuffer(unnamedParam1, unnamedParam2);
}
#endif

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
	static bool injected = false;
	static bool initAttempted = false;

	// Если уже инжектили, возвращаем true (успех)
	if (injected) {
		return true;
	}

	// Проверяем gRenderer - если уже установлен, значит hooks работают
	if (gRenderer != eRenderer::Unknown) {
		injected = true;
		return true;
	}

	// Инициализация выполняется только один раз
	if (!initAttempted) {
		// ВАЖНО: Создаем контекст СРАЗУ, потому что Application может начать
		// использовать ImGui API до первого вызова EndScene
		if (!ImGui::GetCurrentContext()) {
			ImGui::CreateContext();
		}

		MH_Initialize();
		PVOID pSetCursorPos = GetProcAddress(GetModuleHandle("user32.dll"), "SetCursorPos");
		PVOID pShowCursor = GetProcAddress(GetModuleHandle("user32.dll"), "ShowCursor");
		MH_CreateHook(pSetCursorPos, hkSetCursorPos, reinterpret_cast<LPVOID*>(&oSetCursorPos));
		MH_CreateHook(pShowCursor, hkShowCursor, reinterpret_cast<LPVOID*>(&oShowCursor));
		MH_EnableHook(pSetCursorPos);
		MH_EnableHook(pShowCursor);

		initAttempted = true;
	}

	// Попытка установки hooks для рендера (можно вызывать повторно)
	if (!injected) {
		/*
		    Must check for d3d9 first!
		    Seems to crash with nvidia geforce experience overlay
		    if anything else is checked before d3d9
		*/
#ifdef LDYOM_BACKEND_DX11
		if (GetModuleHandle("_gtaRenderHook.asi")) {
			goto dx11;
		}
#endif

		// Get D3D9 device directly from GTA SA's RenderWare engine (RwD3D9Device at 0xC97C28)
		// This avoids creating a temporary device (which conflicts with UAL's wndmode.dll)
		{
			auto* pDevice = *reinterpret_cast<IDirect3DDevice9**>(0xC97C28);
			if (pDevice) {
				auto* vtable = *reinterpret_cast<void***>(pDevice);
				// IDirect3DDevice9 vtable: index 16 = Reset, index 42 = EndScene
				MH_CreateHook(vtable[16], hkReset, reinterpret_cast<LPVOID*>(&oReset));
				MH_CreateHook(vtable[42], hkEndScene, reinterpret_cast<LPVOID*>(&oEndScene));
				MH_EnableHook(vtable[16]);
				MH_EnableHook(vtable[42]);
				gRenderer = eRenderer::Dx9;
				injected = true;
			}
		}

#ifdef LDYOM_BACKEND_OPENGL
		if (!injected) {
			HMODULE hMod = GetModuleHandle("OPENGL32.dll");
			if (hMod) {
				FARPROC addr = GetProcAddress(hMod, "wglSwapBuffers");
				if (addr) {
					MH_CreateHook(reinterpret_cast<LPVOID>(addr), hkGlSwapBuffer, reinterpret_cast<LPVOID*>(&oGlSwapBuffer));
					MH_EnableHook(reinterpret_cast<LPVOID>(addr));
					gRenderer = eRenderer::OpenGL;
					injected = true;
				}
			}
		}
#endif

#ifdef LDYOM_BACKEND_DX11
	dx11:
		if (!injected && GetModuleHandle("d3d11.dll")) {
			// Create temporary D3D11 device to get IDXGISwapChain vtable
			WNDCLASSEX wc = {};
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = DefWindowProc;
			wc.hInstance = GetModuleHandle(nullptr);
			wc.lpszClassName = "LdyomTemp";
			RegisterClassEx(&wc);
			HWND tempWnd = CreateWindow(wc.lpszClassName, "Temp", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, wc.hInstance, NULL);

			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferCount = 1;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferDesc.Width = 100;
			swapChainDesc.BufferDesc.Height = 100;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.OutputWindow = tempWnd;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.Windowed = TRUE;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

			D3D_FEATURE_LEVEL featureLevel;
			constexpr D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0};
			IDXGISwapChain* tempSwapChain = nullptr;
			ID3D11Device* tempDevice = nullptr;
			ID3D11DeviceContext* tempContext = nullptr;

			HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
				featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc,
				&tempSwapChain, &tempDevice, &featureLevel, &tempContext);

			if (SUCCEEDED(hr)) {
				// IDXGISwapChain vtable: index 8 = Present, index 13 = ResizeBuffers
				auto* vtable = *reinterpret_cast<void***>(tempSwapChain);
				MH_CreateHook(vtable[8], hkPresent, reinterpret_cast<LPVOID*>(&oPresent));
				MH_CreateHook(vtable[13], hkResizeBuffers, reinterpret_cast<LPVOID*>(&oResizeBuffers));
				MH_EnableHook(vtable[8]);
				MH_EnableHook(vtable[13]);
				gRenderer = eRenderer::Dx11;
				injected = true;
			}

			if (tempContext) tempContext->Release();
			if (tempDevice) tempDevice->Release();
			if (tempSwapChain) tempSwapChain->Release();
			DestroyWindow(tempWnd);
			UnregisterClass(wc.lpszClassName, wc.hInstance);
		}
#endif
	}

	return injected;
}

void ImguiHook::setRenderFunc(std::function<void()> func) { renderFunc = func; }

void ImguiHook::Remove() {
	renderFunc = nullptr;
	SetWindowLongPtr(hwnd, -4, (LRESULT)oWndProc); // GWL_WNDPROC = -4

	if (gRenderer == eRenderer::Dx9) {
		ImGui_ImplDX9_Shutdown();
	}
#ifdef LDYOM_BACKEND_DX11
	else if (gRenderer == eRenderer::Dx11) {
		ImGui_ImplDX11_Shutdown();
	}
#endif
#ifdef LDYOM_BACKEND_OPENGL
	else if (gRenderer == eRenderer::OpenGL) {
		ImGui_ImplOpenGL3_Shutdown();
	}
#endif

	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	MH_DisableHook(MH_ALL_HOOKS);
}
