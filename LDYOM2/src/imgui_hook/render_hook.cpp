#include "render_hook.h"
#include "imgui_hook.h"
#include "utils/process_utils.h"
#include <logger.h>

HINSTANCE gDllHandle;

void ImGuiHook() {
	if (!HasGameLaunched(gDllHandle, 30, 5000)) {
		return;
	}

	if (!CheckAndPromptSilentPatch()) {
		return;
	}

	if (!ImguiHook::Inject()) {
		MessageBox(HWND_DESKTOP, "Failed to inject dxhook..", "LDYOM", MB_ICONERROR);
	}

	LDYOM_INFO("ImGui hooked successfully");
}

void ImGuiHookRemove() {
	ImguiHook::Remove();
	LDYOM_INFO("ImGui hook removed");
}