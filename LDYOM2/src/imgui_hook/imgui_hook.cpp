#include "imgui_hook.h"

#include "MinHook.h"
#include "imgui_impl_rw.h"
#include "imgui_impl_win32.h"
#include "injector/injector.hpp"
#include "plugin.h"
#include "utils/imgui_configurate.h"
#include <CMenuManager.h>
#include <logger.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool ImguiHook::GetMouseState() { return mouseVisible; }
void ImguiHook::SetMouseState(bool state) { mouseVisible = state; }
void ImguiHook::SetControlEnabled(bool state) { controlEnabled = state; }
void ImguiHook::dirtyObjectsFlag() { dirtyObjects = true; }

LRESULT ImguiHook::hkWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (m_bInitialized) {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

        const bool disableControl = !controlEnabled && mouseVisible && wParam != VK_ESCAPE;
        if ((ImGui::GetIO().WantTextInput || disableControl) && !FrontEndMenuManager.m_bMenuActive) {
            plugin::Call<0x53F1E0>(); // CPad::ClearKeyboardHistory
            return 1;
        }
        if (disableControl && wParam == VK_HOME)
            return 1;
    }

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK ImguiHook::hkSetCursorPos(int x, int y) {
    if (ImGui::GetIO().MouseDrawCursor) {
        return TRUE;
    }
    return oSetCursorPos(x, y);
}

BOOL CALLBACK ImguiHook::hkShowCursor(bool flag) {
    if (ImGui::GetIO().MouseDrawCursor) {
        return oShowCursor(TRUE);
    }
    return oShowCursor(flag);
}

void ImguiHook::ProcessMouse() {
    static bool curState = false;
    if (curState == mouseVisible)
        return;

    CPlayerPed* player = FindPlayerPed();
    CPad* pad = player ? player->GetPadFromPlayer() : nullptr;
    if (pad) {
        pad->DisablePlayerControls = mouseVisible;
    }

    ImGui::GetIO().MouseDrawCursor = mouseVisible;

    if (mouseVisible) {
        plugin::patch::SetUChar(0x6194A0, 0xC3);
        plugin::patch::Nop(0x541DD7, 5);
        plugin::patch::SetUChar(0x4EB731, 0xEB);
        plugin::patch::SetUChar(0x4EB75A, 0xEB);
    } else {
        plugin::patch::SetUChar(0x6194A0, 0xE9);
        plugin::patch::SetRaw(0x541DD7, (char*)"\xE8\xE4\xD5\xFF\xFF", 5);
        plugin::patch::SetUChar(0x4EB731, 0x74);
        plugin::patch::SetUChar(0x4EB75A, 0x74);
    }

    CPad::UpdatePads();
    CPad::NewMouseControllerState.x = 0;
    CPad::NewMouseControllerState.y = 0;
    CPad::ClearMouseHistory();
    CPad::GetPad(0)->NewState.DPadUp   = 0;
    CPad::GetPad(0)->OldState.DPadUp   = 0;
    CPad::GetPad(0)->NewState.DPadDown = 0;
    CPad::GetPad(0)->OldState.DPadDown = 0;

    curState = mouseVisible;
}

void ImguiHook::InitImGui() {
    if (!ImGui::GetCurrentContext()) {
        ImGui::CreateContext();
    }
    ImGui_ImplWin32_Init(RsGlobal.ps->window);
    ImGui_ImplRW_Init();
    ImGui_ImplWin32_EnableDpiAwareness();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;

    hwnd = RsGlobal.ps->window;
    oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hwnd, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndProc)));

    injector::MakeNOP(0x531155, 5); // shift trigger fix

    int width  = static_cast<int>(plugin::screen::GetScreenWidth());
    int height = static_cast<int>(plugin::screen::GetScreenHeight());
    ImGuiConfigurate::update(width, height);

    LDYOM_INFO("ImGui (RenderWare backend) initialized");
}

void ImguiHook::RenderImGui() {
    static bool imguiInitialized = false;
    if (!imguiInitialized) {
        InitImGui();
        imguiInitialized = true;
    }

    if (!FrontEndMenuManager.m_bMenuActive) {
        ProcessMouse();

        // Rebuild fonts/style when screen size changes or dirty flag set
        static int prevWidth = 0, prevHeight = 0;
        int width  = static_cast<int>(plugin::screen::GetScreenWidth());
        int height = static_cast<int>(plugin::screen::GetScreenHeight());
        if (prevWidth != width || prevHeight != height || dirtyObjects) {
            ImGuiConfigurate::update(width, height);
            prevWidth  = width;
            prevHeight = height;
            dirtyObjects = false;
        }

        if (ImGui::IsKeyPressed(ImGuiKey_RightAlt, false)) {
            ImGui::GetIO().AddKeyEvent(ImGuiKey_Escape, true);
        }

        ImGui_ImplWin32_NewFrame();
        ImGui_ImplRW_NewFrame();
        ImGui::NewFrame();

        if (renderFunc) {
            renderFunc();
        }

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplRW_RenderDrawData(ImGui::GetDrawData());
    } else {
        // Menu is open: hide cursor but do not render
        bool saved = mouseVisible;
        mouseVisible = false;
        ProcessMouse();
        mouseVisible = saved;
    }
}

void ImguiHook::ShutdownImGui() {
    if (oWndProc) {
        SetWindowLongPtr(hwnd, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));
        oWndProc = nullptr;
    }
    ImGui_ImplRW_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool ImguiHook::Inject() {
    if (m_bInitialized)
        return true;

    // IMPORTANT: Create context immediately — Application may call ImGui APIs
    // before the first render frame fires InitImGui().
    if (!ImGui::GetCurrentContext()) {
        ImGui::CreateContext();
    }

    // Hook cursor API via MinHook
    MH_Initialize();
    PVOID pSetCursorPos = GetProcAddress(GetModuleHandle("user32.dll"), "SetCursorPos");
    PVOID pShowCursor   = GetProcAddress(GetModuleHandle("user32.dll"), "ShowCursor");
    MH_CreateHook(pSetCursorPos, hkSetCursorPos, reinterpret_cast<LPVOID*>(&oSetCursorPos));
    MH_CreateHook(pShowCursor,   hkShowCursor,   reinterpret_cast<LPVOID*>(&oShowCursor));
    MH_EnableHook(pSetCursorPos);
    MH_EnableHook(pShowCursor);

    // Hook into RenderWare draw events
    static plugin::CdeclEvent<plugin::AddressList<0x53EB12, plugin::H_CALL>,
                              plugin::PRIORITY_AFTER,
                              plugin::ArgPickNone,
                              void()>
        draw2dStuffEvent;

    draw2dStuffEvent += []() { RenderImGui(); };
    plugin::Events::drawMenuBackgroundEvent += []() { RenderImGui(); };

    m_bInitialized = true;
    LDYOM_INFO("RenderWare hook installed");
    return true;
}

void ImguiHook::setRenderFunc(std::function<void()> func) { renderFunc = std::move(func); }

void ImguiHook::Remove() {
    renderFunc = nullptr;
    ShutdownImGui();
    MH_DisableHook(MH_ALL_HOOKS);
    m_bInitialized = false;
}
