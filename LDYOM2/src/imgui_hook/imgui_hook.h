#pragma once
#pragma warning(push, 0)
#ifdef LDYOM_BACKEND_DX11
#include <d3d11.h>
#endif
#include <d3d9.h>
#include <dinput.h>

#ifdef LDYOM_BACKEND_OPENGL
#include <GL/gl.h>
#endif
#include <functional>

#pragma warning(pop)

enum class eRenderer { Dx9,
#ifdef LDYOM_BACKEND_DX11
	Dx11,
#endif
#ifdef LDYOM_BACKEND_OPENGL
	OpenGL,
#endif
	Unknown };

class ImguiHook {
	using f_EndScene = HRESULT(CALLBACK*)(IDirect3DDevice9*);
	using f_Reset = HRESULT(CALLBACK*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	using f_SetCursorPos = BOOL(CALLBACK*)(int, int);
	using f_ShowCursor = BOOL(CALLBACK*)(bool);
	using f_GetDeviceState = HRESULT(CALLBACK*)(IDirectInputDevice8* pThis, DWORD cbData, LPVOID lpvData);

#ifdef LDYOM_BACKEND_DX11
	using f_Present = HRESULT(CALLBACK*)(IDXGISwapChain*, UINT, UINT);
	using f_ResizeBuffers = HRESULT(CALLBACK*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
#endif

#ifdef LDYOM_BACKEND_OPENGL
	using f_SwapBuffer = BOOL(CALLBACK*)(HDC, UINT);
#endif

	static inline WNDPROC oWndProc;
	static inline f_EndScene oEndScene;
	static inline f_Reset oReset;
	static inline f_SetCursorPos oSetCursorPos;
	static inline f_ShowCursor oShowCursor;
	static inline f_GetDeviceState oGetDeviceState;

#ifdef LDYOM_BACKEND_DX11
	static inline f_Present oPresent;
	static inline f_ResizeBuffers oResizeBuffers;
	static inline ID3D11DeviceContext* pDeviceContext;
	static inline ID3D11RenderTargetView* pRenderTargetView;
#endif

#ifdef LDYOM_BACKEND_OPENGL
	static inline f_SwapBuffer oGlSwapBuffer;
#endif

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

#ifdef LDYOM_BACKEND_DX11
	// DirectX11, Renderhook
	static HRESULT CALLBACK hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	static HRESULT CALLBACK hkResizeBuffers(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
#endif

#ifdef LDYOM_BACKEND_OPENGL
	// OpenGL
	static bool CALLBACK hkGlSwapBuffer(HDC unnamedParam1, UINT unnamedParam2);
#endif

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
