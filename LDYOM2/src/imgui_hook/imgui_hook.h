#pragma once
#pragma warning(push, 0)
#include <Windows.h>
#include <functional>
#pragma warning(pop)

class ImguiHook {
    using f_SetCursorPos = BOOL(CALLBACK*)(int, int);
    using f_ShowCursor   = BOOL(CALLBACK*)(bool);

    static inline WNDPROC              oWndProc      = nullptr;
    static inline f_SetCursorPos       oSetCursorPos = nullptr;
    static inline f_ShowCursor         oShowCursor   = nullptr;
    static inline HWND                 hwnd          = nullptr;

    static inline bool                 mouseVisible   = false;
    static inline bool                 controlEnabled = true;
    static inline bool                 dirtyObjects   = false;
    static inline std::function<void()> renderFunc    = nullptr;

    static void RenderImGui();
    static void InitImGui();
    static void ShutdownImGui();
    static void ProcessMouse();

    static LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static BOOL   CALLBACK hkSetCursorPos(int x, int y);
    static BOOL   CALLBACK hkShowCursor(bool flag);

  public:
    static inline bool m_bInitialized = false;

    ImguiHook() = delete;
    ImguiHook(const ImguiHook&) = delete;
    void operator=(const ImguiHook&) = delete;

    static bool GetMouseState();
    static void SetMouseState(bool state);
    static void SetControlEnabled(bool state);
    static void dirtyObjectsFlag();

    static bool Inject();
    static void Remove();
    static void setRenderFunc(std::function<void()> func);
};
