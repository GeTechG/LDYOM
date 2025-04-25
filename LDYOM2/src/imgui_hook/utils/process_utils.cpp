#include "process_utils.h"
#include <windows.h>
#include <psapi.h>
#include <string>

DWORD GetProcessIdFromModule(HMODULE hModule) {
    DWORD processId = 0;
    HANDLE hProcess = GetCurrentProcess();
    MODULEINFO moduleInfo;

    if (hProcess != nullptr) {
        if (GetModuleInformation(hProcess, hModule, &moduleInfo, sizeof(moduleInfo))) {
            processId = GetProcessId(hProcess);
        }
        CloseHandle(hProcess);
    }

    return processId;
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	DWORD processId = 0;
	GetWindowThreadProcessId(hwnd, &processId);

	if (processId == static_cast<DWORD>(lParam)) {
		return FALSE; // Stop
	}
	return TRUE; // Continue
}

bool HasGameLaunched(HMODULE hModule, int maxRetries, int sleepDuration) {
	DWORD processId = GetProcessIdFromModule(hModule);
	if (processId == 0) {
		MessageBox(nullptr, "Failed to get process ID", "ImGuiRedux", MB_ICONERROR);
		return false;
	}

	int retries = 0;
	while (retries < maxRetries) {
		Sleep(sleepDuration);
		if (!EnumWindows(EnumWindowsProc, static_cast<LPARAM>(processId))) {
			return true;
		}
		retries++;
	}
	MessageBox(nullptr, "Failed to detect game window.", "ImGuiRedux", MB_ICONERROR);
	return false;
}

bool CheckAndPromptSilentPatch() {
	std::string moduleName = "SilentPatchSA.asi";

	if (!GetModuleHandle(moduleName.c_str())) {
		int msgID = MessageBox(nullptr,
		                       "SilentPatch not found. Do you want to install Silent Patch? (Game restart required)",
		                       "LDYOM", MB_OKCANCEL | MB_DEFBUTTON1);
		if (msgID == IDOK) {
			ShellExecute(nullptr, "open", "https://gtaforums.com/topic/669045-silentpatch/", nullptr, nullptr,
			             SW_SHOWNORMAL);
		}
		return false;
	}
	return true;
}