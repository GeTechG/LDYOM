#include "render_hook.h"
#include "imgui_hook.h"
#include "utils/process_utils.h"
#include <logger.h>

HINSTANCE gDllHandle;

bool TryImGuiHook() {
	// Сделать проверку более мягкой - сократить timeout
	if (!HasGameLaunched(gDllHandle, 10, 2000)) {
		LDYOM_WARN("Game window not detected yet");
		return false;
	}

	// Проверяем SilentPatch, но НЕ блокируем инициализацию если его нет
	// Просто показываем предупреждение один раз
	static bool silentPatchWarningShown = false;
	if (!silentPatchWarningShown) {
		CheckAndPromptSilentPatch();  // Может показать MessageBox, но не прерывает выполнение
		silentPatchWarningShown = true;
	}

	if (!ImguiHook::Inject()) {
		LDYOM_ERROR("Failed to inject DirectX hook");
		return false;
	}

	LDYOM_INFO("ImGui hooked successfully");
	return true;
}

// Обертка для обратной совместимости
void ImGuiHook() {
	TryImGuiHook();
}

void ImGuiHookRemove() {
	ImguiHook::Remove();
	LDYOM_INFO("ImGui hook removed");
}