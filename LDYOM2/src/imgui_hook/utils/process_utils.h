#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

#include <windows.h>

// Структура для поиска окон процесса
struct WindowSearchData {
	DWORD processId;
	bool foundVisible;
	bool foundMinimized;
};

DWORD GetProcessIdFromModule(HMODULE hModule);
bool HasGameLaunched(HMODULE hModule, int maxRetries, int sleepDuration);
bool CheckAndPromptSilentPatch();

#endif // PROCESS_UTILS_H
