#pragma once
#pragma warning(push, 0)
#include <d3d11.h>
#include <d3d9.h>
#include <dinput.h>

#include <GL/gl.h>
#include <functional>

#pragma warning(pop)

enum class eRenderer { Dx9, Dx11, Dx12, OpenGL, Unknown };

class ImguiHook {
	using f_EndScene = HRESULT(CALLBACK*)(IDirect3DDevice9*);
	using f_Present = HRESULT(CALLBACK*)(IDXGISwapChain*, UINT, UINT);
	using f_Reset = HRESULT(CALLBACK*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	using f_ResizeBuffers = HRESULT(CALLBACK*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
	using f_SetCursorPos = BOOL(CALLBACK*)(int, int);
	using f_ShowCursor = BOOL(CALLBACK*)(bool);
	using f_SwapBuffer = BOOL(CALLBACK*)(HDC, UINT);
	using f_GetDeviceState = HRESULT(CALLBACK*)(IDirectInputDevice8* pThis, DWORD cbData, LPVOID lpvData);

	static inline WNDPROC oWndProc;
	static inline f_Present oPresent;
	static inline f_EndScene oEndScene;
	static inline f_Reset oReset;
	static inline f_ResizeBuffers oResizeBuffers;
	static inline f_SetCursorPos oSetCursorPos;
	static inline f_ShowCursor oShowCursor;
	static inline f_SwapBuffer oGlSwapBuffer;
	static inline f_GetDeviceState oGetDeviceState;
	static inline ID3D11DeviceContext* pDeviceContext;
	static inline ID3D11RenderTargetView* pRenderTargetView;

	static inline bool mouseVisible;
	static inline bool controlEnabled = true;
	static inline std::function<void()> renderFunc = nullptr;
	static inline HWND hwnd = nullptr;
	static inline bool dirtyObjects = false;

	static void CALLBACK ProcessFrame(void* ptr);
	static LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void ProcessMouse();

	static BOOL CALLBACK hkSetCursorPos(int, int);
	static BOOL CALLBACK hkShowCursor(bool);
	// DirectX9
	static HRESULT CALLBACK hkEndScene(IDirect3DDevice9* pDevice);
	static HRESULT CALLBACK hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

	// DirectX11, Renderhook
	static HRESULT CALLBACK hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	static HRESULT CALLBACK hkResizeBuffers(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);

	// OpenGL
	static bool CALLBACK hkGlSwapBuffer(HDC unnamedParam1, UINT unnamedParam2);

  public:
	static inline bool m_bInitialized = false;

	ImguiHook() = delete;
	ImguiHook(const ImguiHook&) = delete;
	void operator=(const ImguiHook&) = delete;

	static bool GetMouseState();
	static bool Inject();
	static void setRenderFunc(std::function<void()> func);
	static void Remove();
	static void SetMouseState(bool state);
	static void SetControlEnabled(bool state);
	static void dirtyObjectsFlag();
};
