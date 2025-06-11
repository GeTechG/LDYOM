#include "process_utils.h"
#include <psapi.h>
#include <string>
#include <windows.h>

BOOL CALLBACK EnumWindowsSearchProc(HWND hwnd, LPARAM lParam) {
	WindowSearchData* data = reinterpret_cast<WindowSearchData*>(lParam);
	DWORD processId = 0;
	GetWindowThreadProcessId(hwnd, &processId);

	if (processId == data->processId) {
		if (IsWindowVisible(hwnd)) {
			if (IsIconic(hwnd)) {
				data->foundMinimized = true;
			} else {
				data->foundVisible = true;
				return FALSE; // Stop - найдено видимое, не свернутое окно
			}
		}
	}
	return TRUE; // Continue
}

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
		// Проверяем, что окно видимо и не свернуто
		if (IsWindowVisible(hwnd) && !IsIconic(hwnd)) {
			return FALSE; // Stop - найдено видимое, не свернутое окно
		}
	}
	return TRUE; // Continue
}

bool HasGameLaunched(HMODULE hModule, int maxRetries, int sleepDuration) {
	DWORD processId = GetProcessIdFromModule(hModule);
	if (processId == 0) {
		MessageBox(nullptr, "Failed to get process ID", "LDYOM", MB_ICONERROR);
		return false;
	}

	int retries = 0;
	while (retries < maxRetries) {

		WindowSearchData searchData = {processId, false, false};
		EnumWindows(EnumWindowsSearchProc, reinterpret_cast<LPARAM>(&searchData));

		if (searchData.foundVisible) {
			return true; // Найдено видимое, не свернутое окно
		}

		// Если найдено только свернутое окно, продолжаем ожидание
		if (searchData.foundMinimized && retries == maxRetries - 1) {
			MessageBox(nullptr, "Game window is minimized. Please restore the game window and try again.", "LDYOM",
			           MB_ICONWARNING);
			return false;
		}

		Sleep(sleepDuration);

		retries++;
	}

	MessageBox(nullptr, "Failed to detect visible game window. Game may not be launched properly.", "LDYOM",
	           MB_ICONERROR);
	return false;
}

bool CheckAndPromptSilentPatch() {
	std::string moduleName = "SilentPatchSA.asi";

	if (!GetModuleHandle(moduleName.c_str())) {
		int msgID =
			MessageBox(nullptr, "SilentPatch not found. Do you want to install Silent Patch? (Game restart required)",
		               "LDYOM", MB_OKCANCEL | MB_DEFBUTTON1);
		if (msgID == IDOK) {
			ShellExecute(nullptr, "open", "https://gtaforums.com/topic/669045-silentpatch/", nullptr, nullptr,
			             SW_SHOWNORMAL);
		}
		return false;
	}
	return true;
}