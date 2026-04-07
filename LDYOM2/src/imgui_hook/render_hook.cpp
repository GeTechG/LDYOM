#include "render_hook.h"
#include "imgui_hook.h"
#include <logger.h>

HINSTANCE gDllHandle;

bool TryImGuiHook() {
    if (!ImguiHook::Inject()) {
        LDYOM_ERROR("Failed to install RenderWare hook");
        return false;
    }

    LDYOM_INFO("ImGui hooked successfully");
    return true;
}

void ImGuiHook() {
    TryImGuiHook();
}

void ImGuiHookRemove() {
    ImguiHook::Remove();
    LDYOM_INFO("ImGui hook removed");
}
